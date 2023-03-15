/*
 * Poller.h
 *
 *  Created on: 2023年3月9日
 *      Author: hou-lei
 */
#ifndef CPP_POLLER_H
#define CPP_POLLER_H

#include "net/poll/PollerEventsHandler.h"
#include <memory>
#include <thread>
#include <mutex>
#include <map>
#include <functional>


namespace chat {
namespace net {
namespace poll {


class Poller final {
public:
//    typedef std::function<void(const int, const short, const short)> PollEventsHandler;
//    typedef std::function<void(const int)> CErrorHandler;
    // void PollEventsHandler(const int fd, const short events, const short revents)
    using PollEventsHandler = std::function<void(const int, const short, const short)>;
    using CErrorHandler = std::function<void(const int)>;

    Poller();
    ~Poller();
    void startup();
    void shutdown();
    void join();

    void registEventsHandler(int fd, int events, PollEventsHandler handler);
    void unregistEventsHandler(int fd);
    int getEventsHandlerSize();
    void setCErrorHandler(CErrorHandler errorHandler);

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
    CErrorHandler errorHandler;

};


} /* namespace poll */
} /* namespace net */
} /* namespace chat */

#endif //CPP_POLLER_H
