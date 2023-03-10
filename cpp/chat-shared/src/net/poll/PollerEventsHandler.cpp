//
// Created by NAVER on 2023/3/9.
//

#include "net/poll/PollerEventsHandler.h"
#include <poll.h>

namespace chat {
namespace net {
namespace poll {


BasePollerEventsHandler::BasePollerEventsHandler(const int fd)
        : fd(fd), events(0),
          onPollIn(nullptr), onPollPri(nullptr), onPollOut(nullptr), onPollRdnorm(nullptr), onPollWrnorm(nullptr),
          onPollRdband(nullptr), onPollWrban(nullptr),
          onPollErr(nullptr), onPollHup(nullptr), onPollNval(nullptr) {

}

void BasePollerEventsHandler::registEventsHandler(const short events, const PollEventsHandler eventsHandler) {
    this->events |= events;

    if (events & POLLIN) {
        this->onPollIn = std::move(eventsHandler);
    }
    if (events & POLLOUT) {
        this->onPollOut = std::move(eventsHandler);
    }
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
    if (events & POLLERR) {
        this->onPollErr = std::move(eventsHandler);
    }
    if (events & POLLHUP) {
        this->onPollHup = std::move(eventsHandler);
    }
    if (events & POLLNVAL) {
        this->onPollNval = std::move(eventsHandler);
    }
}

void BasePollerEventsHandler::executeEventsHandler(const int fd, const short events, const short revents) {
    if (this->onPollIn && (revents & POLLIN)) {
        this->onPollIn(fd, events, revents);
    }
    if (this->onPollOut && (revents & POLLOUT)) {
        this->onPollOut(fd, events, revents);
    }
    if (this->onPollPri && (revents & POLLPRI)) {
        this->onPollPri(fd, events, revents);
    }
    if (this->onPollRdnorm && (revents & POLLRDNORM)) {
        this->onPollRdnorm(fd, events, revents);
    }
    if (this->onPollWrnorm && (revents & POLLWRNORM)) {
        this->onPollWrnorm(fd, events, revents);
    }
    if (this->onPollRdband && (revents & POLLRDBAND)) {
        this->onPollRdband(fd, events, revents);
    }
    if (this->onPollWrban && (revents & POLLWRBAND)) {
        this->onPollWrban(fd, events, revents);
    }
    if (this->onPollErr && (revents & POLLERR)) {
        this->onPollErr(fd, events, revents);
    }
    if (this->onPollHup && (revents & POLLHUP)) {
        this->onPollHup(fd, events, revents);
    }
    if (this->onPollNval && (revents & POLLNVAL)) {
        this->onPollNval(fd, events, revents);
    }
}

short BasePollerEventsHandler::getEvents() {
    return this->events;
}


ExtendPollerEventsHandler::ExtendPollerEventsHandler(int fd)
        :BasePollerEventsHandler(fd) {

}

void ExtendPollerEventsHandler::registEventsHandler(const short events,
                                                    const BasePollerEventsHandler::PollEventsHandler eventsHandler) {
    BasePollerEventsHandler::registEventsHandler(events, eventsHandler);
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
}

void ExtendPollerEventsHandler::executeEventsHandler(const int fd, const short events, const short revents) {
    BasePollerEventsHandler::executeEventsHandler(fd, events, revents);
    if (this->onPollExtend && (revents & POLLEXTEND)) {
        this->onPollExtend(fd, events, revents);
    }
    if (this->onPollAttrib && (revents & POLLATTRIB)) {
        this->onPollAttrib(fd, events, revents);
    }
    if (this->onPollNlink && (revents & POLLNLINK)) {
        this->onPollNlink(fd, events, revents);
    }
    if (this->onPollWrite && (revents & POLLWRITE)) {
        this->onPollWrite(fd, events, revents);
    }
}


} /* namespace poll */
} /* namespace net */
} /* namespace chat */