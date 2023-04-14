/*
 * pool_size_policies.hpp
 *
 * Created on: 2023/4/12 15:08.
 *     Author: hou-lei
 */
#ifndef CHAT_DEMO_POOL_SIZE_POLICIES_HPP
#define CHAT_DEMO_POOL_SIZE_POLICIES_HPP

#include <functional>

namespace chat {
namespace threadpool {

    template<typename Pool>
    class static_size {
    private:
        std::reference_wrapper<Pool volatile> m_pool;
    public:
        static_size(Pool volatile & pool) : m_pool(pool) {

        }
        bool resize(size_t const worker_count) {
            return m_pool.get().resize(worker_count);
        }
    };

}
}


#endif //CHAT_DEMO_POOL_SIZE_POLICIES_HPP
