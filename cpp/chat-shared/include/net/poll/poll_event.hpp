/*
 * poll_event.hpp
 *
 *  Created on: 2023/4/16 2:06
 *      Author: hou-lei
 */
#ifndef __CHAT_DEMO_POLL_EVENT_HPP_
#define __CHAT_DEMO_POLL_EVENT_HPP_

#include <functional>

namespace chat {
namespace net {
namespace poll {

    enum poll_event {
//    #define POLLIN          0x0001          /* any readable data available */
//    #define POLLPRI         0x0002          /* OOB/Urgent readable data */
//    #define POLLOUT         0x0004          /* file descriptor is writeable */
//    #define POLLERR         0x0008          /* some poll error occurred */
//    #define POLLHUP         0x0010          /* file descriptor was "hung up" */
//    #define POLLNVAL        0x0020          /* requested events "invalid" */
        POLLIN = 0x0001,
        POLLPRI = 0x0002,
        POLLOUT = 0x0004,
        POLLERR = 0x0008,
        POLLHUP = 0x0010,
        POLLNVAL = 0x0020
    };

    // void poll_event_handler(const int fd, const short events, const short revents)
    typedef std::function<void(const int, const short, const short)> poll_event_handler;

}
}
}

#endif //__CHAT_DEMO_POLL_EVENT_HPP_
