//
// Created by NAVER on 2023/3/10.
//

#ifndef CPP_CHATCLIENT_H
#define CPP_CHATCLIENT_H

#include "net/poll/Poller.h"
using namespace chat::net::poll;
#include <string>

namespace chat {
namespace net {


class ChatClient {
public:
    ChatClient(std::string host, int port);
    ~ChatClient();
    void connect();
    void handle_conn();
    void shutdown();
private:
    std::string remoteIp;
    int remotePort;
    int clientFd;
    Poller poller;
};



} /* namespace net */
} /* namespace chat */
#endif //CPP_CHATCLIENT_H
