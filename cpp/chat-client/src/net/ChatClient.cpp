//
// Created by NAVER on 2023/3/10.
//

#include "../../include/net/ChatClient.h"
#include "exception/CErrorException.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <netinet/tcp.h>

using namespace chat::exception;

namespace chat {
namespace net {


ChatClient::ChatClient(std::string host, int port)
        :remoteIp(host), remotePort(port), clientFd(0), poller() {

}

ChatClient::~ChatClient() {
    poller.shutdown();
}

void ChatClient::connect() {
    // 创建 tcp socket 套接字
    if (this->clientFd <= 0) {
        this->clientFd = ::socket(AF_INET, SOCK_STREAM, 0);
        if(this->clientFd == -1) {
            THROW_EXCEPTION(CErrorException);
        }
    }
    // 构建 socket address 结构体
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(this->remotePort);
    server_addr.sin_addr.s_addr = ::inet_addr(this->remoteIp.c_str());
    // 将 clientFd 连接到指定的服务器 网络地址 server_addr
    if(::connect(this->clientFd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1) {
        close(this->clientFd);
        THROW_EXCEPTION(CErrorException);
    }

    std::cout << "[ChatClient][connect] connect address :=> " << inet_ntoa(server_addr.sin_addr) << ":" << ntohs(server_addr.sin_port) << std::endl;
}

void ChatClient::handle_conn() {
    int fl = ::fcntl(this->clientFd, F_GETFL, 0);
    ::fcntl(this->clientFd, F_SETFL, fl | O_NONBLOCK);
    fl = ::fcntl(STDIN_FILENO, F_GETFL, 0);
    ::fcntl(STDIN_FILENO, F_SETFL, fl | O_NONBLOCK);
    int no_delay = 1;
    setsockopt(this->clientFd, IPPROTO_TCP, TCP_NODELAY, &no_delay, sizeof(int));
    int socketFd = this->clientFd;
    poller.registEventsHandler(STDIN_FILENO, POLLIN, [&](const int fd, const short events, const short revents){
        char buffer[1024] = {0};
        int len = ::read(STDIN_FILENO, buffer, 1024);
        if (len <= 0) {
            std::cerr << "[ChatClient][handle_conn] read stdin failed ("<< len << ")." << std::endl;
            ::shutdown(socketFd, SHUT_WR);
            return;
        }
        ::write(socketFd, buffer, len);
    });
    poller.registEventsHandler(this->clientFd, POLLIN, [&](const int fd, const short events, const short revents){
        char buffer[1024] = {0};
        int len = ::read(fd, buffer, 1024);
        if (len <= 0) {
            std::cerr << "[ChatClient][handle_conn] read socket failed("<< len << ")." << std::endl;
            poller.unregistEventsHandler(fd);
            close(fd);
            return;
        }
        ::write(STDIN_FILENO, buffer, len);
    });
}

void ChatClient::startup() {
    poller.startup();
}

void ChatClient::shutdown() {
    poller.shutdown();
}

} /* namespace net */
} /* namespace chat */