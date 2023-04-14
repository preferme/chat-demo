/*
 * pool_size_policies.hpp
 *
 * Created on: 2023/4/14 14:34.
 *     Author: hou-lei
 */
#ifndef CHAT_DEMO_POOL_SIZE_POLICIES_HPP
#define CHAT_DEMO_POOL_SIZE_POLICIES_HPP


namespace chat {
namespace threadpool {

    template<typename Pool>
    class pool_size_policy {
    public:
        virtual void init(Pool& pool, int init_size = 0);
    };


    template <typename Pool>
    class static_size {
    public:
        void init(Pool& pool, int init_size = 0) {
            pool.resize_worker_threads(init_size);
        }
    };

}
}

#endif //CHAT_DEMO_POOL_SIZE_POLICIES_HPP
