//
// Created by NAVER on 2023/3/6.
//

#include "ByteBuffer.h"
#include "CodecUtils.h"
#include "protocol/codec/StringConverter.h"
#include <sstream>
#include <algorithm>
#include <cstdio>

#define INT_ZERO (0)
#define LONG_ZERO (0L)
#define FLOAT_ZERO (0.0f)
#define DOUBLE_ZERO (0.0)

// 4 MiB page
#define CALCULATE_THRESHOLD (1048576 * 4)

#define isOutOfBounds(index, length, capacity) \
(((index) | (length) | (capacity) | ((index) + (length)) | ((capacity) - ((index) + (length)))) < 0)

static int checkPositiveOrZero(const int i, std::string name) {
    if (i < INT_ZERO) {
        std::stringstream ss;
        ss << name << " : " << i << " (expected: >= 0)";
        throw std::invalid_argument(ss.str());
    }
    return i;
}
static long checkPositiveOrZero(const long i, std::string name) {
    if (i < LONG_ZERO) {
        std::stringstream ss;
        ss << name << " : " << i << " (expected: >= 0)";
        throw std::invalid_argument(ss.str());
    }
    return i;
}
static float checkPositiveOrZero(const float i, std::string name) {
    if (i < FLOAT_ZERO) {
        std::stringstream ss;
        ss << name << " : " << i << " (expected: >= 0)";
        throw std::invalid_argument(ss.str());
    }
    return i;
}
static double checkPositiveOrZero(const double i, std::string name) {
    if (i < DOUBLE_ZERO) {
        std::stringstream ss;
        ss << name << " : " << i << " (expected: >= 0)";
        throw std::invalid_argument(ss.str());
    }
    return i;
}

static int numberOfLeadingZeros(int i) {
    // HD, Count leading 0's
    if (i <= 0)
        return i == 0 ? 32 : 0;
    int n = 31;
    if (i >= 1 << 16) { n -= 16; i = ((unsigned int)i) >> 16; }
    if (i >= 1 <<  8) { n -=  8; i = ((unsigned int)i) >>  8; }
    if (i >= 1 <<  4) { n -=  4; i = ((unsigned int)i) >>  4; }
    if (i >= 1 <<  2) { n -=  2; i = ((unsigned int)i) >>  2; }
    return n - (((unsigned int)i) >> 1);
}
static int findNextPositivePowerOfTwo(const int value) {
    assert(value > 0x80000000 && value < 0x40000000);
    return 1 << (32 - numberOfLeadingZeros(value - 1));
}
static int calculateNewCapacity(int minNewCapacity, int maxCapacity) {
    checkPositiveOrZero(minNewCapacity, "minNewCapacity");
    if (minNewCapacity > maxCapacity) {
        char message[128]={0};
        std::snprintf(message, 128,
                      "minNewCapacity: %d (expected: not greater than maxCapacity(%d)",
                      minNewCapacity, maxCapacity);
        throw std::invalid_argument(message);
    }
    const int threshold = CALCULATE_THRESHOLD; // 4 MiB page

    if (minNewCapacity == threshold) {
        return threshold;
    }

    // If over threshold, do not double but just increase by threshold.
    if (minNewCapacity > threshold) {
        int newCapacity = minNewCapacity / threshold * threshold;
        if (newCapacity > maxCapacity - threshold) {
            newCapacity = maxCapacity;
        } else {
            newCapacity += threshold;
        }
        return newCapacity;
    }

    // 64 <= newCapacity is a power of 2 <= threshold
    const int newCapacity = findNextPositivePowerOfTwo(std::max(minNewCapacity, 64));
    return std::min(newCapacity, maxCapacity);
}
int firstIndexOf(ByteBuffer* buffer, int fromIndex, int toIndex, char value) {
    fromIndex = std::max(fromIndex, 0);
    if (fromIndex >= toIndex || buffer->capacity() == 0) {
        return -1;
    }
    const int length = toIndex - fromIndex;
    buffer->checkIndex(fromIndex, length);
    // if (!PlatformDependent.isUnaligned()) { ... }
    for (int i = fromIndex; i < toIndex; i++) {
        if (CodecUtils::getByte(buffer->memory, i) == value) {
            return i;
        }
    }
    // else { ... }
    return -1;
}
int lastIndexOf(ByteBuffer* buffer, int fromIndex, int toIndex, char value) {
    assert(fromIndex > toIndex);
    const int capacity = buffer->capacity();
    fromIndex = std::min(fromIndex, capacity);
    if (fromIndex < 0 || capacity == 0) {
        return -1;
    }
    buffer->checkIndex(toIndex, fromIndex - toIndex);
    for (int i = fromIndex - 1; i >= toIndex; i--) {
        if (CodecUtils::getByte(buffer->memory, i) == value) {
            return i;
        }
    }

    return -1;
}


