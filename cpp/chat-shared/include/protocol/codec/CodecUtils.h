/*
 * CodecUtils.h
 *
 *  Created on: 2023年3月7日
 *      Author: hou-lei
 */

#ifndef CPP_CODECUTILS_H
#define CPP_CODECUTILS_H

#include <exception>
#include <string>


namespace chat {
namespace protocol {
namespace codec {


typedef char byte;

class CodecUtils final {
public:
    static byte getByte(const byte* memory, const int index);
    static short getShort(const byte* memory, const int index);
    static short getShortBE(const byte* memory, const int index);
    static short getShortLE(const byte* memory, const int index);
    static int getUnsignedMedium(const byte* memory, const int index);
    static int getUnsignedMediumBE(const byte* memory, const int index);
    static int getUnsignedMediumLE(const byte* memory, const int index);
    static int getInt(const byte* memory, const int index);
    static int getIntBE(const byte* memory, const int index);
    static int getIntLE(const byte* memory, const int index);
    static long getLong(const byte* memory, const int index);
    static long getLongBE(const byte* memory, const int index);
    static long getLongLE(const byte* memory, const int index);
    static bool getBool(const byte* memory, const int index);
    static float getFloat(const byte* memory, const int index);
    static float getFloatBE(const byte* memory, const int index);
    static float getFloatLE(const byte* memory, const int index);
    static double getDouble(const byte* memory, const int index);
    static double getDoubleBE(const byte* memory, const int index);
    static double getDoubleLE(const byte* memory, const int index);

    static void setByte(byte* const memory, const int index, const int value);
    static void setShort(byte* const memory, const int index, const int value);
    static void setShortBE(byte* const memory, const int index, const int value);
    static void setShortLE(byte* const memory, const int index, const int value);
    static void setMedium(byte* const memory, const int index, const int value);
    static void setMediumBE(byte* const memory, const int index, const int value);
    static void setMediumLE(byte* const memory, const int index, const int value);
    static void setInt(byte* const memory, const int index, const int value);
    static void setIntBE(byte* const memory, const int index, const int value);
    static void setIntLE(byte* const memory, const int index, const int value);
    static void setLong(byte* const memory, const int index, const long value);
    static void setLongBE(byte* const memory, const int index, const long value);
    static void setLongLE(byte* const memory, const int index, const long value);
    static void setBool(byte* const memory, const int index, const bool value);
    static void setFloat(byte* const memory, const int index, const float value);
    static void setFloatBE(byte* const memory, const int index, const float value);
    static void setFloatLE(byte* const memory, const int index, const float value);
    static void setDouble(byte* const memory, const int index, const double value);
    static void setDoubleBE(byte* const memory, const int index, const double value);
    static void setDoubleLE(byte* const memory, const int index, const double value);

    static std::string getString(const byte* memory, const int index);
    static void setString(byte* const memory, const int index, const std::string& value);
    static std::wstring getWString(const byte* memory, const int index);
    static int setWString(byte* const memory, const int index, const std::wstring& value);

private:
    static bool isBigEndian;
    CodecUtils(){}
};

} /* namespace codec */
} /* namespace protocol */
} /* namespace chat */

#endif //CPP_CODECUTILS_H
