/*
 * pool_task.h
 *
 * Created on: 2023/4/13 17:11.
 *     Author: hou-lei
 */
#ifndef CHAT_DEMO_POOL_TASK_HPP
#define CHAT_DEMO_POOL_TASK_HPP

#include <functional>

namespace chat {
namespace threadpool {

    typedef std::function<void(void)> pool_task;

    class priority_task {
    private:
        pool_task m_function;
        unsigned int m_priority;
    public:

        priority_task(pool_task const & function, int const priority)
                : m_function(function), m_priority(priority) {

        }

        void operator() (void) const{
            if(m_function != nullptr) {
                m_function();
            }
        }

        bool operator< (const priority_task& rhs) const {
            return m_priority < rhs.m_priority;
        }

        bool operator== (const priority_task& rhs) const {
            return m_priority == rhs.m_priority;
        }

    };

}
}


#endif //CHAT_DEMO_POOL_TASK_HPP
