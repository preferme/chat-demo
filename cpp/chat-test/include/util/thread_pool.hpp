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

namespace chat {
namespace util {

    class thread_pool {
    public:
        typedef std::function<void(void)> task_type;
    private:
        int m_core_size;
        volatile bool m_terminate;
        mutable std::queue<task_type> m_task_queue;
        mutable std::vector<shared_ptr<std::thread>> m_threads;
        static const task_type m_poison;
    private:
        std::mutex m_monitor;
        std::condition_variable m_task_come;
    public:
        thread_pool(int core_size = 0)
                : m_core_size(core_size)
                , m_task_queue()
                , m_threads()
                , m_terminate(false) {
            create_threads(core_size);
        }
        ~thread_pool() {
//            auto_shutdown(true);
        }

        bool execute(task_type task) {
            if (m_terminate) {
                return false;
            }
            std::unique_lock<std::mutex> lock(m_monitor);
            m_task_queue.push(task);
            m_task_come.notify_one();
            return true;
        }

        void shutdown() {
            auto_shutdown(true);
        }
    private:
        void thread_execute() {
            try {
                while (!m_terminate) {
                    task_type task = nullptr;
                    {
                        std::unique_lock<std::mutex> lock(m_monitor);
                        if (m_task_queue.empty()) {
                            std::cout<< "[thread_execute] wait. " << m_terminate << std::endl;
                            m_task_come.wait(lock);
                            std::cout<< "[thread_execute] wake up. " << m_terminate << std::endl;
                        } else {
                            task = m_task_queue.front();
                            m_task_queue.pop();
                            std::cout<< "[thread_execute] pop. " << m_task_queue.size() << std::endl;
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
            std::unique_lock<std::mutex> lock(m_monitor);
            std::cout<< "[thread_exit] " << m_threads.size() << " " << m_terminate << std::endl;
            m_threads.erase(std::remove_if(m_threads.begin(), m_threads.end(), [](std::shared_ptr<std::thread> worker){
                std::cout<< "[thread_exit] remove_if " << worker->get_id() << " " << std::this_thread::get_id() << std::endl;
                return worker->get_id() == std::this_thread::get_id();
            }), m_threads.end());
        }

        void thread_exception() {
            {
                std::unique_lock<std::mutex> lock(m_monitor);
                m_threads.erase(
                        std::remove_if(m_threads.begin(), m_threads.end(), [](std::shared_ptr<std::thread> worker) {
                            return worker->get_id() == std::this_thread::get_id();
                        }), m_threads.end());
            }
            create_threads(1);
        }

        void create_threads(int threads) {
            std::unique_lock<std::mutex> lock(m_monitor);
            for (int i = 0; i < threads; ++i) {
                m_threads.push_back(std::make_shared<std::thread>(std::bind(&thread_pool::thread_execute, this)));
            }
        }

        void auto_shutdown(bool wait_all) {
            std::cout<< "[auto_shutdown] 0." << std::endl;
            m_terminate = true;
            int size = m_threads.size();
            for (int i = 0; i < size; ++i) {
                execute(nullptr);
            }
//            std::unique_lock<std::mutex> lock(m_monitor);
//            std::cout<< "[auto_shutdown] 0.1  " << m_threads.size() << std::endl;
//            for (std::vector<shared_ptr<std::thread>>::iterator iter = m_threads.begin(); iter != m_threads.end(); iter++) {
////                    (*iter)->join();
//                std::cout<< "[auto_shutdown] 0.2" << std::endl;
//                m_task_queue.push(nullptr);
//                m_task_come.notify_one();
//            }
//            m_task_come.notify_all();

            std::cout<< "[auto_shutdown] 1." << std::endl;
            if (wait_all) {
                std::cout<< "[auto_shutdown] 2." << std::endl;
//                for (std::vector<shared_ptr<std::thread>>::iterator iter = m_threads.begin(); iter != m_threads.end(); iter++) {
//                    (*iter)->join();
//                    std::cout<< "[auto_shutdown] 2.1" << std::endl;
//                }
                while (!m_threads.empty()) {
                    m_task_come.notify_one();
                    std::cout<< "[auto_shutdown] 2.1" << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            }
            std::cout<< "[auto_shutdown] 3." << std::endl;
            m_threads.clear();
        }

    };

    const std::function<void(void)> thread_pool::m_poison([](){});

}
}

#endif //CHAT_DEMO_THREAD_POOL_HPP
