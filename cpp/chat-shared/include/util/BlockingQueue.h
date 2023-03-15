/*
 * BlockingQueue.h
 *
 *  Created on: 2023年3月11日
 *      Author: hou-lei
 */

#ifndef CPP_BLOCKINGQUEUE_H
#define CPP_BLOCKINGQUEUE_H

#include <list>
#include <mutex>
#include <condition_variable>

namespace chat{
namespace util {


template <typename E>
class BlockingQueue {
public:
    explicit BlockingQueue(int capacity): capacity(capacity) {
        ::printf("create BlockingQueue\n");
    }
    ~BlockingQueue() {
        this->list.clear();
        ::printf("release BlockingQueue\n");
    }

    BlockingQueue(const BlockingQueue<E>& other) = delete;
    BlockingQueue<E>& operator=(const BlockingQueue<E>& other) = delete;

    int size() const {
        std::lock_guard<std::mutex> lockGuard(this->mutex);
        return list.size();
    }
    bool empty() const {
        std::lock_guard<std::mutex> lockGuard(this->mutex);
        return list.size() <= 0;
    }
    bool full() const {
        std::lock_guard<std::mutex> lockGuard(this->mutex);
        return list.size() >= capacity;
    }

    void put(const E e) {
        std::unique_lock<std::mutex> uniqueLock(this->mutex);
        while(list.size() >= capacity) {
            cond_full.wait(uniqueLock);
        }
        list.push_back(e);
        cond_empty.notify_one();
    }
    E take() {
        std::unique_lock<std::mutex> uniqueLock(this->mutex);
        while (list.size() <= 0) {
            cond_empty.wait(uniqueLock);
        }
        E e = list.front();
        list.pop_front();
        cond_full.notify_one();
        return e;
    }


private:
    int capacity;
    std::list<E> list;
    std::mutex mutex;
    std::condition_variable cond_full;
    std::condition_variable cond_empty;

};


}
}


#endif //CPP_BLOCKINGQUEUE_H
