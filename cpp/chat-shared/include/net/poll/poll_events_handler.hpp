/*
 * poll_event_handler.hpp
 *
 *  Created on: 2023/4/16 2:12
 *      Author: hou-lei
 */
#ifndef __CHAT_DEMO_POLL_EVENT_HANDLER_HPP_
#define __CHAT_DEMO_POLL_EVENT_HANDLER_HPP_

#include "net/poll/poll_event.hpp"
#include <memory>

namespace chat {
namespace net {
namespace poll {

    class poll_events_handler : public std::enable_shared_from_this<poll_events_handler> {
    private:
        int m_fd;
        short m_events;
        poll_event_handler m_onPollIn;
        poll_event_handler m_onPollPri;
        poll_event_handler m_onPollOut;
        poll_event_handler m_onPollErr;
        poll_event_handler m_onPollHup;
        poll_event_handler m_onPollNval;

    public:
        poll_events_handler(const int fd)
                : m_fd(fd)
                , m_events(0)
                , m_onPollIn(nullptr)
                , m_onPollPri(nullptr)
                , m_onPollOut(nullptr)
                , m_onPollErr(nullptr)
                , m_onPollHup(nullptr)
                , m_onPollNval(nullptr) {

        }

        void regist_event_handler(const short events, const poll_event_handler handler) {
            this->m_events |= events;

            if (events & POLLIN) {
                this->m_onPollIn = std::move(handler);
            }
            if (events & POLLPRI) {
                this->m_onPollPri = std::move(handler);
            }
            if (events & POLLOUT) {
                this->m_onPollOut = std::move(handler);
            }
            if (events & POLLERR) {
                this->m_onPollErr = std::move(handler);
            }
            if (events & POLLHUP) {
                this->m_onPollHup = std::move(handler);
            }
            if (events & POLLNVAL) {
                this->m_onPollNval = std::move(handler);
            }
        }

        void execute_event_handler(const int fd, const short events, const short revents) {
            if (this->m_onPollIn != nullptr && (revents & POLLIN)) {
                this->m_onPollIn(fd, events, revents);
            }
            if (this->m_onPollOut != nullptr && (revents & POLLOUT)) {
                this->m_onPollOut(fd, events, revents);
            }
            if (this->m_onPollPri != nullptr && (revents & POLLPRI)) {
                this->m_onPollPri(fd, events, revents);
            }
            if (this->m_onPollErr != nullptr && (revents & POLLERR)) {
                this->m_onPollErr(fd, events, revents);
            }
            if (this->m_onPollHup != nullptr && (revents & POLLHUP)) {
                this->m_onPollHup(fd, events, revents);
            }
            if (this->m_onPollNval != nullptr && (revents & POLLNVAL)) {
                this->m_onPollNval(fd, events, revents);
            }
        }

        short get_events() {
            return this->m_events;
        }

        std::shared_ptr<poll_events_handler> shared_ptr() {
            return this->shared_from_this();
        }

    };


}
}
}
#endif //__CHAT_DEMO_POLL_EVENT_HANDLER_HPP_
