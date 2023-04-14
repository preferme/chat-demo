/*
 * thread_factory.hpp
 *
 * Created on: 2023/4/10 17:49.
 *     Author: hou-lei
 */
#ifndef CPP_THREAD_FACTORY_HPP
#define CPP_THREAD_FACTORY_HPP

#include <thread>

namespace chat {
namespace threadpool {


    class thread_factory {
    public:
        virtual std::thread* newThread() = 0;
    };




}
}


#endif //CPP_THREAD_FACTORY_HPP