bool ByteBuffer::checkAccessible = true;
bool ByteBuffer::checkBounds = true;

ByteBuffer::ByteBuffer(const int capacity)
        : maxCapacity(capacity), memory(allocateArray(capacity)), _readerIndex(0), _writerIndex(0), markedReaderIndex(0), markedWriterIndex(0) {

}

ByteBuffer::~ByteBuffer() {
    freeArray(this->memory);
}

std::ostream& operator<<(std::ostream& os, const ByteBuffer& buffer) {
    if (buffer.memory == nullptr) {
        return os << "ByteBuffer(freed)";
    }
    os << "ByteBuffer(ridx: " << buffer._readerIndex;
    os << ", widx: " << buffer._writerIndex;
    os << ", cap: " << buffer.maxCapacity;
    os << ')';
    return os;
}


// --- protected methods ---//

void ByteBuffer::ensureAccessible() {
    if (ByteBuffer::checkAccessible && this->memory == nullptr) {
        throw std::runtime_error("Illegal Memory Alloc");
    }
}
void ByteBuffer::checkNewCapacity(const int newCapacity) {
    ensureAccessible();
    if (checkBounds && (newCapacity < 0 || newCapacity > capacity())) {
        std::stringstream ss;
        ss << "newCapacity: " << newCapacity << " (expected: 0-" << capacity() << ')';
        throw std::invalid_argument(ss.str());
    }
}
// Called after a capacity reduction
void ByteBuffer::trimIndicesToCapacity(int newCapacity) {
    if (writerIndex() > newCapacity) {
        _readerIndex = std::min(readerIndex(), newCapacity);
        _writerIndex = newCapacity;
    }
}
char* ByteBuffer::allocateArray(int initialCapacity) {
    return new char[initialCapacity];
}
void ByteBuffer::freeArray(char* array) {
    if (array) {
        delete[] array;
    }
}
void ByteBuffer::checkReadableBytes(int minimumReadableBytes) {
    checkReadableBytes0(checkPositiveOrZero(minimumReadableBytes, "minimumReadableBytes"));
}
void ByteBuffer::checkIndex(int index, int fieldLength) {
    ensureAccessible();
    checkIndex0(index, fieldLength);
}
void ByteBuffer::checkSrcIndex(int index, int length, int srcIndex, int srcCapacity) {
    checkIndex(index, length);
    if (checkBounds) {
        checkRangeBounds("srcIndex", srcIndex, length, srcCapacity);
    }
}
void ByteBuffer::checkDstIndex(int index, int length, int dstIndex, int dstCapacity) {
    checkIndex(index, length);
    if (checkBounds) {
        checkRangeBounds("dstIndex", dstIndex, length, dstCapacity);
    }
}

