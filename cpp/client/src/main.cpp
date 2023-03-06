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


int main() {
    std::unique_ptr<ByteBuffer> buffer = std::make_unique<ByteBuffer>();

    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
    return 0;
}