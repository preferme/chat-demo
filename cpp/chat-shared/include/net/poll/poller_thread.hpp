/*
 * poller_thread.hpp
 *
 *  Created on: 2023/4/16 2:49
 *      Author: hou-lei
 */
#ifndef __CHAT_DEMO_POLLER_THREAD_HPP_
#define __CHAT_DEMO_POLLER_THREAD_HPP_

#include "net/poll/poll_events_handler.hpp"
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <map>
#include <poll.h>
#include <errno.h>
#include <string.h>

extern int errno;

namespace chat {
namespace net {
namespace poll {


    class poller_thread : public std::enable_shared_from_this<poller_thread> {
    public:
        typedef std::function<void(const int)> cerror_handler_type;
    private:
        volatile bool m_terminate;
        std::thread* m_thread;
        mutable std::mutex m_monitor;
        mutable std::map<int, std::shared_ptr<poll_events_handler>> m_events_dispatcher;
        mutable cerror_handler_type m_error_handler;

    public:
        poller_thread()
                : m_terminate(false)
                , m_thread(nullptr)
                , m_monitor()
                , m_events_dispatcher()
                , m_error_handler(nullptr) {

        }

        ~poller_thread() {
            shutdown(true);
        }

        void startup() {
            std::unique_lock<std::mutex> lock(m_monitor);
            if (m_thread == nullptr) {
                m_terminate = false;
                m_thread = new std::thread(std::bind(&poller_thread::execute_poll_cycle, this));
            }
        }

        void join() {
            std::thread* p_thread = nullptr;
            {
                std::unique_lock<std::mutex> lock(m_monitor);
                p_thread = m_thread;
            }
            if (p_thread != nullptr) {
                p_thread->join();
            }
        }

        void shutdown(bool wait) {
            m_terminate = true;
            std::unique_lock<std::mutex> lock(m_monitor);
            if (m_thread != nullptr) {
                if (wait) {
                    m_thread->join();
                } else {
                    m_thread->detach();
                }
                m_thread = nullptr;
            }
        }

    private:
        void on_poll_error(int error_no) {
            if (m_error_handler != nullptr) {
                m_error_handler(error_no);
            } else {
                // EAGAIN 内部数据结构的分配失败，但后续请求可能会成功。
                // EINTR 在轮询()期间捕获到信号。
                // EINVAL nfds参数大于 {OPEN_MAX}，[ XSR ]或 fd[选项开始]  成员之一指的是从多路复用器下游（直接或间接）链接的 STREAM 或多路复用器。
                switch (error_no) {
                    case EAGAIN:
                        fprintf(stderr, "poll error : EAGAIN 内部数据结构的分配失败，但后续请求可能会成功。");
                        break;
                    case EINTR:
                        fprintf(stderr, "poll error : EINTR 在轮询()期间捕获到信号。");
                        break;
                    case EINVAL:
                        fprintf(stderr, "poll error :  EINVAL nfds参数大于 {OPEN_MAX}。");
                        break;
                    default:
                        const char* msg =  strerror(error_no);
                        fprintf(stderr, "poll error : (%d) %s", error_no, msg);
                }
            }
        }

        void on_poll_events(int fd, short events, short revents) {
            if (m_events_dispatcher.find(fd) != m_events_dispatcher.end()) {
                m_events_dispatcher[fd]->execute_event_handler(fd, events, revents);
            }
//            else {
//                std::cout << "no eventHandler to be set yet." << std::endl;
//            }
        }

        void execute_poll_cycle() {
            struct pollfd * fds = nullptr;
            nfds_t nfds = 0;
            int timeout = 100; // 100 ms
            while (!m_terminate) {
                {
                    std::unique_lock<std::mutex> lock(m_monitor);
                    nfds = m_events_dispatcher.size();
                    if (nfds > 0) {
                        struct pollfd pollfds[nfds];
                        fds = pollfds;
                        int index = 0;
                        for (std::map<int,std::shared_ptr<poll_events_handler>>::iterator iter = m_events_dispatcher.begin(); iter != m_events_dispatcher.end(); ++iter) {
                            pollfds[index].fd = iter->first;
                            pollfds[index].events = iter->second->get_events();
                            index++;
                        }
                    }
                }
                if (nfds <= 0) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    continue;
                }
                int result = ::poll(fds, nfds, timeout);
                // 值 0 表示 调用超时并且没有选择文件描述符
                if (result == 0) {
                    continue;;
                }
                // 失败时，poll () 返回 -1 并设置 errno以指示错误。
                if (result < 0) {
                    on_poll_error(errno);
//            std::cerr << "[Poller][executeCyclePoll] [" << std::this_thread::get_id() << "] poll error." << std::endl;
                }
                // 正值表示已选择的文件描述符总数（即，revents成员非零的文件描述符）
                if (result > 0) {
                    for (int index = 0; index < nfds; ++index) {
                        if (fds[index].revents) {
                            on_poll_events(fds[index].fd, fds[index].events, fds[index].revents);
//                } else {
//                    std::cout << "[Poller][executeCyclePoll] [" << std::this_thread::get_id() << "] fd: " << fds[index].fd;
//                    std::cout << ", events: " << fds[index].events << ", revents: " << fds[index].revents << std::endl;
                        }
                    }
                }
            }
//            std::cout << "[Poller][executeCyclePoll] [" << std::this_thread::get_id() << "] exit." << std::endl;
        }
    };


}
}
}

#endif //__CHAT_DEMO_POLLER_THREAD_HPP_
