/*
 * pool_task_queue.h
 *
 * Created on: 2023/4/13 17:19.
 *     Author: hou-lei
 */
#ifndef CHAT_DEMO_POOL_TASK_QUEUE_HPP
#define CHAT_DEMO_POOL_TASK_QUEUE_HPP

#include <deque>
#include <queue>

#include "pool_task.hpp"

namespace chat {
namespace threadpool {

    class pool_task_queue {
    public:
        virtual bool push(pool_task const & task) = 0;
        virtual pool_task const & pop() const = 0;
        virtual int size() const = 0;
        virtual bool empty() const = 0;
        virtual void clear() = 0;
    };


    class fifo_task_queue : public pool_task_queue {
    protected:
        std::deque<pool_task> m_container;
    public:
        bool push(pool_task const &task) override {
            m_container.push_back(task);
            return true;
        }

        pool_task const &pop() const override {
            std::deque<pool_task>& container = const_cast<std::deque<pool_task>&>(m_container);
            const pool_task &task = container.front();
            container.pop_front();
            return task;
        }

        int size() const override {
            return m_container.size();
        }

        bool empty() const override {
            return m_container.empty();
        }

        void clear() override {
            m_container.clear();
        }

    };
    
}
}




#endif //CHAT_DEMO_POOL_TASK_QUEUE_HPP
