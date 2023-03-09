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

int main() {
    ByteBuffer buff(2048);
    cout << buff << endl;


    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
    return 0;
}