// --- private methods --- //
void ByteBuffer::ensureWritable0(int minWritableBytes) {
    const int writerIndex = this->writerIndex();
    const int targetCapacity = writerIndex + minWritableBytes;
    // using non-short-circuit & to reduce branching - this is a hot path and targetCapacity should rarely overflow
    if (targetCapacity >= 0 & targetCapacity <= capacity()) {
        ensureAccessible();
        return;
    }
    if (checkBounds && (targetCapacity < 0 || targetCapacity > maxCapacity)) {
        ensureAccessible();
        std::stringstream ss;
        ss << "writerIndex(" << writerIndex << ") + minWritableBytes(" << minWritableBytes;
        ss << ") exceeds maxCapacity(" << maxCapacity << "): " << *this;
        throw std::invalid_argument(ss.str());
    }

    // Normalize the target capacity to the power of 2.
    const int fastWritable = /*maxFastWritableBytes()*/writableBytes();
    int newCapacity = fastWritable >= minWritableBytes ? writerIndex + fastWritable
                                                       : calculateNewCapacity(targetCapacity, maxCapacity);

    // Adjust to the new capacity.
    capacity(newCapacity);
}
void ByteBuffer::checkReadableBytes0(int minimumReadableBytes){
    ensureAccessible();
    if (checkBounds && _readerIndex > _writerIndex - minimumReadableBytes) {
        std::stringstream ss;
        ss << "readerIndex(" << _readerIndex << ") + length(" << minimumReadableBytes;
        ss << ") exceeds writerIndex(" << _writerIndex << "): " << *this;
        throw std::out_of_range(ss.str());
    }
}
void ByteBuffer::checkIndex0(int index, int fieldLength) {
    if (checkBounds) {
        checkRangeBounds("index", index, fieldLength, capacity());
    }
}
void ByteBuffer::checkIndexBounds(const int readerIndex, const int writerIndex, const int capacity) {
    if (readerIndex < 0 || readerIndex > writerIndex || writerIndex > capacity) {
        char msg[256] = {0};
        std::snprintf(msg, 256,
                      "readerIndex: %d, writerIndex: %d (expected: 0 <= readerIndex <= writerIndex <= capacity(%d))",
                      readerIndex, writerIndex, capacity);
        throw std::out_of_range(msg);
    }
}
void ByteBuffer::checkRangeBounds(const char* indexName, const int index, const int fieldLength, const int capacity) {
    if (isOutOfBounds(index, fieldLength, capacity)) {
        char msg[128] = {0};
        std::snprintf(msg, 128,  "%s: %d, length: %d (expected: range(0, %d))", indexName, index, fieldLength, capacity);
        throw std::out_of_range(msg);
    }
}
void ByteBuffer::checkReadableBounds(ByteBuffer& src, const int length) {
    if (length > src.readableBytes()) {
        std::stringstream ss;
        ss << "length(" << length << ") exceeds src.readableBytes(" << src.readableBytes() << ") where src is: " << src;
        throw std::out_of_range(ss.str());
    }
}

// --- public methods --- //

char* ByteBuffer::array() {
    ensureAccessible();
    return memory;
}

