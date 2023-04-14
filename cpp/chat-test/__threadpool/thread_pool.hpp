/*
 * thread_pool.h
 *
 * Created on: 2023/4/13 19:09.
 *     Author: hou-lei
 */
#ifndef CHAT_DEMO_THREAD_POOL_HPP
#define CHAT_DEMO_THREAD_POOL_HPP

#include "pool_core.hpp"
#include "auto_shutdown_policies.hpp"
#include "pool_size_policies.hpp"

#include <iostream>

namespace chat {
namespace threadpool {


    template <
            template <typename> class SizePolicy            = static_size,
            template <typename> class AutoShutdownPolicy   = wait_for_all_tasks
    >
    class thread_pool {
    public:
        typedef pool_core<SizePolicy, AutoShutdownPolicy> pool_core_type;
        typedef SizePolicy<pool_core_type> size_policy_type;
        typedef AutoShutdownPolicy<pool_core_type> shutdown_policy_type;
    private:
        std::shared_ptr<pool_core_type> m_core;
        std::shared_ptr<size_policy_type> m_size_policy;
        std::shared_ptr<shutdown_policy_type> m_shutdown_policy;
    public:
        thread_pool(int core_size = 0, std::shared_ptr<pool_task_queue> task_queue = std::make_shared<fifo_task_queue>())
                : m_core(std::make_shared<pool_core_type>(core_size, task_queue))
                , m_size_policy(std::make_shared<size_policy_type>())
                , m_shutdown_policy(std::make_shared<shutdown_policy_type>()) {

            m_size_policy->init(*m_core, core_size);
        }

        virtual ~thread_pool() {
//            m_shutdown_policy->auto_shutdown(*m_core);
        }

        bool schedule(pool_task task) {
            return m_core->schedule(task);
        }

        void wait(int const task_threshold = 0) const {
            m_core->wait(task_threshold);
        }

        void shutdown() {
            std::cout<< "[thread_pool] shutdown" << std::endl;
            m_shutdown_policy->auto_shutdown(*m_core);
        }

        void wakeup() {
            std::cout<< "[thread_pool] wakeup" << std::endl;
            m_core->wakeup();
        }
    };


}
}




#endif //CHAT_DEMO_THREAD_POOL_HPP
