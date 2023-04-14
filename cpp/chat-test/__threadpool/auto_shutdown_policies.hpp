/*
 * auto_shutdown_policies.h
 *
 * Created on: 2023/4/14 11:58.
 *     Author: hou-lei
 */
#ifndef CHAT_DEMO_AUTO_SHUTDOWN_POLICIES_HPP
#define CHAT_DEMO_AUTO_SHUTDOWN_POLICIES_HPP

#include <iostream>

namespace chat {
namespace threadpool {

    template<typename Pool>
    class auto_shutdown_policy {
    public:
        virtual void auto_shutdown(Pool& pool) = 0;
    };

    template<typename Pool>
    class wait_for_all_tasks : public auto_shutdown_policy<Pool> {
    public:
        void auto_shutdown(Pool& pool) override {
            std::cout << "[wait_for_all_tasks][auto_shutdown] begin" << std::endl;
            pool.wait();
            pool.terminate_all_workers(true);
            std::cout << "[wait_for_all_tasks][auto_shutdown] end" << std::endl;
        }
    };

    template<typename Pool>
    class wait_for_active_tasks : public auto_shutdown_policy<Pool> {
    public:
        void auto_shutdown(Pool& pool) override {
            pool.clean_tasks();
            pool.wait();
            pool.terminate_all_workers(true);
        }
    };

    template<typename Pool>
    class immediately : public auto_shutdown_policy<Pool> {
    public:
        void auto_shutdown(Pool& pool) override {
            pool.clean_tasks();
            pool.terminate_all_workers(false);
        }
    };

}
}



#endif //CHAT_DEMO_AUTO_SHUTDOWN_POLICIES_HPP
