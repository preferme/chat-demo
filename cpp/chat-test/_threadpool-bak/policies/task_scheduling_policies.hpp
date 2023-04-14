/*
 * task_scheduling_policies.hpp
 *
 * Created on: 2023/4/10 15:48.
 *     Author: hou-lei
 */
#ifndef CPP_TASK_SCHEDULING_POLICIES_HPP
#define CPP_TASK_SCHEDULING_POLICIES_HPP

#include <deque>
#include <queue>
#include "threadpool/adaptors/task_adaptors.hpp"

namespace chat {
namespace threadpool {

    template<typename Task>
    class task_scheduling_policy {
    public:
        typedef Task task_type;
    public:
        virtual bool push(task_type const & task) = 0;
        virtual task_type const & pop() const = 0;
        virtual size_t size() const = 0;
        virtual bool empty() const = 0;
        virtual void clear() = 0;
    };


    template<typename Task = general_task>
    class fifo_task_scheduler : public task_scheduling_policy<Task> {
    public:
        typedef Task task_type;
    protected:
        std::deque<task_type> m_container;
    public:
        bool push(task_type const &task) override {
            m_container.push_back(task);
            return true;
        }

        task_type const &pop() const override {
            std::deque<task_type>& container = const_cast<std::deque<task_type>&>(m_container);
            const task_type &task = container.front();
            container.pop_front();
            return task;
        }

        size_t size() const override {
            return m_container.size();
        }

        bool empty() const override {
            return m_container.empty();
        }

        void clear() override {
            m_container.clear();
        }
    };



    template<typename Task = general_task>
    class lifo_task_scheduler : public task_scheduling_policy<Task> {
    public:
        typedef Task task_type;
    protected:
        std::deque<task_type> m_container;
    public:
        bool push(task_type const &task) override {
            m_container.push_front(task);
            return true;
        }

        task_type const & pop() const override {
            task_type const & task = m_container.front();
            m_container.pop_front();
            return task;
        }

        size_t size() const override {
            return m_container.size();
        }

        bool empty() const override {
            return m_container.empty();
        }

        void clear() override {
            m_container.clear();
        }
    };


    template<typename Task = priority_task>
    class priority_task_scheduler : public task_scheduling_policy<Task> {
    public:
        typedef Task task_type;
    protected:
        std::priority_queue<task_type> m_container;
    public:
        bool push(task_type const &task) override {
            m_container.push(task);
            return true;
        }

        task_type const & pop() const override {
            task_type const & task = m_container.top();
            m_container.pop();
            return task;
        }

        size_t size() const override {
            return m_container.size();
        }

        bool empty() const override {
            return m_container.empty();
        }

        void clear() override {
            while (!m_container.empty()) {
                m_container.pop();
            }
        }
    };

}
}

#endif //CPP_TASK_SCHEDULING_POLICIES_HPP
