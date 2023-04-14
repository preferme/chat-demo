/*
 * auto_shutdown_policies.hpp
 *
 * Created on: 2023/4/11 15:01.
 *     Author: hou-lei
 */
#ifndef CHAT_DEMO_AUTO_SHUTDOWN_POLICIES_HPP
#define CHAT_DEMO_AUTO_SHUTDOWN_POLICIES_HPP


namespace chat {
namespace threadpool {


    template<typename Pool>
    class auto_shutdown_policies {
    public:
        virtual void shutdown(Pool& pool) = 0;
    };


    template<typename Pool>
    class wait_for_all_tasks : public auto_shutdown_policies<Pool> {
    public:
        void shutdown(Pool& pool) {
            pool.wait();
            pool.terminate_all_workers(true);
        }
    };


    template<typename Pool>
    class wait_for_active_tasks : public auto_shutdown_policies<Pool> {
    public:
        void shutdown(Pool& pool) {
            pool.clear();
            pool.wait();
            pool.terminate_all_workers(true);
        }
    };


    template<typename Pool>
    class immediately : public auto_shutdown_policies<Pool> {
    public:
        void shutdown(Pool& pool) {
            pool.clear();
            pool.terminate_all_workers(false);
        }
    };

}
}


#endif //CHAT_DEMO_AUTO_SHUTDOWN_POLICIES_HPP
