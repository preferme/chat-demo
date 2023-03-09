//============================================================================
// Name        : main.cpp
// Author      : HL
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <signal.h>
#include <memory>
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

#include "protocol/codec/ByteBuffer.h"
#include "protocol/codec/CodecUtils.h"
using namespace chat::protocol::codec;

void onInterrupt(int value) {
    cout << "[onInterrupt] (" << value << ")" << endl;
}

// -----------------------
#include "Poller.h"
#include "ErrorHandler.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

class PollerErrorHandler : public ErrorHandler {
public:
    void doHandler(const int error) override {
        cout << "ON PollerErrorHandler " << endl;
    }
};

int main() {
    // 为优雅关闭做准备
    signal(SIGINT, onInterrupt);

    const char* ip = "127.0.0.1";
    const int port = 65432;

    int socketfd = socket(AF_INET,SOCK_STREAM,0);
    if(socketfd == -1)
    {
        perror("socket");
        exit(-1);
    }
    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(struct sockaddr_in));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(port);
    server_addr.sin_addr.s_addr=inet_addr(ip);
    //将cfd连接到制定的服务器网络地址server_addr
    if(connect(socketfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr)) == -1)
    {
        perror("connect");
        close(socketfd);
        exit(-1);
    }
    Poller poller;

    PollerErrorHandler errorHandler;
    poller.setErrorHandler(&errorHandler);
    poller.regestEventsHandler(socketfd, POLLIN, [](const int fd, const short events, const short revents) {
        cout << "ON POLLIN event" << endl;
    });




    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
    return 0;
}