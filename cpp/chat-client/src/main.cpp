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

static bool running = true;

void onInterrupt(int value) {
    cout << "[onInterrupt] (" << value << ")" << endl;
    running = false;
}

// -----------------------
//#include "net/ChatClient.h"
//#include "exception/CErrorException.h"
#include "net/chat_client.hpp"
#include "exception/cerror_exception.hpp"

using namespace chat::net;
using namespace chat::exception;

int main(int argc, char* argv[]) {
    cout << "[main] signal " << endl;
    // 为优雅关闭做准备
    signal(SIGINT, onInterrupt);    // 2
    signal(SIGQUIT, onInterrupt);   // 3
    signal(SIGKILL, onInterrupt);   // 9
    signal(SIGTERM, onInterrupt);   // 15
    signal(SIGTSTP, onInterrupt);   // 18

    const char* ip = "127.0.0.1";
    int port = 65432;

    if (argc == 3) {
        ip = argv[1];
        port = ::atoi(argv[2]);
    }
    cout << "[main] make client " << endl;
    chat_client client(ip, port);

    try {
        cout << "[main] connect " << endl;
        client.connect();
        cout << "[main] regist handler " << endl;
        client.handle_connection();
        cout << "[main] start polling " << endl;
        client.startup();
    } catch (chat::exception::exception& ex) {
        std::cerr << ex << endl;
    }

    while(running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    client.shutdown();
    while(running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    client.shutdown();

    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
    return 0;
}