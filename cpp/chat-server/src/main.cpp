//============================================================================
// Name        : main.cpp
// Author      : HL
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <memory>
#include <iostream>
#include <vector>
using namespace std;

#include "protocol/codec/ByteBuffer.h"
#include "protocol/codec/CodecUtils.h"
using namespace chat::protocol::codec;

typedef void (*fun_sum)(int a, int b);

void sum(int a, int b) {
    cout << "call sum (" << a << ", " << b << ")" << endl;
}

struct Sum {
    void operator()(int a, int b) {
        cout << "call operator()(" << a << ", " << b << ")" << endl;
    }
};

template<class __sum>
void test(__sum fun) {
    cout << "execute test() " << endl;
    fun(2,3);
}

int main() {

    Sum sum1;
    test(sum);

    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
    return 0;
}