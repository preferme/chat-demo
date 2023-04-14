/*
 * _threadpool-bak.hpp
 *
 * Created on: 2023/4/10 17:22.
 *     Author: hou-lei
 */
#ifndef CPP_THREADPOOL_HPP
#define CPP_THREADPOOL_HPP

#include <memory>

#include "threadpool/adaptors/task_adaptors.hpp"
#include "threadpool/policies/task_scheduling_policies.hpp"
#include "threadpool/policies/auto_shutdown_policies.hpp"

#include "threadpool/detail/pool_core.hpp"


namespace chat {
namespace threadpool {

    template <
            typename Task                               = general_task,
            template<typename> class SchedulingPolicy   = fifo_task_scheduler,

            template<typename> class AutoShutdownPolicy = wait_for_all_tasks
            >
    class pool_core {

    public: // Type definitions
        typedef Task task_type;                                   //!< Indicates the task's type.
        typedef SchedulingPolicy<task_type> scheduler_type;       //!< Indicates the scheduler's type.
    private:
        typedef pool_core<Task, SchedulingPolicy, AutoShutdownPolicy> pool_core_type;

        std::shared_ptr<pool_core_type> m_core;
        // If the last pool holding a pointer to the core is deleted the controller shuts the pool down.
        std::shared_ptr<void>           m_auto_shutdown_controoler;

    public:
        pool_core(size_t core_size = 0, size_t max_size = 0)
                : m_core(new pool_core_type(core_size))
                , m_auto_shutdown_controoler(nullptr, std::bind(&pool_core_type::auto_shutdown, m_core)) {
//            size_policy_type::init(*m_core, initial_threads);
//            m_core->resize(core_size);
        }


        /**
         * Gets the number of threads in the pool.
         * @return The number of threads.
         */
        size_t size()	const {
            return m_core->size();
        }


        /**
         * Schedules a task for asynchronous execution. The task will be executed once only.
         * @param task The task function object. It should not throw execeptions.
         * @return true, if the task could be scheduled and false otherwise.
         */
        bool schedule(task_type const & task) {
            return m_core->schedule(task);
        }


        /**
         * Returns the number of tasks which are currently executed.
         * @return The number of active tasks.
         */
        size_t active() const {
            return m_core->active();
        }


        /**
         * Returns the number of tasks which are ready for execution.
         * @return The number of pending tasks.
         */
        size_t pending() const {
            return m_core->pending();
        }


        /**
         * Removes all pending tasks from the pool's scheduler.
         */
        void clear() {
            m_core->clear();
        }


        /**
         * Indicates that there are no tasks pending.
         * @return true if there are no tasks ready for execution.
         * @remarks This function is more efficient that the check 'pending() == 0'.
         */
        bool empty() const {
            return m_core->empty();
        }


        /**
         * The current thread of execution is blocked until the sum of all active
         *  and pending tasks is equal or less than a given threshold.
         * @param task_threshold The maximum number of tasks in pool and scheduler.
         */
        void wait(size_t task_threshold = 0) const {
            m_core->wait(task_threshold);
        }

    };


    /**
     * @brief Fifo pool.
     *
     * The pool's tasks are fifo scheduled task_func functors.
     */
    typedef thread_pool<general_task, fifo_task_scheduler, wait_for_all_tasks> fifo_pool;


    /**
     * @brief Lifo pool.
     *
     * The pool's tasks are lifo scheduled task_func functors.
     */
    typedef thread_pool<general_task, lifo_task_scheduler, wait_for_all_tasks> lifo_pool;


    /**
     * @brief Pool for prioritized task.
     *
     * The pool's tasks are prioritized prio_task_func functors.
     */
    typedef thread_pool<priority_task, priority_task_scheduler, wait_for_all_tasks> priority_pool;


}
}

#endif //CPP_THREADPOOL_HPP
