//
// Created by NAVER on 2023/3/9.
//

#ifndef CPP_POLLER_H
#define CPP_POLLER_H

#include "ErrorHandler.h"
#include "PollerEventsHandler.h"
#include <memory>
#include <thread>
#include <mutex>
#include <map>


class Poller final {
public:
    Poller();
    ~Poller();
    void startup();
    void shutdown();
    void join();

    void regestEventsHandler(int fd, int events, PollEventsHandler handler);
    void setErrorHandler(ErrorHandler* errorHandler);

protected:
    void setRunnable(bool runnable);
    bool getRunnable();
    void onPollError(const int en);
    void onPollEvents(int fd, short events, short revents);
private:
    static void executeCyclePoll(Poller* self) noexcept;
    std::unique_ptr<std::thread> pollerThread;
    bool runnable;
    std::mutex mutex;
    std::map<int, std::shared_ptr<PollerEventsHandler>> eventsDispatchers;
    ErrorHandler* errorHandler;




};


#endif //CPP_POLLER_H