int ByteBuffer::capacity() {
    return maxCapacity;
}
ByteBuffer& ByteBuffer::capacity(int newCapacity) {
    checkNewCapacity(newCapacity);
    byte* oldArray = this->memory;
    int oldCapacity = this->maxCapacity;
    if (newCapacity == oldCapacity) {
        return *this;
    }

    int bytesToCopy;
    if (newCapacity > oldCapacity) {
        bytesToCopy = oldCapacity;
    } else {
        trimIndicesToCapacity(newCapacity);
        bytesToCopy = newCapacity;
    }
    byte* newArray = allocateArray(newCapacity);
    std::memcpy(newArray, oldArray, bytesToCopy);
    this->memory = newArray;
    freeArray(oldArray);
    return *this;
}
ByteBuffer& ByteBuffer::clear() {
    _readerIndex = _writerIndex = 0;
    return *this;
}
ByteBuffer& ByteBuffer::ensureWritable(int minWritableBytes) {
    ensureWritable0(checkPositiveOrZero(minWritableBytes, "minWritableBytes"));
    return *this;
}
int ByteBuffer::ensureWritable(int minWritableBytes, bool force) {
    ensureAccessible();
    checkPositiveOrZero(minWritableBytes, "minWritableBytes");

    if (minWritableBytes <= writableBytes()) {
        return 0;
    }

    const int maxCapacity = this->capacity();
    const int writerIndex = this->writerIndex();
    if (minWritableBytes > maxCapacity - writerIndex) {
        if (!force || capacity() == maxCapacity) {
            return 1;
        }

        capacity(maxCapacity);
        return 3;
    }

    int fastWritable = /*maxFastWritableBytes();*/writableBytes();
    int newCapacity = fastWritable >= minWritableBytes ? writerIndex + fastWritable
                                                       : calculateNewCapacity(writerIndex + minWritableBytes, maxCapacity);

    // Adjust to the new capacity.
    capacity(newCapacity);
    return 2;
}
int ByteBuffer::indexOf(int fromIndex, int toIndex, char value) {
    if (fromIndex <= toIndex) {
        return firstIndexOf(this, fromIndex, toIndex, value);
    }
    return lastIndexOf(this, fromIndex, toIndex, value);
}
bool ByteBuffer::isReadable(int numBytes) {
    return _writerIndex - _readerIndex >= numBytes;
}
bool ByteBuffer::isWritable(int numBytes) {
    return capacity() - _writerIndex >= numBytes;
}
ByteBuffer& ByteBuffer::markReaderIndex() {
    markedReaderIndex = _readerIndex;
    return *this;
}
ByteBuffer& ByteBuffer::markWriterIndex() {
    markedWriterIndex = _writerIndex;
    return *this;
}
int ByteBuffer::readableBytes() {
    return _writerIndex - _readerIndex;
}
int ByteBuffer::readerIndex() {
    return _readerIndex;
}
ByteBuffer& ByteBuffer::readerIndex(int readerIndex) {
    if (checkBounds) {
        checkIndexBounds(readerIndex, _writerIndex, capacity());
    }
    _readerIndex = readerIndex;
    return *this;
}
ByteBuffer& ByteBuffer::resetReaderIndex() {
    readerIndex(markedReaderIndex);
    return *this;
}
ByteBuffer& ByteBuffer::resetWriterIndex() {
    writerIndex(markedWriterIndex);
    return *this;
}
ByteBuffer& ByteBuffer::setIndex(int readerIndex, int writerIndex) {
    if (checkBounds) {
        checkIndexBounds(readerIndex, writerIndex, capacity());
    }
    _readerIndex = readerIndex;
    _writerIndex = writerIndex;
    return *this;
}
ByteBuffer& ByteBuffer::skipBytes(int length) {
    checkReadableBytes(length);
    _readerIndex += length;
    return *this;
}
int ByteBuffer::writableBytes() {
    return capacity() - _writerIndex;
}
int ByteBuffer::writerIndex() {
    return _writerIndex;
}
ByteBuffer& ByteBuffer::writerIndex(int writerIndex) {
    if (checkBounds) {
        checkIndexBounds(_readerIndex, writerIndex, capacity());
    }
    _writerIndex = writerIndex;
    return *this;
}

