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
#include "net/ChatServer.h"

using namespace chat::net;

static bool running = true;

void onInterrupt(int value) {
    cout << "[onInterrupt] (" << value << ")" << endl;
    running = false;
}


int main() {

    // 为优雅关闭做准备
    ::signal(SIGINT,  onInterrupt);
    ::signal(SIGQUIT, onInterrupt);

    const char* ip = "127.0.0.1";
    const int port = 65432;

    ChatServer server(ip, port);
    server.bind();
    server.listen();
    server.startup();

    while(running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    server.shutdown();

    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
    return 0;
}