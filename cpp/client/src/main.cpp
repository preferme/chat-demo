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


int main() {
    byte memory[100]{0x34, 0x12};
    short value = 0x1234;
    int b = CodecUtils::getShort(memory, 0);
    cout << "memory " << b << "  value " << value <<endl;


    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
    return 0;
}