// --- get data methods --- //
bool ByteBuffer::getBool(int index) {
    checkIndex(index, 1);
    return CodecUtils::getBool(memory, index);
}
char ByteBuffer::getByte(int index) {
    checkIndex(index, 1);
    return CodecUtils::getByte(memory, index);
}
ByteBuffer& ByteBuffer::getBytes(int index, char* dst, int dstLength, int length) {
    checkDstIndex(index, length, 0, dstLength);
    std::memcpy(dst, memory, length);
    return *this;
}
ByteBuffer& ByteBuffer::getBytes(int index, char* dst, int length) {
    return getBytes(index, dst, length, length);
}
ByteBuffer& ByteBuffer::getBytes(int index, ByteBuffer& dst, int dstIndex, int length) {
    checkDstIndex(index, length, dstIndex, dst.capacity());
    dst.setBytes(dstIndex, memory, index, length);
    return *this;
}
ByteBuffer& ByteBuffer::getBytes(int index, ByteBuffer& dst, int length) {
    getBytes(index, dst, dst.writerIndex(), length);
    dst.writerIndex(dst.writerIndex() + length);
    return *this;
}
ByteBuffer& ByteBuffer::getBytes(int index, ByteBuffer& dst) {
    return getBytes(index, dst, dst.writableBytes());
}
wchar_t ByteBuffer::getWChar(int index) {
    return (wchar_t )getShort(index);
}
std::string ByteBuffer::getString(int index) {
    short length = CodecUtils::getShort(memory, index);
    checkIndex(index, length);
    return CodecUtils::getString(memory, index);
}
std::wstring ByteBuffer::getWString(int index) {
    short length = CodecUtils::getShort(memory, index);
    checkIndex(index, length);
    return CodecUtils::getWString(memory, index);
}
double ByteBuffer::getDouble(int index) {
    checkIndex(index, 8);
    return CodecUtils::getDouble(memory, index);
}
float ByteBuffer::getFloat(int index) {
    checkIndex(index, 4);
    return CodecUtils::getFloat(memory, index);
}
int ByteBuffer::getInt(int index) {
    checkIndex(index, 4);
    return CodecUtils::getInt(memory, index);
}
long ByteBuffer::getLong(int index) {
    checkIndex(index, 8);
    return CodecUtils::getLong(memory, index);
}
int ByteBuffer::getMedium(int index) {
    int value = CodecUtils::getUnsignedMedium(memory, index);
    if ((value & 0x800000) != 0) {
        value |= 0xff000000;
    }
    return value;
}
int ByteBuffer::getUnsignedMedium(int index) {
    checkIndex(index, 3);
    return CodecUtils::getUnsignedMedium(memory, index);
}
short ByteBuffer::getShort(int index) {
    checkIndex(index, 2);
    return CodecUtils::getShort(memory, 2);
}

// --- read data methods --- //
bool ByteBuffer::readBool() {
    checkReadableBytes0(1);
    byte b = CodecUtils::getBool(memory, _readerIndex);
    _readerIndex += 1;
    return b;
}
char ByteBuffer::readByte() {
    checkReadableBytes0(1);
    byte b = CodecUtils::getByte(memory, _readerIndex);
    _readerIndex += 1;
    return b;
}
ByteBuffer& ByteBuffer::readBytes(char* dst, int dstLength, int length) {
    checkReadableBytes(length);
    getBytes(_readerIndex, dst, dstLength, length);
    _readerIndex += length;
    return *this;
}
ByteBuffer& ByteBuffer::readBytes(char* dst, int length) {
    return readBytes(dst, length, length);
}
ByteBuffer& ByteBuffer::readBytes(ByteBuffer& dst, int dstIndex, int length) {
    checkReadableBytes(length);
    getBytes(_readerIndex, dst, dstIndex, length);
    _readerIndex += length;
    return *this;
}
ByteBuffer& ByteBuffer::readBytes(ByteBuffer& dst, int length) {
    if (checkBounds) {
        if (length > dst.writableBytes()) {
            std::stringstream ss;
            ss << "length(" << length << ") exceeds dst.writableBytes(" << dst.writableBytes() << ") where dst is: " << dst;
            throw std::out_of_range(ss.str());
        }
    }
    readBytes(dst, dst.writerIndex(), length);
    dst.writerIndex(dst.writerIndex() + length);
    return *this;
}
ByteBuffer& ByteBuffer::readBytes(ByteBuffer& dst) {
    readBytes(dst, dst.writableBytes());
    return *this;
}
wchar_t ByteBuffer::readWChar() {
    return (wchar_t)readShort();
}
double ByteBuffer::readDouble() {
    checkReadableBytes0(8);
    double v = CodecUtils::getDouble(memory, _readerIndex);
    _readerIndex += 8;
    return v;
}
float ByteBuffer::readFloat() {
    checkReadableBytes0(4);
    float v = CodecUtils::getFloat(memory, _readerIndex);
    _readerIndex += 4;
    return v;
}
int ByteBuffer::readInt() {
    checkReadableBytes0(4);
    int v = CodecUtils::getInt(memory, _readerIndex);
    _readerIndex += 4;
    return v;
}
long ByteBuffer::readLong() {
    checkReadableBytes0(8);
    long v = CodecUtils::getLong(memory, _readerIndex);
    _readerIndex += 8;
    return v;
}
int ByteBuffer::readMedium() {
    int value = readUnsignedMedium();
    if ((value & 0x800000) != 0) {
        value |= 0xff000000;
    }
    return value;
}
int ByteBuffer::readUnsignedMedium() {
    checkReadableBytes0(3);
    int v = CodecUtils::getUnsignedMedium(memory, _readerIndex);
    _readerIndex += 3;
    return v;
}
short ByteBuffer::readShort() {
    checkReadableBytes0(2);
    short v = CodecUtils::getShort(memory, _readerIndex);
    _readerIndex += 2;
    return v;
}
std::string ByteBuffer::readString() {
    checkReadableBytes0(2);
    short length = CodecUtils::getShort(memory, _readerIndex);
    checkReadableBytes0(2 + length);
    std::string v = CodecUtils::getString(memory, _readerIndex);
    _readerIndex += 2+length;
    return v;
}
std::wstring ByteBuffer::readWstring() {
    checkReadableBytes0(2);
    short length = CodecUtils::getShort(memory, _readerIndex);
    checkReadableBytes0(2 + length);
    std::wstring v = CodecUtils::getWString(memory, _readerIndex);
    _readerIndex += 2+length;
    return v;
}

