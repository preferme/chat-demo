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
#include "net/ChatClient.h"
#include "exception/CErrorException.h"

using namespace chat::net;
using namespace chat::exception;

int main(int argc, char* argv[]) {
    // 为优雅关闭做准备
    signal(SIGINT, onInterrupt);

    char* ip = "127.0.0.1";
    int port = 65432;

    if (argc == 3) {
        ip = argv[1];
        port = ::atoi(argv[2]);
    }

    ChatClient client(ip, port);

    try {
        client.connect();
        client.handle_conn();
        client.startup();
    } catch (Exception& ex) {
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