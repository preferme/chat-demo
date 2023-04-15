/*
 * thread_pool.hpp
 *
 * Created on: 2023/4/14 17:20.
 *     Author: hou-lei
 */
#ifndef CHAT_DEMO_THREAD_POOL_HPP
#define CHAT_DEMO_THREAD_POOL_HPP

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <memory>
#include <vector>
#include <algorithm>

namespace chat {
namespace util {

    class thread_pool {
    public:
        typedef std::function<void(void)> task_type;
    private:
        volatile bool m_terminate;
        mutable std::queue<task_type> m_task_queue;
        mutable std::vector<shared_ptr<std::thread>> m_threads;
    private:
        std::mutex m_monitor;
        std::condition_variable m_task_submit_or_terminate;
    public:
        thread_pool(int core_size = 0)
                : m_task_queue()
                , m_threads()
                , m_terminate(false) {
            create_threads(core_size);
        }
        ~thread_pool() {
            auto_shutdown(true);
        }

        bool execute(task_type task) {
            if (m_terminate) {
                return false;
            }
            std::unique_lock<std::mutex> lock(m_monitor);
            m_task_queue.push(task);
            m_task_submit_or_terminate.notify_one();
            return true;
        }

    private:
        void thread_execute() {
            try {
                while (!m_terminate) {
                    task_type task = nullptr;
                    {
                        std::unique_lock<std::mutex> lock(m_monitor);
                        if (m_task_queue.empty()) {
                            m_task_submit_or_terminate.wait(lock);
                        } else {
                            task = m_task_queue.front();
                            m_task_queue.pop();
                        }
                    }
                    if (task != nullptr) {
                        task();
                    }
                }
                thread_exit();
            } catch (...) {
                thread_exception();
            }
        }

        void thread_exit() {
            if (!m_terminate) {
                std::unique_lock<std::mutex> lock(m_monitor);
                std::vector<shared_ptr<std::thread>>::iterator iter = std::find_if(m_threads.begin(), m_threads.end(), [](std::shared_ptr<std::thread> const& worker){
                    return worker->get_id() == std::this_thread::get_id();
                });
                std::swap(*iter, m_threads.back());
                m_threads.back()->detach();
                m_threads.pop_back();
            }
        }

        void thread_exception() {
            if (!m_terminate) {
                std::unique_lock<std::mutex> lock(m_monitor);
                std::vector<shared_ptr<std::thread>>::iterator iter = std::find_if(m_threads.begin(), m_threads.end(), [](std::shared_ptr<std::thread> const& worker){
                    return worker->get_id() == std::this_thread::get_id();
                });
                std::swap(*iter, m_threads.back());
                m_threads.back()->detach();
                m_threads.pop_back();
                m_threads.push_back(std::make_shared<std::thread>(std::bind(&thread_pool::thread_execute, this)));
            }
        }

        void create_threads(int threads) {
            std::unique_lock<std::mutex> lock(m_monitor);
            for (int i = 0; i < threads; ++i) {
                m_threads.push_back(std::make_shared<std::thread>(std::bind(&thread_pool::thread_execute, this)));
            }
        }

        void auto_shutdown(bool wait_all) {
            m_terminate = true;
            m_task_submit_or_terminate.notify_all();

            if (wait_all) {
                for (std::vector<shared_ptr<std::thread>>::iterator iter = m_threads.begin(); iter != m_threads.end(); iter++) {
                    (*iter)->join();
                }
                m_threads.clear();
            }
        }

    };


}
}

#endif //CHAT_DEMO_THREAD_POOL_HPP
