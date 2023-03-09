//
// Created by NAVER on 2023/3/9.
//

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
    typedef std::function<void(const int, const short, const short)> PollEventsHandler;
    typedef std::function<void(const int)> ErrorHandler;

    Poller();
    ~Poller();
    void startup();
    void shutdown();
    void join();

    void registEventsHandler(int fd, int events, PollEventsHandler handler);
    void registErrorHandler(ErrorHandler errorHandler);

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
    ErrorHandler errorHandler;

};


} /* namespace poll */
} /* namespace net */
} /* namespace chat */

#endif //CPP_POLLER_H
