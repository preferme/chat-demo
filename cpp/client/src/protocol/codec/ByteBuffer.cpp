//
// Created by NAVER on 2023/3/6.
//

#include "ByteBuffer.h"

ByteBuffer::ByteBuffer(const int capacity)
        : capacity(capacity), readIndex(0), writeIndex(0), mark(0), value(new char[capacity]) {

}

ByteBuffer::~ByteBuffer() {
    if (this->value) {
        delete [] this->value;
    }
}


