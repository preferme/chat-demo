//
// https://netty.io/4.0/api/io/netty/buffer/ByteBuf.html
// https://cplusplus.com/reference/mutex/
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

    char* array();
    int capacity();
    void capacity(int capacity);
    void clear();
    // compare copy copy(index,length)
    ByteBuffer& ensureWritable(int minWritableBytes);
    int ensureWritable(int minWritableBytes, bool force);
    // equals
    // forEach-iterator
    bool getBool(int index);
    char getByte(int index);
    ByteBuffer& getBytes(int index, char* dst);
    ByteBuffer& getBytes(int index, char* dst, int dstIndex, int length);
    ByteBuffer& getBytes(int index, ByteBuffer& dst);
    ByteBuffer& getBytes(int index, ByteBuffer& dst, int dstIndex, int length);
    char getChar(int index);
    double getDouble(int index);
    float getFloat(int index);
    int getInt(int index);
    long getLong(int index);
    int getMedium(int index);
    short getShort(int index);
    bool hasArray();
    int indexOf(int fromIndex, int toIndex, char value);
    bool isReadable(int size=0);
    bool isWritable(int size=0);
    ByteBuffer& markReaderIndex();
    ByteBuffer& markWriterIndex();
    int maxCapacity();
    int maxWritableBytes();
    int readableBytes();
    bool readBoolean();
    char readByte();


private:
    char* value;
    int capacity;
    int readIndex;
    int writeIndex;
    int mark;
};


#endif //__PROTOCOL_CODEC_BYTEBUFFER_H_
