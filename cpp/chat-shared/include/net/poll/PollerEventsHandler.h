/*
 * PollerEventsHandler.h
 *
 *  Created on: 2023年3月9日
 *      Author: hou-lei
 */

#ifndef CPP_POLLEREVENTSHANDLER_H
#define CPP_POLLEREVENTSHANDLER_H

#include <poll.h>
#include <functional>

namespace chat {
namespace net {
namespace poll {


class PollerEventsHandler {
public:
    typedef std::function<void(const int, const short, const short)> PollEventsHandler;

    PollerEventsHandler(const int fd);
    virtual void registEventsHandler(const short events, const PollEventsHandler eventsHandler);
    virtual void executeEventsHandler(const int fd, const short events, const short revents);
    short getEvents();

protected:
    int fd;
    short events;
//    #define POLLIN          0x0001          /* any readable data available */
//    #define POLLPRI         0x0002          /* OOB/Urgent readable data */
//    #define POLLOUT         0x0004          /* file descriptor is writeable */
    PollEventsHandler onPollIn;
    PollEventsHandler onPollPri;
    PollEventsHandler onPollOut;
    /*
     * These events are set if they occur regardless of whether they were
     * requested.
     */
//    #define POLLERR         0x0008          /* some poll error occurred */
//    #define POLLHUP         0x0010          /* file descriptor was "hung up" */
//    #define POLLNVAL        0x0020          /* requested events "invalid" */
    PollEventsHandler onPollErr;
    PollEventsHandler onPollHup;
    PollEventsHandler onPollNval;
#if POLLRDNORM != POLLIN
//    #define POLLRDNORM      0x0040          /* non-OOB/URG data available */
//    #define POLLWRNORM      POLLOUT         /* no write type differentiation */
//    #define POLLRDBAND      0x0080          /* OOB/Urgent readable data */
//    #define POLLWRBAND      0x0100          /* OOB/Urgent data can be written */
    PollEventsHandler onPollRdnorm;
    PollEventsHandler onPollWrnorm;
    PollEventsHandler onPollRdband;
    PollEventsHandler onPollWrban;
#endif
#if defined(POLLEXTEND)
    /*
     * FreeBSD extensions: polling on a regular file might return one
     * of these events (currently only supported on local filesystems).
     */
//    #define POLLEXTEND      0x0200          /* file may have been extended */
//    #define POLLATTRIB      0x0400          /* file attributes may have changed */
//    #define POLLNLINK       0x0800          /* (un)link/rename may have happened */
//    #define POLLWRITE       0x1000          /* file's contents may have changed */
    PollEventsHandler onPollExtend;
    PollEventsHandler onPollAttrib;
    PollEventsHandler onPollNlink;
    PollEventsHandler onPollWrite;
#endif
};



} /* namespace poll */
} /* namespace net */
} /* namespace chat */

#endif //CPP_POLLEREVENTSHANDLER_H
