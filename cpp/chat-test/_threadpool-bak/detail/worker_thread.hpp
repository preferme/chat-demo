/*
 * worker_thread.hpp
 *
 * Created on: 2023/4/10 17:21.
 *     Author: hou-lei
 */
#ifndef CPP_WORKER_THREAD_HPP
#define CPP_WORKER_THREAD_HPP

#include <memory>
#include <thread>
#include <functional>

#include "util/non_copyable.hpp"
#include "util/scope_guard.hpp"


#include <iostream>

namespace chat {
namespace threadpool {

    template<typename Pool>
    class worker_thread
            : public std::enable_shared_from_this< worker_thread<Pool> >
            , private chat::util::non_copyable {
    public:
        typedef Pool pool_type;
    private:
        std::shared_ptr<pool_type>   m_pool;
        std::shared_ptr<std::thread> m_thread;
    protected:
        void on_exception() {
            m_pool->worker_died_unexpectedly(this->shared_from_this());
        }
        void on_finished() {
            m_pool->worker_destructed(this->shared_from_this());
        }
    public:
        worker_thread(std::shared_ptr<pool_type> const & pool)
                    : m_pool(pool) {
            std::cout << "worker_thread(pool)" << std::endl;
            this->m_thread = std::make_shared<std::thread>(std::bind(&worker_thread::run, this));
            std::cout << "worker_thread" << std::endl;
        }

        void run() {
            std::cout << "worker_thread::run()" << std::endl;
            try {
                while(m_pool->worker_execute_task(this->shared_from_this()));
                on_finished();
            } catch (...) {
                on_exception();
            }
        }

        void join() {
            m_thread->join();
        }

    };

}
}

#endif //CPP_WORKER_THREAD_HPP
