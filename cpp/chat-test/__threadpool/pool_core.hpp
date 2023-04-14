/*
 * pool_core.h
 *
 * Created on: 2023/4/13 14:18.
 *     Author: hou-lei
 */
#ifndef CHAT_DEMO_POOL_CORE_HPP
#define CHAT_DEMO_POOL_CORE_HPP

#include <memory>
#include <functional>
#include <vector>
#include "util/non_copyable.hpp"
#include "worker_thread.hpp"
#include "pool_task.hpp"
#include "pool_task_queue.hpp"


namespace chat {
namespace threadpool {



    template <
            template <typename> class SizePolicy,
            template <typename> class AutoShutdownPolicy
    >
    class pool_core : public std::enable_shared_from_this< pool_core<SizePolicy, AutoShutdownPolicy> >, private chat::util::non_copyable {
    public:
        typedef pool_core<SizePolicy, AutoShutdownPolicy> pool_type;
        typedef SizePolicy<pool_type> size_policy_type;
        typedef worker_thread<pool_type> worker_thread_type;
    private:
        friend class worker_thread<pool_type>;
        friend class AutoShutdownPolicy<pool_type>;
        friend class SizePolicy<pool_type>;

    private:
        const int m_core_size;
        volatile int m_worker_count;
        volatile int m_target_worker_count;
        volatile int m_active_worker_count;

    private:
        std::shared_ptr<pool_task_queue> m_task_queue;
        bool  m_terminate_all_workers;								// Indicates if termination of all workers was triggered.
        std::vector<std::shared_ptr<worker_thread_type>> m_workers;

    private:
        mutable std::mutex m_monitor;
        mutable std::condition_variable m_worker_idle_or_terminated;	// A worker is idle or was terminated.
        mutable std::condition_variable m_task_or_terminate_workers;  // Task is available OR total worker count should be reduced.

    public:
        pool_core(int core_size = 0,
                  std::shared_ptr<pool_task_queue> task_queue = std::make_shared<fifo_task_queue>())
                : m_core_size(core_size)
                , m_task_queue(task_queue)
                , m_worker_count(0)
                , m_active_worker_count(0)
                , m_target_worker_count(core_size)
                , m_terminate_all_workers(false) {

        }

        bool schedule(pool_task& task) {
            std::cout << "schedule task" << std::endl;
            std::unique_lock<std::mutex> lock(m_monitor);
            if (m_task_queue->push(task)) {
                m_task_or_terminate_workers.notify_one();
                return true;
            }
            return false;
        }

        void wait(int const task_threshold = 0) const {
            std::cout << "wait " << task_threshold << std::endl;
            std::unique_lock<std::mutex> lock(m_monitor);
            while (task_threshold < m_active_worker_count + m_task_queue->size()) {
                m_worker_idle_or_terminated.wait(lock);
            }
            std::cout << "wait end " << task_threshold << std::endl;
        }

        void clean_tasks(std::shared_ptr<std::vector<pool_task>> remains = nullptr) {
            std::unique_lock<std::mutex> lock(m_monitor);
            if (remains == nullptr) {
                m_task_queue->clear();
            } else {
                while(!m_task_queue->empty()) {
                    remains->push_back(m_task_queue->pop());
                }
            }
        }

        void wakeup() {
            std::cout << "[pool_core][wakeup] " << std::endl;
            m_task_or_terminate_workers.notify_all();
        }

    private:
        bool resize_worker_threads(int const woker_count) {
            std::cout << "[pool_core][resize_worker_threads] " << woker_count << std::endl;
            std::unique_lock<std::mutex> lock(m_monitor);
            if (m_terminate_all_workers) {
                return false;
            }
            m_target_worker_count = woker_count;

            if (m_worker_count <= m_target_worker_count) {
                while (m_worker_count < m_target_worker_count) {
                    try {
                        std::shared_ptr<worker_thread_type> worker = std::make_shared<worker_thread_type>(this->shared_from_this());
                        m_workers.push_back(worker);
                        m_worker_count++;
                        m_active_worker_count++;
                    } catch (...) {
                        return false;
                    }
                }
            } else {
                m_task_or_terminate_workers.notify_all();
            }
            return true;
        }

        void terminate_all_workers(bool const wait) {
            std::cout << "terminate_all_workers" << std::endl;
            std::unique_lock<std::mutex> lock(m_monitor);
            m_terminate_all_workers = true;
            m_target_worker_count = 0;
            m_task_or_terminate_workers.notify_all();
            if (wait) {
                std::cout << "terminate_all_workers wait." << std::endl;
                while (m_active_worker_count > 0) {
                    m_worker_idle_or_terminated.wait(lock);
                }
                std::cout << "terminate_all_workers join threads" << std::endl;
                for (typename std::vector<shared_ptr<worker_thread_type>>::iterator iter = m_workers.begin();
                        iter != m_workers.end();
                        ++iter) {
                    (*iter)->join();
                }
                m_workers.clear();
            }
        }

        bool fetch_task(pool_task& task) {
            task = nullptr;
            std::unique_lock<std::mutex> lock(m_monitor);

            // decrease number of threads if necessary
            if (m_worker_count > m_target_worker_count) {
                std::cout << "worker_execute_task decrease thread" << std::endl;
                return false;
            }

            while (m_task_queue->empty()) {
                std::cout << "[pool_core][worker_execute_task] m_worker_count= " << m_worker_count << ", m_target_worker_count=" << m_target_worker_count << std::endl;
                // decrease number of threads if necessary
                if (m_worker_count > m_target_worker_count) {
                    std::cout << "worker_execute_task decrease threads" << std::endl;
                    return false;
                } else {
                    m_active_worker_count--;
                    m_worker_idle_or_terminated.notify_all();
                    std::cout << "[pool_core][worker_execute_task] m_task_queue is empty wait begin" << std::endl;
                    m_task_or_terminate_workers.wait(lock);
                    std::cout << "[pool_core][worker_execute_task] m_task_queue is empty wait finished" << std::endl;
                    m_active_worker_count++;
                    std::cout << "[pool_core][worker_execute_task] m_active_worker_count++ " << std::endl;
                }
            }

            task = m_task_queue->pop();
            return true;
        }

        // called by worker_thread
        bool worker_execute_task(std::shared_ptr<worker_thread_type> worker) {
            std::cout << "[pool_core][worker_execute_task] "<< worker << std::endl;
            pool_task  task;
            if (fetch_task(task) == false) {
                return false;
            }

            if (task != nullptr) {
                std::cout << "execute task" << std::endl;
                task();
            }
            std::cout << "worker_execute_task end" << std::endl;
            return true;
        }

        void worker_died_unexpectedly(std::shared_ptr<worker_thread_type> worker) {
            std::cout << "worker_died_unexpectedly " << worker << std::endl;
            m_workers.erase(std::remove(m_workers.begin(), m_workers.end(), worker), m_workers.end());
        }

        void worker_destructed(std::shared_ptr<worker_thread_type> worker) {
            std::cout << "worker_destructed" << std::endl;
            m_workers.erase(std::remove(m_workers.begin(), m_workers.end(), worker), m_workers.end());
        }

    };




}
}

#endif //CHAT_DEMO_POOL_CORE_HPP
