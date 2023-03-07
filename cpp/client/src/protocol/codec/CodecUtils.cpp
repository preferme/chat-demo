//
// Created by NAVER on 2023/3/7.
//

#include "CodecUtils.h"
#include "protocol/codec/StringConverter.h"
#include <iostream>

static bool nativeEndianIsBigOrLittle() {
    union {short value; char array[2];} data;
    data.value = 0x4321;
    // BIG_ENDIAN
    if (data.array[0] == 0x43 && data.array[1] == 0x21) {
        return true;
    // LITTLE_ENDIAN
    } else if (data.array[0] == 0x21 && data.array[1] == 0x43) {
        return false;
    } else {
        std::cerr << "unsupported endian" << std::endl;
        throw std::runtime_error("unsupported endian");
    }
}

bool CodecUtils::isBigEndian = nativeEndianIsBigOrLittle();

byte CodecUtils::getByte(const byte* memory, const int index) {
    return memory[index];
}

short CodecUtils::getShort(const byte* memory, const int index) {
    return isBigEndian ? getShortBE(memory, index) : getShortLE(memory, index);
}
short CodecUtils::getShortBE(const byte* memory, const int index) {
    return (short) (memory[index] << 8 | memory[index + 1] & 0xFF);
}
short CodecUtils::getShortLE(const byte* memory, const int index){
    return (short) (memory[index] & 0xff | memory[index + 1] << 8);
}

int CodecUtils::getUnsignedMedium(const byte* memory, const int index){
    return isBigEndian ? getUnsignedMediumBE(memory, index) : getUnsignedMediumLE(memory, index);
}
int CodecUtils::getUnsignedMediumBE(const byte* memory, const int index){
    return  (memory[index]     & 0xff) << 16 |
            (memory[index + 1] & 0xff) <<  8 |
            memory[index + 2] & 0xff;
}
int CodecUtils::getUnsignedMediumLE(const byte* memory, const int index){
    return  memory[index]     & 0xff         |
            (memory[index + 1] & 0xff) <<  8 |
            (memory[index + 2] & 0xff) << 16;
}

int CodecUtils::getInt(const byte* memory, const int index){
    return isBigEndian ? getIntBE(memory, index) : getIntLE(memory, index);
}
int CodecUtils::getIntBE(const byte* memory, const int index){
    return  (memory[index]     & 0xff) << 24 |
            (memory[index + 1] & 0xff) << 16 |
            (memory[index + 2] & 0xff) <<  8 |
            memory[index + 3] & 0xff;
}
int CodecUtils::getIntLE(const byte* memory, const int index){
    return  memory[index]      & 0xff        |
            (memory[index + 1] & 0xff) << 8  |
            (memory[index + 2] & 0xff) << 16 |
            (memory[index + 3] & 0xff) << 24;
}

long CodecUtils::getLong(const byte* memory, const int index){
    return isBigEndian ? getLongBE(memory, index) : getLongLE(memory, index);
}
long CodecUtils::getLongBE(const byte* memory, const int index){
    return  ((long) memory[index]     & 0xff) << 56 |
            ((long) memory[index + 1] & 0xff) << 48 |
            ((long) memory[index + 2] & 0xff) << 40 |
            ((long) memory[index + 3] & 0xff) << 32 |
            ((long) memory[index + 4] & 0xff) << 24 |
            ((long) memory[index + 5] & 0xff) << 16 |
            ((long) memory[index + 6] & 0xff) <<  8 |
            (long) memory[index + 7] & 0xff;
}
long CodecUtils::getLongLE(const byte* memory, const int index){
    return  (long) memory[index]      & 0xff        |
            ((long) memory[index + 1] & 0xff) <<  8 |
            ((long) memory[index + 2] & 0xff) << 16 |
            ((long) memory[index + 3] & 0xff) << 24 |
            ((long) memory[index + 4] & 0xff) << 32 |
            ((long) memory[index + 5] & 0xff) << 40 |
            ((long) memory[index + 6] & 0xff) << 48 |
            ((long) memory[index + 7] & 0xff) << 56;
}

void CodecUtils::setByte(byte* const memory, const int index, int value){
    memory[index] = (byte) value;
}

