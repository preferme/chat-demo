/*
 * chat_client.hpp
 *
 *  Created on: 2023/4/16 14:28
 *      Author: hou-lei
 */
#ifndef __CHAT_DEMO_CHAT_CLIENT_HPP_
#define __CHAT_DEMO_CHAT_CLIENT_HPP_

#include "net/poll/poller_thread.hpp"
#include "exception/cerror_exception.hpp"
#include <string>
#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <cstdio>

namespace chat {
namespace net {

    class chat_client : std::enable_shared_from_this<chat_client> {

    private:
        std::string m_remote_ip;
        int m_remote_port;
        int m_client_fd;
        std::shared_ptr<poll::poller_thread> m_poller;

    public:
        chat_client(std::string host, int port)
                : m_remote_ip(host)
                , m_remote_port(port)
                , m_client_fd(0)
                , m_poller(std::make_shared<poll::poller_thread>()) {

        }

        ~chat_client() {
            m_poller->shutdown(true);
        }

        void startup() {
            m_poller->startup();
        }

        void shutdown() {
            m_poller->shutdown(true);
        }

        void connect() {
            // 创建 tcp socket 套接字
            if (this->m_client_fd <= 0) {
                this->m_client_fd = ::socket(AF_INET, SOCK_STREAM, 0);
                if(this->m_client_fd == -1) {
                    THROW_EXCEPTION(chat::exception::cerror_exception);
                }
            }
            // 构建 socket address 结构体
            struct sockaddr_in server_addr;
            memset(&server_addr, 0, sizeof(struct sockaddr_in));
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(this->m_remote_port);
            server_addr.sin_addr.s_addr = ::inet_addr(this->m_remote_ip.c_str());
            // 将 m_client_fd 连接到指定的服务器 网络地址 server_addr
            if(::connect(this->m_client_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1) {
                close(this->m_client_fd);
                THROW_EXCEPTION(chat::exception::cerror_exception);
            }

            ::printf("[ChatClient][connect] connect address :=> %s:%d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
        }

        void handle_connection() {
            int fl = ::fcntl(this->m_client_fd, F_GETFL, 0);
            ::fcntl(this->m_client_fd, F_SETFL, fl | O_NONBLOCK);
            fl = ::fcntl(STDIN_FILENO, F_GETFL, 0);
            ::fcntl(STDIN_FILENO, F_SETFL, fl | O_NONBLOCK);
            int no_delay = 1;
            setsockopt(this->m_client_fd, IPPROTO_TCP, TCP_NODELAY, &no_delay, sizeof(int));
            int socketFd = this->m_client_fd;
            m_poller->regist_events_handler(STDIN_FILENO, POLLIN, [socketFd](const int fd, const short events, const short revents){
                char buffer[1024] = {0};
                int len = ::read(STDIN_FILENO, buffer, 1024);
                if (len <= 0) {
                    ::fprintf(stderr, "[ChatClient][handle_connection] read stdin failed (%d).\n", len);
                    ::shutdown(socketFd, SHUT_WR);
                    return;
                }
                ::printf("[ChatClient][handle_connection] send(%d):(%d) %s", socketFd, len, buffer);
                ::write(socketFd, buffer, len);
            });
            m_poller->regist_events_handler(this->m_client_fd, POLLIN, [this](const int fd, const short events, const short revents){
                char buffer[1024] = {0};
                int len = ::read(fd, buffer, 1024);
                if (len <= 0) {
                    ::fprintf(stderr, "[ChatClient][handle_connection] read stdin failed (%d).\n", len);
                    this->m_poller->unregist_events_handler(fd);
                    close(fd);
                    return;
                }
                ::printf("[ChatClient][handle_connection] receive(%d):(%d) %s", fd, len, buffer);
            });
        }

    };

}
}

#endif //__CHAT_DEMO_CHAT_CLIENT_HPP_
