/*
 * task_adaptors.hpp
 *
 * Created on: 2023/4/10 16:00.
 *     Author: hou-lei
 */
#ifndef CPP_TASK_ADAPTORS_HPP
#define CPP_TASK_ADAPTORS_HPP

#include <functional>
#include <chrono>
#include <thread>

namespace chat {
namespace threadpool {


    typedef std::function<void(void)> general_task;


    class priority_task {
    private:
        general_task m_function;
        unsigned int m_priority;
    public:
        priority_task(general_task const & function, unsigned int const priority)
                : m_function(function), m_priority(priority) {

        }

        void operator() (void) const {
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


    template<class Rep, class Period>
    class looped_task {
    private:
        std::function<bool(void)> m_function;
        std::chrono::duration<Rep, Period> m_duration;
    public:
        looped_task(std::function<bool(void)> const & function, std::chrono::duration<Rep, Period> const & duration)
                : m_function(function), m_duration(duration) {

        }

        looped_task(std::function<bool(void)> const & function)
                : m_function(function), m_duration(std::chrono::milliseconds(0)) {

        }

        void operator() (void) const {
            if (m_function != nullptr) {
                do {
                    std::this_thread::sleep_for(m_duration);
                } while (m_function());
            }
        }
    };

}
}

#endif //CPP_TASK_ADAPTORS_HPP
