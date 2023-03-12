//
// Created by Administrator on 2023/3/11.
//

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
    explicit BlockingQueue(int capacity);
    ~BlockingQueue();
    BlockingQueue(const BlockingQueue<E>& other) = delete;
    BlockingQueue<E>& operator=(const BlockingQueue<E>& other) = delete;

    int size() const;
    bool empty() const;
    bool full() const;

    void put(E& e);
    E& take();


private:
    int capacity;
    std::list<E> list;
    std::lock_guard<std::mutex> mutex;
    std::condition_variable cond_full;
    std::condition_variable cond_empty;

};


}
}


#endif //CPP_BLOCKINGQUEUE_H