// --- set data methods --- //
ByteBuffer& ByteBuffer::setBool(int index, bool value) {
    checkIndex(index, 1);
    CodecUtils::setBool(memory, index, value);
    return *this;
}
ByteBuffer& ByteBuffer::setByte(int index, int value) {
    checkIndex(index, 1);
    CodecUtils::setByte(memory, index, value);
    return *this;
}
ByteBuffer& ByteBuffer::setBytes(int index, char* src, int srcLength, int length) {
    checkSrcIndex(index, length, 0, srcLength);
    std::memcpy(memory+index, src, length);
    return *this;
}
ByteBuffer& ByteBuffer::setBytes(int index, char* src, int srcLength) {
    return setBytes(index, src, srcLength, srcLength);
}
ByteBuffer& ByteBuffer::setBytes(int index, ByteBuffer& src, int srcIndex, int length) {
    checkSrcIndex(index, length, srcIndex, src.capacity());
    src.getBytes(srcIndex, memory, index, length);
    return *this;
}
ByteBuffer& ByteBuffer::setBytes(int index, ByteBuffer& src, int length) {
    checkIndex(index, length);
    if (checkBounds) {
        checkReadableBounds(src, length);
    }

    setBytes(index, src, src.readerIndex(), length);
    src.readerIndex(src.readerIndex() + length);
    return *this;
}
ByteBuffer& ByteBuffer::setBytes(int index, ByteBuffer& src) {
    setBytes(index, src, src.readableBytes());
    return *this;
}
ByteBuffer& ByteBuffer::setWChar(int index, int value) {
    return setShort(index, (wchar_t )value);
}
int ByteBuffer::setString(int index, std::string& value) {
    checkIndex(index, value.length());
    CodecUtils::setString(memory, index, value);
    return value.length();
}
int ByteBuffer::setWString(int index, std::wstring& value) {
    std::string utf8 = ::chat::protocol::codec::to_string(value);
    checkIndex(index, utf8.length());
    CodecUtils::setString(memory, index, utf8);
    return utf8.length();
}
ByteBuffer& ByteBuffer::setDouble(int index, double value) {
    checkIndex(index, 8);
    CodecUtils::setDouble(memory, index, value);
    return *this;
}
ByteBuffer& ByteBuffer::setFloat(int index, float value) {
    checkIndex(index, 4);
    CodecUtils::setFloat(memory, index, value);
    return *this;
}
ByteBuffer& ByteBuffer::setInt(int index, int value) {
    checkIndex(index, 4);
    CodecUtils::setInt(memory, index, value);
    return *this;
}
ByteBuffer& ByteBuffer::setLong(int index, long value) {
    checkIndex(index, 8);
    CodecUtils::setLong(memory, index, value);
    return *this;
}
ByteBuffer& ByteBuffer::setMedium(int index, int value) {
    checkIndex(index, 3);
    CodecUtils::setMedium(memory, index, value);
    return *this;
}
ByteBuffer& ByteBuffer::setShort(int index, int value) {
    checkIndex(index, 2);
    CodecUtils::setShort(memory, index, value);
    return *this;
}
ByteBuffer& ByteBuffer::setZero(int index, int length) {
    if (length == 0) {
        return *this;
    }

    checkIndex(index, length);

    int nLong = (unsigned int)length >> 3;
    int nBytes = length & 7;
    for (int i = nLong; i > 0; i --) {
        CodecUtils::setLong(memory, index, 0);
        index += 8;
    }
    if (nBytes == 4) {
        CodecUtils::setInt(memory, index, 0);
        // Not need to update the index as we not will use it after this.
    } else if (nBytes < 4) {
        for (int i = nBytes; i > 0; i --) {
            CodecUtils::setByte(memory, index, 0);
            index ++;
        }
    } else {
        CodecUtils::setInt(memory, index, 0);
        index += 4;
        for (int i = nBytes - 4; i > 0; i --) {
            CodecUtils::setByte(memory, index, 0);
            index ++;
        }
    }
    return *this;
}

