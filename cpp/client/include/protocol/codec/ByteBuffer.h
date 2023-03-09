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
//  compare copy copy(index,length)
//  equals hashCode
//  toString
    friend std::ostream& operator<<(std::ostream& os, const ByteBuffer& package);
//  forEach-iterator
//  int maxCapacity();

    char* array();
    int capacity();
    ByteBuffer& capacity(int newCapacity);
    ByteBuffer& clear();

    ByteBuffer& ensureWritable(int minWritableBytes);
    int ensureWritable(int minWritableBytes, bool force);
    int indexOf(int fromIndex, int toIndex, char value);
    bool isReadable(int size=0);
    bool isWritable(int size=0);
    ByteBuffer& markReaderIndex();
    ByteBuffer& markWriterIndex();
    int readableBytes();
    int readerIndex();
    ByteBuffer& readerIndex(int readerIndex);
    ByteBuffer& resetReaderIndex();
    ByteBuffer& resetWriterIndex();
    ByteBuffer& setIndex(int readerIndex, int writerIndex);
    ByteBuffer& skipBytes(int length);
    int writableBytes();
    int writerIndex();
    ByteBuffer& writerIndex(int writerIndex);

    // --- get data methods --- //
    bool getBool(int index);
    char getByte(int index);
    ByteBuffer& getBytes(int index, char* dst, int dstLength, int length);
    ByteBuffer& getBytes(int index, char* dst, int length);
    ByteBuffer& getBytes(int index, ByteBuffer& dst, int dstIndex, int length);
    ByteBuffer& getBytes(int index, ByteBuffer& dst, int length);
    ByteBuffer& getBytes(int index, ByteBuffer& dst);
    wchar_t getWChar(int index);
    std::string getString(int index);
    std::wstring getWString(int index);
    double getDouble(int index);
    float getFloat(int index);
    int getInt(int index);
    long getLong(int index);
    int getMedium(int index);
    int getUnsignedMedium(int index);
    short getShort(int index);

    // --- read data methods --- //
    bool readBool();
    char readByte();
    ByteBuffer& readBytes(char* dst, int dstLength, int length);
    ByteBuffer& readBytes(char* dst, int length);
    ByteBuffer& readBytes(ByteBuffer& dst, int dstIndex, int length);
    ByteBuffer& readBytes(ByteBuffer& dst, int length);
    ByteBuffer& readBytes(ByteBuffer& dst);
    wchar_t readWChar();
    double readDouble();
    float readFloat();
    int readInt();
    long readLong();
    int readMedium();
    int readUnsignedMedium();
    short readShort();
    std::string readString();
    std::wstring readWstring();

    // --- set data methods --- //
    ByteBuffer& setBool(int index, bool value);
    ByteBuffer& setByte(int index, int value);
    ByteBuffer& setBytes(int index, char* src, int srcLength, int length);
    ByteBuffer& setBytes(int index, char* src, int srcLength);
    ByteBuffer& setBytes(int index, ByteBuffer& src);
    ByteBuffer& setBytes(int index, ByteBuffer& src, int length);
    ByteBuffer& setBytes(int index, ByteBuffer& src, int srcIndex, int length);
    ByteBuffer& setWChar(int index, int value);
    int setString(int index, std::string& value);
    int setWString(int index, std::wstring& value);
    ByteBuffer& setDouble(int index, double value);
    ByteBuffer& setFloat(int index, float value);
    ByteBuffer& setInt(int index, int value);
    ByteBuffer& setLong(int index, long value);
    ByteBuffer& setMedium(int index, int value);
    ByteBuffer& setShort(int index, int value);
    ByteBuffer& setZero(int index, int length);

    // --- write data methods --- //
    ByteBuffer& writeBool(bool value);
    ByteBuffer& writeByte(int value);
    ByteBuffer& writeBytes(char* src, int srcLength, int length);
    ByteBuffer& writeBytes(char* src, int srcLength);
    ByteBuffer& writeBytes(ByteBuffer& src);
    ByteBuffer& writeBytes(ByteBuffer& src, int length);
    ByteBuffer& writeBytes(ByteBuffer& src, int srcIndex, int length);
    ByteBuffer& writeWChar(int value);
    int writeString(std::string& value);
    int writeWString(std::wstring& value);
    ByteBuffer& writeDouble(double value);
    ByteBuffer& writeFloat(float value);
    ByteBuffer& writeInt(int value);
    ByteBuffer& writeLong(long value);
    ByteBuffer& writeMedium(int value);
    ByteBuffer& writeShort(int value);
    ByteBuffer& writeZero(int length);

    // --- operator<< --- //
    ByteBuffer& operator<<(bool value);
    ByteBuffer& operator<<(char value);
    ByteBuffer& operator<<(short value);
    ByteBuffer& operator<<(wchar_t value);
    ByteBuffer& operator<<(int value);
    ByteBuffer& operator<<(long value);
    ByteBuffer& operator<<(float __f);
    ByteBuffer& operator<<(double __f);
    ByteBuffer& operator<<(std::string __s);
    ByteBuffer& operator<<(std::wstring __s);

    // --- operator>> --- //
    ByteBuffer& operator>>(bool& value);
    ByteBuffer& operator>>(char& value);
    ByteBuffer& operator>>(short& value);
    ByteBuffer& operator>>(wchar_t& value);
    ByteBuffer& operator>>(int& value);
    ByteBuffer& operator>>(long& value);
    ByteBuffer& operator>>(float& value);
    ByteBuffer& operator>>(double& value);
    ByteBuffer& operator>>(std::string& value);
    ByteBuffer& operator>>(std::wstring& value);

protected:
    static bool checkAccessible;
    void ensureAccessible();
    void checkNewCapacity(const int newCapacity);
    void trimIndicesToCapacity(int newCapacity);
    char* allocateArray(int initialCapacity);
    void freeArray(char* array);
    void checkReadableBytes(int minimumReadableBytes);
    void checkIndex(int index, int fieldLength);
    void checkSrcIndex(int index, int length, int srcIndex, int srcCapacity);
    void checkDstIndex(int index, int length, int dstIndex, int dstCapacity);

private:
    static bool checkBounds;
    int maxCapacity;
    char* memory;
    int _readerIndex;
    int _writerIndex;
    int markedReaderIndex;
    int markedWriterIndex;
    void ensureWritable0(int minWritableBytes);
    void checkReadableBytes0(int minimumReadableBytes);
    void checkIndex0(int index, int fieldLength);
    static void checkIndexBounds(const int readerIndex, const int writerIndex, const int capacity);
    static void checkRangeBounds(const char *indexName, const int index, const int fieldLength, const int capacity);
    static void checkReadableBounds(ByteBuffer& src, const int length);
    friend  int lastIndexOf(ByteBuffer* buffer, int fromIndex, int toIndex, char value);
    friend int firstIndexOf(ByteBuffer* buffer, int fromIndex, int toIndex, char value);
};


#endif //__PROTOCOL_CODEC_BYTEBUFFER_H_
