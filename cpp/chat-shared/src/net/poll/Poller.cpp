/*
 * Poller.cpp
 *
 *  Created on: 2023年3月9日
 *      Author: hou-lei
 */

#include "net/poll/Poller.h"
#include <poll.h>
#include <errno.h>
#include <string.h>
#include <iostream>

extern int errno;


namespace chat {
namespace net {
namespace poll {


Poller::Poller()
        : pollerThread(nullptr), runnable(false), errorHandler(nullptr) {

}
Poller::~Poller() {
    shutdown();
}
void Poller::startup() {
    if (pollerThread == nullptr) {
        setRunnable(true);
        pollerThread = std::make_unique<std::thread>(Poller::executeCyclePoll, this);
    }
}
void Poller::shutdown() {
    setRunnable(false);
}
void Poller::join() {
    pollerThread->join();
}

void Poller::registEventsHandler(int fd, int events, PollEventsHandler handler) {
    std::lock_guard<std::mutex> __lg(this->mutex);
    if (this->eventsDispatchers.find(fd) == this->eventsDispatchers.end()) {
        this->eventsDispatchers[fd] = std::make_shared<PollerEventsHandler>(fd);
    }
    std::shared_ptr<PollerEventsHandler> dispatcher = this->eventsDispatchers[fd];
    dispatcher->registEventsHandler(events, handler);
}
void Poller::unregistEventsHandler(int fd) {
    std::lock_guard<std::mutex> __lg(this->mutex);
    this->eventsDispatchers.erase(fd);
}
int Poller::getEventsHandlerSize() {
    std::lock_guard<std::mutex> __lg(this->mutex);
    return this->eventsDispatchers.size();
}
void Poller::setCErrorHandler(CErrorHandler errorHandler) {
    this->errorHandler = std::move(errorHandler);
}


void Poller::setRunnable(bool runnable) {
    std::lock_guard<std::mutex> __lg(this->mutex);
    this->runnable = runnable;
}
bool Poller::getRunnable() {
    std::lock_guard<std::mutex> __lg(this->mutex);
    return this->runnable;
}
void Poller::onPollError(const int errorno) {
    if (this->errorHandler) {
        this->errorHandler(errorno);
    } else {
        // EAGAIN 内部数据结构的分配失败，但后续请求可能会成功。
        // EINTR 在轮询()期间捕获到信号。
        // EINVAL nfds参数大于 {OPEN_MAX}，[ XSR ]或 fd[选项开始]  成员之一指的是从多路复用器下游（直接或间接）链接的 STREAM 或多路复用器。
        switch (errorno) {
            case EAGAIN:
                std::cerr << "poll error : EAGAIN 内部数据结构的分配失败，但后续请求可能会成功。" << std::endl;
                break;
            case EINTR:
                std::cerr << "poll error : EINTR 在轮询()期间捕获到信号。" << std::endl;
                break;
            case EINVAL:
                std::cerr << "poll error :  EINVAL nfds参数大于 {OPEN_MAX}。" << std::endl;
                break;
            default:
                const char* msg =  strerror(errorno);
                std::cerr << "poll error : (" << errorno << ") " << msg << std::endl;
        }
    }
}
void Poller::onPollEvents(int fd, short events, short revents) {
    std::cout << "[Poller][onPollEvents] [" << std::this_thread::get_id() << "]  fd(" << fd << "), events(" << events << "), revents(" << revents << ")." << std::endl;
    if (this->eventsDispatchers.find(fd) != this->eventsDispatchers.end()) {
        this->eventsDispatchers[fd]->executeEventsHandler(fd, events, revents);
    } else {
        std::cout << "no eventHandler to be set yet." << std::endl;
    }
}
void Poller::executeCyclePoll(Poller* self) noexcept {
    std::cout << "[Poller][executeCyclePoll] [" << std::this_thread::get_id() << "] start." << std::endl;
    struct pollfd * fds = nullptr;
    nfds_t nfds = 0;
    int timeout = 100; // 100 ms
    while (self->getRunnable()) {
        self->mutex.lock();
        nfds = self->eventsDispatchers.size();
        struct pollfd pollfds[nfds];
        fds = pollfds;
        int index = 0;
        for (std::map<int,std::shared_ptr<PollerEventsHandler>>::iterator iter = self->eventsDispatchers.begin(); iter != self->eventsDispatchers.end(); ++iter) {
            pollfds[index].fd = iter->first;
            pollfds[index].events = iter->second->getEvents();
            index++;
        }
        self->mutex.unlock();
        if (nfds <= 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
//            std::cout << "[Poller][executeCyclePoll] [" << std::this_thread::get_id() << "] sleep 1 second." << std::endl;
            continue;
        }
        int result = ::poll(fds, nfds, timeout);
        // 值 0 表示 调用超时并且没有选择文件描述符
        if (result == 0) {
//            std::cout << "[Poller][executeCyclePoll] [" << std::this_thread::get_id() << "] poll nothing." << std::endl;
            continue;;
        }
        // 失败时，poll () 返回 -1 并设置 errno以指示错误。
        if (result < 0) {
            self->onPollError(errno);
//            std::cerr << "[Poller][executeCyclePoll] [" << std::this_thread::get_id() << "] poll error." << std::endl;
        }
        // 正值表示已选择的文件描述符总数（即，revents成员非零的文件描述符）
        if (result > 0) {
            for (int index = 0; index < nfds; ++index) {
                if (fds[index].revents) {
                    self->onPollEvents(fds[index].fd, fds[index].events, fds[index].revents);
//                } else {
//                    std::cout << "[Poller][executeCyclePoll] [" << std::this_thread::get_id() << "] fd: " << fds[index].fd;
//                    std::cout << ", events: " << fds[index].events << ", revents: " << fds[index].revents << std::endl;
                }
            }
        }
    }
    std::cout << "[Poller][executeCyclePoll] [" << std::this_thread::get_id() << "] exit." << std::endl;
}


} /* namespace poll */
} /* namespace net */
} /* namespace chat */