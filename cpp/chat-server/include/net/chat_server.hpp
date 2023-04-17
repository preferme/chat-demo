/*
 * chat_server.hpp
 *
 * Created on: 2023/4/17 11:31.
 *     Author: hou-lei
 */
#ifndef CHAT_DEMO_CHAT_SERVER_HPP
#define CHAT_DEMO_CHAT_SERVER_HPP

#include <string>
#include <memory>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "net/poll/poller_thread.hpp"
#include "exception/cerror_exception.hpp"

using namespace chat::net::poll;
using namespace chat::exception;

namespace chat {
namespace net {


    class chat_server :std::enable_shared_from_this<chat_server>{
    private:
        std::string m_host;
        int m_port;
        int m_server_socket_fd;
        std::shared_ptr<poller_thread> m_server_poller;
        std::shared_ptr<poller_thread> m_client_poller;

    public:
        chat_server(const std::string host, const int port)
                :m_host(host), m_port(port), m_server_socket_fd(0)
                , m_server_poller(std::make_shared<poller_thread>())
                , m_client_poller(std::make_shared<poller_thread>()) {

        }

        ~chat_server() {
            shutdown();
        }

        void startup() {
            std::cout << "[chat_server][startup] ." << std::endl;
            m_server_poller->startup();
            m_client_poller->startup();
        }

        void shutdown() {
            std::cout << "[chat_server][shutdown] ." << std::endl;
            m_server_poller->shutdown(true);
            m_client_poller->shutdown(true);
        }

        void bind() {
            // 创建 tcp socket 套接字
            if (this->m_server_socket_fd <= 0) {
                this->m_server_socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
                if(this->m_server_socket_fd == -1) {
                    THROW_EXCEPTION(cerror_exception);
                }
            }
            // 构建 socket address 结构体
            struct sockaddr_in server_addr;
            memset(&server_addr, 0, sizeof(struct sockaddr_in));
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(this->m_port);
            server_addr.sin_addr.s_addr = this->m_host.empty()
                                          ? INADDR_ANY
                                          : inet_addr(this->m_host.c_str());
            // 绑定IP地址和端口号
            if(::bind(this->m_server_socket_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) == -1) {
                close(this->m_server_socket_fd);
                THROW_EXCEPTION(cerror_exception);
            }

            std::cout << "[chat_server][bind] bind address : " << inet_ntoa(server_addr.sin_addr) << ":" << ntohs(server_addr.sin_port) << std::endl;
        }

        void listen(int backlog = 100) {
            if (::listen(this->m_server_socket_fd, backlog) == -1) {
                THROW_EXCEPTION(cerror_exception);
            }
//    this->serverPoller.registEventsHandler(this->serverSocketFd, POLLIN, std::bind(&ChatServer::onConnectionIncome, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
            this->m_server_poller->regist_events_handler(this->m_server_socket_fd, POLLIN, [this](const int fd, const short events, const short revents) {
                this->on_connection_income(fd, events, revents);
            });
            std::cout << "[chat_server][listen] listen." << std::endl;
        }

    private:
        void on_connection_income(const int fd, const short events, const short revents) {
            std::cout << "[chat_server][on_connection_income] begin" << std::endl;
            struct sockaddr_in client_addr;
            socklen_t clitaddrlen = sizeof(client_addr);
            int clientFd = ::accept(this->m_server_socket_fd, (struct sockaddr*)&client_addr, &clitaddrlen);
            if (clientFd == -1) {
                std::cerr << "[chat_server][on_connection_income] accept failed (" << errno << ")" << strerror(events) << std::endl;
                return;
            }
            std::cout << "[chat_server][on_connection_income] accept connection (" << clientFd << ")." << std::endl;
            int fl = ::fcntl(clientFd, F_GETFL, 0);
            ::fcntl(clientFd, F_SETFL, fl | O_NONBLOCK);
            m_client_poller->regist_events_handler(clientFd, POLLIN, [this](const int fd, const short events, const short revents){
                char buffer[1024]= {0};
                int len = ::read(fd, buffer, 1024);
                if (len <= 0) {
                    std::cerr << "[chat_server][on_connection_income][client_poller] read fd(" << fd << ") failed (" << len << ")." << std::endl;
                    this->m_client_poller->unregist_events_handler(fd);
                    std::cerr << "[chat_server][on_connection_income][client_poller] close fd(" << fd << ") " << std::endl;
                    ::close(fd);
//                    std::cerr << "[chat_server][on_connection_income][client_poller] close fd(" << fd << ") over." << std::endl;
                    return;
                }
                std::cout << "[chat_server][on_connection_income][client_poller] " << buffer << std::flush;//<< std::endl;
//        ::printf("[income] %s\n", buffer);
                ::write(fd, buffer, len);
            });
        }
    };


}
}
#endif //CHAT_DEMO_CHAT_SERVER_HPP
