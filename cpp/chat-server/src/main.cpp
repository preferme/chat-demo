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



// -----------------------
#include "net/chat_server.hpp"

using namespace chat::net;

static bool running = true;

void onInterrupt(int value) {
    cout << "[onInterrupt] (" << value << ")" << endl;
    running = false;
//    exit(0);
}


int main(int argc, char* argv[]) {

    // 为优雅关闭做准备
    signal(SIGINT, onInterrupt);    // 2
    signal(SIGQUIT, onInterrupt);   // 3
    signal(SIGKILL, onInterrupt);   // 9
    signal(SIGTERM, onInterrupt);   // 15
    signal(SIGTSTP, onInterrupt);   // 18

    const char* ip = "127.0.0.1";
    int port = 65432;

    if (argc == 2) {
        port = ::atoi(argv[1]);
    }

    std::shared_ptr<chat_server> server = std::make_shared<chat_server>(ip, port);
    server->bind();
    server->listen();
    server->startup();

    while(running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    server->shutdown();

    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
    return 0;
}