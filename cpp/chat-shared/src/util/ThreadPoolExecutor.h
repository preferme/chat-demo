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

namespace chat{
namespace util {


class ThreadPoolExecutor {
public:
    // void run();
    using Runnable = std::function<void()>;
    // void rejectedExecution(Runnable r, ThreadPoolExecutor executor);
    using RejectedExecutionHandler = std::function<void(Runnable&, ThreadPoolExecutor&)>;

    template <class Rep, class Period>
    ThreadPoolExecutor(const int corePoolSize, const int maximumPoolSize,
                       const std::chrono::duration<Rep, Period> keepAliveTime,
                       const std::shared_ptr<BlockingQueue<Runnable>> workQueue, const RejectedExecutionHandler handler);
    ~ThreadPoolExecutor();

};


}
}

#endif //CPP_THREADPOOLEXECUTOR_H
