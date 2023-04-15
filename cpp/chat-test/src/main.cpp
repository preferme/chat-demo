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
#include <thread>
#include <chrono>
#include <sstream>
using namespace std;

#include "util/thread_pool.hpp"
using namespace chat::util;

static bool running = true;
std::mutex g_mutex;

void onInterrupt(int value) {
    cout << "[onInterrupt] (" << value << ")" << endl;
    running = false;
}



int main(int argc, char* argv[]) {
    // 为优雅关闭做准备
    signal(SIGINT, onInterrupt);    // 2
    signal(SIGQUIT, onInterrupt);   // 3
    signal(SIGKILL, onInterrupt);   // 9
    signal(SIGTSTP, onInterrupt);   // 18

    thread_pool pool(5);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    pool.execute([](){
        cout << "task~~" << endl;
    });

    cout << "main::execute " << endl;



//    std::this_thread::sleep_for(std::chrono::seconds(3));

    for (int i = 0; i < 5; ++i) {
        pool.execute([i]() {
            lock_guard<mutex> lock(g_mutex);
            cout << "task_" << i << endl;
        });
    }

//    pool.shutdown();
//    for (int i = 0; i < 5; ++i) {
//        pool.execute(nullptr);
//    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
//    pool.shutdown();

//    pool.wait();


    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
    return 0;
}