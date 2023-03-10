//
// Created by NAVER on 2023/3/10.
//

#include "net/ChatServer.h"
#include "exception/CErrorException.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <fcntl.h>

using namespace chat::exception;


namespace chat {
namespace net {


ChatServer::ChatServer(const std::string host, const int port)
        :host(host), port(port), serverSocketFd(0), serverPoller() {

//    this->serverPoller.setCErrorHandler([](int){    });

}

ChatServer::~ChatServer() {
    this->shutdown();
}

void ChatServer::bind() {
    // 创建 tcp socket 套接字
    if (this->serverSocketFd <= 0) {
        this->serverSocketFd = ::socket(AF_INET, SOCK_STREAM, 0);
        if(this->serverSocketFd == -1) {
            THROW_EXCEPTION(CErrorException);
        }
    }
    // 构建 socket address 结构体
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(this->port);
    server_addr.sin_addr.s_addr = this->host.empty()
                                ? INADDR_ANY
                                : inet_addr(this->host.c_str());
    // 绑定IP地址和端口号
    if(::bind(this->serverSocketFd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) == -1) {
        close(this->serverSocketFd);
        THROW_EXCEPTION(CErrorException);
    }

    std::cout << "[ChatServer][bind] bind address : " << inet_ntoa(server_addr.sin_addr) << ":" << ntohs(server_addr.sin_port) << std::endl;
}

void ChatServer::listen(int backlog) {
    if (::listen(this->serverSocketFd, backlog) == -1) {
        THROW_EXCEPTION(CErrorException);
    }
//    this->serverPoller.registEventsHandler(this->serverSocketFd, POLLIN, std::bind(&ChatServer::onConnectionIncome, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    this->serverPoller.registEventsHandler(this->serverSocketFd, POLLIN, [&](const int fd, const short events, const short revents) {
        this->onConnectionIncome(fd, events, revents);
    });
    std::cout << "[ChatServer][listen] listen." << std::endl;
}

void ChatServer::onConnectionIncome(const int fd, const short events, const short revents) {
    std::cout << "[ChatServer][onConnectionIncome] income ." << std::endl;
    struct sockaddr_in client_addr;
    socklen_t clitaddrlen = sizeof(client_addr);
    int clientFd = ::accept(this->serverSocketFd, (struct sockaddr*)&client_addr, &clitaddrlen);
    if (clientFd == -1) {
        std::cerr << "[ChatServer][onConnectionIncome] accept failed (" << errno << ")" << strerror(events) << std::endl;
        return;
    }
    std::cout << "[ChatServer][onConnectionIncome] accept connection (" << clientFd << ")." << std::endl;
    int fl = ::fcntl(clientFd, F_GETFL, 0);
    ::fcntl(clientFd, F_SETFL, fl | O_NONBLOCK);
    clientPoller.registEventsHandler(clientFd, POLLIN, [&](const int fd, const short events, const short revents){
        char buffer[1024]= {0};
        int len = ::read(fd, buffer, 1024);
        if (len <= 0) {
            std::cerr << "[ChatServer][onConnectionIncome] read fd(" << fd << ") failed (" << len << ")." << std::endl;
            clientPoller.unregistEventsHandler(fd);
            ::close(fd);
            return;
        }
        std::cout << "[income] " << buffer << std::flush;//<< std::endl;
        ::write(fd, buffer, len);
    });
}

void ChatServer::startup() {
    serverPoller.startup();
    clientPoller.startup();
    std::cout << "[ChatServer][startup] ." << std::endl;
}

void ChatServer::shutdown() {
    serverPoller.shutdown();
    clientPoller.shutdown();
    std::cout << "[ChatServer][shutdown] ." << std::endl;
}



} /* namespace net */
} /* namespace chat */