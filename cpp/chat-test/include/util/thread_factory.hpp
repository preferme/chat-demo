/*
 * thread_factory.h
 *
 * Created on: 2023/4/13 13:50.
 *     Author: hou-lei
 */
#ifndef CHAT_DEMO_THREAD_FACTORY_H
#define CHAT_DEMO_THREAD_FACTORY_H

#include <memory>
#include <thread>
#include <functional>

namespace chat {
namespace util {

    class thread_factory : public std::enable_shared_from_this<thread_factory> {
        virtual std::shared_ptr<std::thread> create(std::function<void(void)>) = 0;
    };


    class default_thread_facotry : public thread_factory {
    public:
        std::shared_ptr<std::thread> create(std::function<void(void)> func) {
            return std::make_shared<std::thread>(func);
        }
    };

}
}



#endif //CHAT_DEMO_THREAD_FACTORY_H
