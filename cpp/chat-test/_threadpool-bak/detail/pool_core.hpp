/*
 * PoolCore.hpp
 *
 * Created on: 2023/4/10 14:14.
 *     Author: hou-lei
 */
#ifndef CPP_POOLCORE_HPP
#define CPP_POOLCORE_HPP

#include <memory>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "util/non_copyable.hpp"
#include "threadpool/detail/worker_thread.hpp"

namespace chat {
namespace threadpool {

    template <
            typename Task,
            template <typename> class SchedulingPolicy,
//            template <typename> class SizePolicy,
            template <typename> class AutoShutdownPolicy
    >
    class pool_core
            : public std::enable_shared_from_this< pool_core<Task, SchedulingPolicy, AutoShutdownPolicy> >
            , private chat::util::non_copyable {

    public: // Type definitions
        typedef Task task_type;                                 //!< Indicates the task's type.
        typedef SchedulingPolicy<task_type> scheduler_type;     //!< Indicates the scheduler's type.
        typedef pool_core<Task, SchedulingPolicy, AutoShutdownPolicy> pool_type;           //!< Indicates the thread pool's type.
        typedef AutoShutdownPolicy<pool_type> auto_shutdown_policy_type;//!< Indicates the shutdown policy's type.

        typedef worker_thread<pool_type> worker_type;
    private: // Friends
        friend class worker_thread<pool_type>;
        friend class AutoShutdownPolicy<pool_type>;


    private: // The following members are implemented thread-safe:
        mutable std::recursive_mutex  m_monitor;
        mutable std::condition_variable_any m_worker_idle_or_terminated_event;	// A worker is idle or was terminated.
        mutable std::condition_variable_any m_task_or_terminate_workers_event;  // Task is available OR total worker count should be reduced.

    private: // The following members may be accessed by _multiple_ threads at the same time:
        volatile size_t m_worker_count;
        volatile size_t m_target_worker_count;
        volatile size_t m_active_worker_count;

    private: // The following members are accessed only by _one_ thread at the same time:
        scheduler_type  m_scheduler;
    //    scoped_ptr<size_policy_type> m_size_policy; // is never null

        bool  m_terminate_all_workers;								// Indicates if termination of all workers was triggered.
        std::vector<shared_ptr<worker_type> > m_terminated_workers; // List of workers which are terminated but not fully destructed.

    private:
        auto_shutdown_policy_type m_auto_shutdown;

    public:
        /// Constructor.
        pool_core(size_t initial_threads = 0)
                : m_worker_count(0)
                , m_target_worker_count(0)
                , m_active_worker_count(0)
                , m_terminate_all_workers(false)
                , m_auto_shutdown() {

            pool_type volatile & self_ref = *this;
    //        m_size_policy.reset(new size_policy_type(self_ref));
    //        m_scheduler.clear();
            this->resize(initial_threads);
        }

        ~pool_core(){}

        /**
         * Gets the number of threads in the pool.
         * @return The number of threads.
         */
        size_t size() const volatile {
            return m_worker_count;
        }

        /**
         * only called once
         */
        void auto_shutdown() {
            m_auto_shutdown.shutdown(*this);
        }

        /**
         * Returns the number of tasks which are currently executed.
         * @return The number of active tasks.
         */
        size_t active() const volatile {
            return m_active_worker_count;
        }

        /**
         * Returns the number of tasks which are ready for execution.
         * @return The number of pending tasks.
         */
        size_t pending() const volatile {
            std::lock_guard<std::recursive_mutex> lockGuard(this->m_monitor);
            return this->m_scheduler.size();
        }


        /**
         * Schedules a task for asynchronous execution. The task will be executed once only.
         * @param task The task function object. It should not throw execeptions.
         * @return true, if the task could be scheduled and false otherwise.
         */
        bool schedule(task_type const & task)  { // volatile
            std::lock_guard<std::recursive_mutex> lockGuard(this->m_monitor);
            if (this->m_scheduler.push(task)) {
                this->m_task_or_terminate_workers_event.notify_one();
                return true;
            }
            return false;
        }


        /**
         * Removes all pending tasks from the pool's scheduler.
         */
        void clear() volatile {
            std::lock_guard<std::recursive_mutex> lockGuard(this->m_monitor);
            this->m_scheduler.clear();
        }

        /**
         * Indicates that there are no tasks pending.
         * @return true if there are no tasks ready for execution.
         * @remarks This function is more efficient that the check 'pending() == 0'.
         */
        bool empty() const volatile {
            std::lock_guard<std::recursive_mutex> lockGuard(this->m_monitor);
            return this->m_scheduler.empty();
        }

        /**
         * The current thread of execution is blocked until the sum of all active
         * and pending tasks is equal or less than a given threshold.
         * @param task_threshold The maximum number of tasks in pool and scheduler.
         */
        void wait(size_t const task_threshold = 0) const  { // volatile
            const pool_type* self = const_cast<const pool_type*>(this);
            std::unique_lock<std::recursive_mutex> lock(this->m_monitor);
            if(0 == task_threshold) {
                while(0 != self->m_active_worker_count || !self->m_scheduler.empty()) {
                    self->m_worker_idle_or_terminated_event.wait(lock);
                }
            } else {
                while(task_threshold < self->m_active_worker_count + self->m_scheduler.size()) {
                    self->m_worker_idle_or_terminated_event.wait(lock);
                }
            }
        }

    private:
        void terminate_all_workers(bool const wait)  { // volatile
            pool_type* self = const_cast<pool_type*>(this);
            std::unique_lock<std::recursive_mutex> lock(this->m_monitor);

            self->m_terminate_all_workers = true;
            m_target_worker_count = 0;
            self->m_task_or_terminate_workers_event.notify_all();

            if(wait) {
                while(m_active_worker_count > 0) {
                    self->m_worker_idle_or_terminated_event.wait(lock);
                }

                for (typename std::vector<std::shared_ptr<worker_type> >::iterator it = self->m_terminated_workers.begin();
                        it != self->m_terminated_workers.end();
                        ++it ) {

                    (*it)->join();
                }

                self->m_terminated_workers.clear();
            }
        }


        /*! Changes the number of worker threads in the pool. The resizing
        *  is handled by the SizePolicy.
        * \param threads The new number of worker threads.
        * \return true, if pool will be resized and false if not.
        */
        bool resize(size_t const worker_count)  { // volatile

            std::lock_guard<std::recursive_mutex> lockGuard(this->m_monitor);

            if(!m_terminate_all_workers) {
                m_target_worker_count = worker_count;
            } else {
                return false;
            }

            if(m_worker_count <= m_target_worker_count) {
                // increase worker count
                while(m_worker_count < m_target_worker_count) {
                    try {
//                        worker_thread<pool_type>::create_and_attach(this->shared_from_this());
                        std::make_shared<worker_thread<pool_type>>(this->shared_from_this());
                        m_worker_count++;
                        m_active_worker_count++;
                    } catch(...) {
                        return false;
                    }
                }
            } else {
                // decrease worker count
                this->m_task_or_terminate_workers_event.notify_all();   // TODO: Optimize number of notified workers
            }

            return true;
        }


        // worker died with unhandled exception
        void worker_died_unexpectedly(std::shared_ptr<worker_type> worker)  { // volatile
            std::lock_guard<std::recursive_mutex> lock(this->m_monitor);

            m_worker_count--;
            m_active_worker_count--;
            this->m_worker_idle_or_terminated_event.notify_all();

            if(m_terminate_all_workers) {
                this->m_terminated_workers.push_back(worker);
            } else {
//                this->m_size_policy->worker_died_unexpectedly(m_worker_count);
                this->resize(m_worker_count + 1);
            }
        }

        void worker_destructed(std::shared_ptr<worker_type> worker)  { // volatile
            std::lock_guard<std::recursive_mutex> lock(this->m_monitor);
            m_worker_count--;
            m_active_worker_count--;
            this->m_worker_idle_or_terminated_event.notify_all();

            if(m_terminate_all_workers) {
                this->m_terminated_workers.push_back(worker);
            }
        }


        bool worker_execute_task(std::shared_ptr<worker_thread<pool_type>> worker) {// volatile
            std::function<void(void)> task = nullptr;

            { // fetch task
                pool_type* lockedThis = const_cast<pool_type*>(this);
                std::unique_lock<std::recursive_mutex> lock(this->m_monitor);

                // decrease number of threads if necessary
                if(m_worker_count > m_target_worker_count) {
                    return false;	// terminate worker
                }


                // wait for tasks
                while(lockedThis->m_scheduler.empty()) {
                    // decrease number of workers if necessary
                    if(m_worker_count > m_target_worker_count) {
                        return false;	// terminate worker
                    } else {
                        m_active_worker_count--;
                        lockedThis->m_worker_idle_or_terminated_event.notify_all();
                        lockedThis->m_task_or_terminate_workers_event.wait(lock);
                        m_active_worker_count++;
                    }
                }

                task = lockedThis->m_scheduler.pop();
            }

            // call task function
            if(task != nullptr) {
                task();
            }

            //guard->disable();
            return true;
        }

    };

}
}
#endif //CPP_POOLCORE_HPP
