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
#include "net/poll/Poller.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>

using namespace chat::net::poll;


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


    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
    return 0;
}