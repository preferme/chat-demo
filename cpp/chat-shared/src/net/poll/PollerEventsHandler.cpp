//
// Created by NAVER on 2023/3/9.
//

#include "net/poll/PollerEventsHandler.h"
#include <poll.h>

namespace chat {
namespace net {
namespace poll {


PollerEventsHandler::PollerEventsHandler(const int fd) : fd(fd), events(0)
            , onPollIn(nullptr), onPollPri(nullptr), onPollOut(nullptr), onPollErr(nullptr), onPollHup(nullptr), onPollNval(nullptr)
#if POLLRDNORM != POLLIN
            , onPollPri(nullptr), onPollRdnorm(nullptr), onPollWrnorm(nullptr), onPollRdband(nullptr), onPollWrban(nullptr)
#endif
#if defined(POLLEXTEND)
            , onPollExtend(nullptr), onPollAttrib(nullptr), onPollNlink(nullptr), onPollWrite(nullptr)
#endif
{

}

void PollerEventsHandler::registEventsHandler(const short events, const PollEventsHandler eventsHandler) {
    this->events |= events;

    if (events & POLLIN) {
        this->onPollIn = std::move(eventsHandler);
    }
    if (events & POLLOUT) {
        this->onPollOut = std::move(eventsHandler);
    }
    if (events & POLLERR) {
        this->onPollErr = std::move(eventsHandler);
    }
    if (events & POLLHUP) {
        this->onPollHup = std::move(eventsHandler);
    }
    if (events & POLLNVAL) {
        this->onPollNval = std::move(eventsHandler);
    }
#if POLLRDNORM != POLLIN
    if (events & POLLPRI) {
        this->onPollPri = std::move(eventsHandler);
    }
    if (events & POLLRDNORM) {
        this->onPollRdnorm = std::move(eventsHandler);
    }
    if (events & POLLWRNORM) {
        this->onPollWrnorm = std::move(eventsHandler);
    }
    if (events & POLLRDBAND) {
        this->onPollRdband = std::move(eventsHandler);
    }
    if (events & POLLWRBAND) {
        this->onPollWrban = std::move(eventsHandler);
    }
#endif
#if defined(POLLEXTEND)
    if (events & POLLEXTEND) {
        this->onPollExtend = std::move(eventsHandler);
    }
    if (events & POLLATTRIB) {
        this->onPollAttrib = std::move(eventsHandler);
    }
    if (events & POLLNLINK) {
        this->onPollNlink = std::move(eventsHandler);
    }
    if (events & POLLWRITE) {
        this->onPollWrite = std::move(eventsHandler);
    }
#endif
}

void PollerEventsHandler::executeEventsHandler(const int fd, const short events, const short revents) {
    if (this->onPollIn.operator bool() && (revents & POLLIN)) {
        this->onPollIn(fd, events, revents);
    }
    if (this->onPollOut.operator bool() && (revents & POLLOUT)) {
        this->onPollOut(fd, events, revents);
    }
    if (this->onPollPri.operator bool() && (revents & POLLPRI)) {
        this->onPollPri(fd, events, revents);
    }
    if (this->onPollErr.operator bool() && (revents & POLLERR)) {
        this->onPollErr(fd, events, revents);
    }
    if (this->onPollHup.operator bool() && (revents & POLLHUP)) {
        this->onPollHup(fd, events, revents);
    }
    if (this->onPollNval.operator bool() && (revents & POLLNVAL)) {
        this->onPollNval(fd, events, revents);
    }
#if POLLRDNORM != POLLIN
    if (this->onPollRdnorm.operator bool() && (revents & POLLRDNORM)) {
        this->onPollRdnorm(fd, events, revents);
    }
    if (this->onPollWrnorm.operator bool() && (revents & POLLWRNORM)) {
        this->onPollWrnorm(fd, events, revents);
    }
    if (this->onPollRdband.operator bool() && (revents & POLLRDBAND)) {
        this->onPollRdband(fd, events, revents);
    }
    if (this->onPollWrban.operator bool() && (revents & POLLWRBAND)) {
        this->onPollWrban(fd, events, revents);
    }
#endif
#if defined(POLLEXTEND)
    if (this->onPollExtend.operator bool() && (revents & POLLEXTEND)) {
        this->onPollExtend(fd, events, revents);
    }
    if (this->onPollAttrib.operator bool() && (revents & POLLATTRIB)) {
        this->onPollAttrib(fd, events, revents);
    }
    if (this->onPollNlink.operator bool() && (revents & POLLNLINK)) {
        this->onPollNlink(fd, events, revents);
    }
    if (this->onPollWrite.operator bool() && (revents & POLLWRITE)) {
        this->onPollWrite(fd, events, revents);
    }
#endif
}

short PollerEventsHandler::getEvents() {
    return this->events;
}



} /* namespace poll */
} /* namespace net */
} /* namespace chat */