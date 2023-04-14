/*
 * worker_thread.hpp
 *
 * Created on: 2023/4/13 14:51.
 *     Author: hou-lei
 */
#ifndef CHAT_DEMO_WORKER_THREAD_HPP
#define CHAT_DEMO_WORKER_THREAD_HPP

#include <memory>
#include "util/non_copyable.hpp"

#include <iostream>

namespace chat {
namespace threadpool {


    template<typename Pool>
    class worker_thread : public std::enable_shared_from_this<worker_thread<Pool>>, private chat::util::non_copyable {

    private:
        std::shared_ptr<Pool> m_pool;
        std::shared_ptr<std::thread> m_thread;

    public:
        worker_thread(std::shared_ptr<Pool> const& pool)
                : m_pool(pool)
                , m_thread(std::make_shared<std::thread>(std::bind(&worker_thread::run, this))) {
            std::cout<< "[worker_thread] create worker_thread " << m_thread->get_id() << std::endl;
        }

        void run() {
            std::cout<< "[worker_thread][run] begin " << m_thread->get_id() << std::endl;
            try {
                while(m_pool->worker_execute_task(this->shared_from_this()));

                m_pool->worker_destructed(this->shared_from_this());
            } catch (...) {
                m_pool->worker_died_unexpectedly(this->shared_from_this());
            }
            std::cout<< "[worker_thread][run] end " << m_thread->get_id() << std::endl;
        }

        void join() {
            m_thread->join();
        }

        friend iostream & operator<<(iostream& out, std::shared_ptr<worker_thread<Pool>> & worker) {
            return out << worker->m_thread->get_id();
        }
    };

}
}



#endif //CHAT_DEMO_WORKER_THREAD_HPP
