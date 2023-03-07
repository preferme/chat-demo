//
// Created by NAVER on 2023/3/6.
//

#ifndef __PROTOCOL_CODEC_BYTEBUFFER_H_
#define __PROTOCOL_CODEC_BYTEBUFFER_H_

#include "protocol/codec/CodecException.h"
#include <memory>
#include <ostream>

class ByteBuffer final {
public:
    ByteBuffer(const int capacity);
    virtual ~ByteBuffer();


//    char* array();
//    int capacity();
//    void capacity(int capacity);
//    void clear();
    // compare copy copy(index,length)
//    ByteBuffer& ensureWritable(int minWritableBytes);

private:
    char* value;
    int capacity;
    int readIndex;
    int writeIndex;
    int mark;
};


#endif //__PROTOCOL_CODEC_BYTEBUFFER_H_