// --- write data methods --- //
ByteBuffer& ByteBuffer::writeBool(bool value) {
    ensureWritable0(1);
    CodecUtils::setBool(memory, _writerIndex++, value);
    return *this;
}
ByteBuffer& ByteBuffer::writeByte(int value) {
    ensureWritable0(1);
    CodecUtils::setByte(memory, _writerIndex++, value);
    return *this;
}
ByteBuffer& ByteBuffer::writeBytes(char* src, int srcLength, int length) {
    ensureWritable(length);
    setBytes(_writerIndex, src, srcLength, length);
    _writerIndex += length;
    return *this;
}
ByteBuffer& ByteBuffer::writeBytes(char* src, int srcLength) {
    return writeBytes(src, srcLength, srcLength);
}
ByteBuffer& ByteBuffer::writeBytes(ByteBuffer& src, int srcIndex, int length) {
    ensureWritable(length);
    setBytes(_writerIndex, src, srcIndex, length);
    _writerIndex += length;
    return *this;
}
ByteBuffer& ByteBuffer::writeBytes(ByteBuffer& src, int length) {
    if (checkBounds) {
        checkReadableBounds(src, length);
    }
    writeBytes(src, src.readerIndex(), length);
    src.readerIndex(src.readerIndex() + length);
    return *this;
}
ByteBuffer& ByteBuffer::writeBytes(ByteBuffer& src) {
    writeBytes(src, src.readableBytes());
    return *this;
}
ByteBuffer& ByteBuffer::writeWChar(int value) {
    writeShort(value);
    return *this;
}
int ByteBuffer::writeString(std::string& value) {
    ensureWritable0( value.length());
    checkIndex0(_writerIndex,  value.length());
    CodecUtils::setString(memory, _writerIndex, value);
    _writerIndex += value.length();
    return value.length();
}
int ByteBuffer::writeWString(std::wstring& value) {
    std::string utf8 = ::chat::protocol::codec::to_string(value);
    ensureWritable0(utf8.length());
    checkIndex0(_writerIndex,  utf8.length());
    CodecUtils::setString(memory, _writerIndex, utf8);
    _writerIndex += utf8.length();
    return utf8.length();
}
ByteBuffer& ByteBuffer::writeDouble(double value) {
    ensureWritable0(8);
    CodecUtils::setDouble(memory, _writerIndex, value);
    _writerIndex += 8;
    return *this;
}
ByteBuffer& ByteBuffer::writeFloat(float value) {
    ensureWritable0(4);
    CodecUtils::setFloat(memory, _writerIndex, value);
    _writerIndex += 4;
    return *this;
}
ByteBuffer& ByteBuffer::writeInt(int value) {
    ensureWritable0(4);
    CodecUtils::setInt(memory, _writerIndex, value);
    _writerIndex += 4;
    return *this;
}
ByteBuffer& ByteBuffer::writeLong(long value) {
    ensureWritable0(8);
    CodecUtils::setLong(memory, _writerIndex, value);
    _writerIndex += 8;
    return *this;
}
ByteBuffer& ByteBuffer::writeMedium(int value) {
    ensureWritable0(3);
    CodecUtils::setMedium(memory, _writerIndex, value);
    _writerIndex += 3;
    return *this;
}
ByteBuffer& ByteBuffer::writeShort(int value) {
    ensureWritable0(2);
    CodecUtils::setShort(memory, _writerIndex, value);
    _writerIndex += 2;
    return *this;
}
ByteBuffer& ByteBuffer::writeZero(int length) {
    if (length == 0) {
        return *this;
    }

    ensureWritable(length);
    int wIndex = _writerIndex;
    checkIndex0(wIndex, length);

    int nLong = ((unsigned int)length) >> 3;
    int nBytes = length & 7;
    for (int i = nLong; i > 0; i --) {
        CodecUtils::setLong(memory, wIndex, 0);
        wIndex += 8;
    }
    if (nBytes == 4) {
        CodecUtils::setInt(memory, wIndex, 0);
        wIndex += 4;
    } else if (nBytes < 4) {
        for (int i = nBytes; i > 0; i --) {
            CodecUtils::setByte(memory, wIndex, 0);
            wIndex++;
        }
    } else {
        CodecUtils::setInt(memory, wIndex, 0);
        wIndex += 4;
        for (int i = nBytes - 4; i > 0; i --) {
            CodecUtils::setByte(memory, wIndex, 0);
            wIndex++;
        }
    }
    _writerIndex = wIndex;
    return *this;
}

