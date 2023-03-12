//
// Created by Administrator on 2023/3/11.
//

#include "BlockingQueue.h"

namespace chat{
namespace util {

template<typename E>
BlockingQueue<E>::BlockingQueue(int capacity)
        :capacity(capacity) {

}

template<typename E>
BlockingQueue<E>::~BlockingQueue() {
    this->list.clear();
}

template<typename E>
int BlockingQueue<E>::size() const {
    std::lock_guard<std::mutex> lockGuard(this->mutex);
    return list.size();
}

template<typename E>
bool BlockingQueue<E>::empty() const {
    std::lock_guard<std::mutex> lockGuard(this->mutex);
    return list.size() <= 0;
}

template<typename E>
bool BlockingQueue<E>::full() const {
    std::lock_guard<std::mutex> lockGuard(this->mutex);
    return list.size() >= capacity;
}

template<typename E>
void BlockingQueue<E>::put(E &e) {
    std::unique_lock<std::mutex> uniqueLock(this->mutex);
    while(list.size() >= capacity) {
        cond_full.wait(uniqueLock);
    }
    list.push_back(e);
    cond_empty.notify_one();
}

template<typename E>
E &BlockingQueue<E>::take() {
    std::unique_lock<std::mutex> uniqueLock(this->mutex);
    while (list.size() <= 0) {
        cond_empty.wait(uniqueLock);
    }
    E& e = list.front();
    list.pop_front();
    cond_full.notify_one();
    return e;
}


}
}