void CodecUtils::setShort(byte* const memory, const int index, int value){
    return isBigEndian ? setShortBE(memory, index, value) : setShortLE(memory, index, value);
}
void CodecUtils::setShortBE(byte* const memory, const int index, int value){
    memory[index]     = (byte) ((unsigned short)value >> 8);
    memory[index + 1] = (byte) value;
}
void CodecUtils::setShortLE(byte* const memory, const int index, int value){
    memory[index]     = (byte) value;
    memory[index + 1] = (byte) ((unsigned short)value >> 8);
}

void CodecUtils::setMedium(byte* const memory, const int index, int value){
    return isBigEndian ? setMediumBE(memory, index, value) : setMediumLE(memory, index, value);
}
void CodecUtils::setMediumBE(byte* const memory, const int index, int value){
    memory[index]     = (byte) ((unsigned int)value >> 16);
    memory[index + 1] = (byte) ((unsigned int)value >> 8);
    memory[index + 2] = (byte) value;
}
void CodecUtils::setMediumLE(byte* const memory, const int index, int value){
    memory[index]     = (byte) value;
    memory[index + 1] = (byte) ((unsigned int)value >> 8);
    memory[index + 2] = (byte) ((unsigned int)value >> 16);
}

void CodecUtils::setInt(byte* const memory, const int index, int value){
    return isBigEndian ? setIntBE(memory, index, value) : setIntLE(memory, index, value);
}
void CodecUtils::setIntBE(byte* const memory, const int index, int value){
    memory[index]     = (byte) ((unsigned int)value >> 24);
    memory[index + 1] = (byte) ((unsigned int)value >> 16);
    memory[index + 2] = (byte) ((unsigned int)value >> 8);
    memory[index + 3] = (byte) value;
}
void CodecUtils::setIntLE(byte* const memory, const int index, int value){
    memory[index]     = (byte) value;
    memory[index + 1] = (byte) ((unsigned int)value >> 8);
    memory[index + 2] = (byte) ((unsigned int)value >> 16);
    memory[index + 3] = (byte) ((unsigned int)value >> 24);
}

void CodecUtils::setLong(byte* const memory, const int index, const long value){
    return isBigEndian ? setLongBE(memory, index, value) : setLongLE(memory, index, value);
}
void CodecUtils::setLongBE(byte* const memory, const int index, const long value){
    memory[index]     = (byte) ((unsigned long)value >> 56);
    memory[index + 1] = (byte) ((unsigned long)value >> 48);
    memory[index + 2] = (byte) ((unsigned long)value >> 40);
    memory[index + 3] = (byte) ((unsigned long)value >> 32);
    memory[index + 4] = (byte) ((unsigned long)value >> 24);
    memory[index + 5] = (byte) ((unsigned long)value >> 16);
    memory[index + 6] = (byte) ((unsigned long)value >> 8);
    memory[index + 7] = (byte) value;
}
void CodecUtils::setLongLE(byte* const memory, const int index, const long value){
    memory[index]     = (byte) value;
    memory[index + 1] = (byte) ((unsigned long)value >> 8);
    memory[index + 2] = (byte) ((unsigned long)value >> 16);
    memory[index + 3] = (byte) ((unsigned long)value >> 24);
    memory[index + 4] = (byte) ((unsigned long)value >> 32);
    memory[index + 5] = (byte) ((unsigned long)value >> 40);
    memory[index + 6] = (byte) ((unsigned long)value >> 48);
    memory[index + 7] = (byte) ((unsigned long)value >> 56);
}

std::string CodecUtils::getString(const byte* memory, const int index) {
    short length = getShort(memory, index);
    return std::string(memory+2, length);
}
void CodecUtils::setString(byte* const memory, const int index, const std::string& value) {
    for (int i = 0; i < value.length(); ++i) {
        memory[i] = value[i];
    }
}

std::wstring CodecUtils::getWString(const byte* memory, const int index){
    return ::chat::protocol::codec::to_wstring(getString(memory, index));
}
void CodecUtils::setWString(byte* const memory, const int index, const std::wstring& value){
    setString(memory, index, ::chat::protocol::codec::to_string(value));
}
