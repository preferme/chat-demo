//
// Created by NAVER on 2023/3/10.
//

#ifndef CPP_CHATSERVER_H
#define CPP_CHATSERVER_H

#include "net/poll/Poller.h"
#include "exception/CErrorException.h"
#include <string>

using namespace chat::net::poll;
using namespace chat::exception;

namespace chat {
namespace net {


class ChatServer {
public:
    typedef std::function<void(const int)> ErrorHandler;

    ChatServer(const std::string host, const int port);
    ~ChatServer();

    void bind() throw(CErrorException);
    void listen(int backlog = 100) throw(CErrorException);
    void startup();
    void shutdown();

protected:
    void onConnectionIncome(const int fd, const short events, const short revents);
private:
    std::string host;
    int port;
    int serverSocketFd;
    Poller serverPoller;
    Poller clientPoller;

};


} /* namespace net */
} /* namespace chat */
#endif //CPP_CHATSERVER_H
