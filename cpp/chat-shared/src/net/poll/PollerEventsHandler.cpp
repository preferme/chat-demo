/*
 * PollerEventsHandler.cpp
 *
 *  Created on: 2023年3月9日
 *      Author: hou-lei
 */

#include "net/poll/PollerEventsHandler.h"
#include <poll.h>

namespace chat {
namespace net {
namespace poll {


PollerEventsHandler::PollerEventsHandler(const int fd) : fd(fd), events(0)
            , onPollIn(nullptr), onPollPri(nullptr), onPollOut(nullptr), onPollErr(nullptr), onPollHup(nullptr), onPollNval(nullptr)
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
}

short PollerEventsHandler::getEvents() {
    return this->events;
}



} /* namespace poll */
} /* namespace net */
} /* namespace chat */