/*
 * ThreadPoolExecutor.h
 *
 *  Created on: 2023年3月13日
 *      Author: hou-lei
 */

#ifndef CPP_THREADPOOLEXECUTOR_H
#define CPP_THREADPOOLEXECUTOR_H

#include "util/BlockingQueue.h"
#include <chrono>
#include <functional>
#include <atomic>

namespace chat {
namespace util {

// void run();
using Runnable = std::function<void()>;
// void rejectedExecution(Runnable r, ThreadPoolExecutor executor);
class ThreadPoolExecutor;
using RejectedExecutionHandler = std::function<void(Runnable&, ThreadPoolExecutor&)>;

class ThreadPoolExecutor {
public:

    ThreadPoolExecutor(const int corePoolSize, const int maximumPoolSize,
                       const std::chrono::milliseconds keepAliveTime,
                       const std::shared_ptr<BlockingQueue<Runnable>> workQueue, const RejectedExecutionHandler handler)
                       :corePoolSize(corePoolSize), maxPoolSize(maximumPoolSize), keepAliveTime(keepAliveTime), workQueue(workQueue), handler(handler) {

    }
    ~ThreadPoolExecutor();

private:
    int corePoolSize;
    int maxPoolSize;
    volatile std::chrono::milliseconds keepAliveTime;
    std::shared_ptr<BlockingQueue<Runnable>> workQueue;
    RejectedExecutionHandler handler;
};


}
}

#endif //CPP_THREADPOOLEXECUTOR_H