// --- operator<< --- //
ByteBuffer& ByteBuffer::operator<<(bool value) {
    return this->writeBool(value);
}
ByteBuffer& ByteBuffer::operator<<(char value) {
    return this->writeByte(value);
}
ByteBuffer& ByteBuffer::operator<<(short value) {
    return this->writeShort(value);
}
ByteBuffer& ByteBuffer::operator<<(wchar_t value) {
    return this->writeWChar(value);
}
ByteBuffer& ByteBuffer::operator<<(int value) {
    return this->writeInt(value);
}
ByteBuffer& ByteBuffer::operator<<(long value) {
    return this->writeLong(value);
}
ByteBuffer& ByteBuffer::operator<<(float value) {
    return this->writeFloat(value);
}
ByteBuffer& ByteBuffer::operator<<(double value) {
    return this->writeDouble(value);
}
ByteBuffer& ByteBuffer::operator<<(std::string value) {
    this->writeString(value);
    return *this;
}
ByteBuffer& ByteBuffer::operator<<(std::wstring value) {
    this->writeWString(value);
    return *this;
}

// --- operator>> --- //
ByteBuffer& ByteBuffer::operator>>(bool& value) {
    value = this->readBool();
    return *this;
}
ByteBuffer& ByteBuffer::operator>>(char& value) {
    value = this->readByte();
    return *this;
}
ByteBuffer& ByteBuffer::operator>>(short& value) {
    value = this->readShort();
    return *this;
}
ByteBuffer& ByteBuffer::operator>>(wchar_t& value) {
    value = this->readWChar();
    return *this;
}
ByteBuffer& ByteBuffer::operator>>(int& value) {
    value = this->readInt();
    return *this;
}
ByteBuffer& ByteBuffer::operator>>(long& value) {
    value = this->readLong();
    return *this;
}
ByteBuffer& ByteBuffer::operator>>(float& value) {
    value = this->readFloat();
    return *this;
}
ByteBuffer& ByteBuffer::operator>>(double& value) {
    value = this->readDouble();
    return *this;
}
ByteBuffer& ByteBuffer::operator>>(std::string& value) {
    value = this->readString();
    return *this;
}
ByteBuffer& ByteBuffer::operator>>(std::wstring& value) {
    value = this->readWstring();
    return *this;
}