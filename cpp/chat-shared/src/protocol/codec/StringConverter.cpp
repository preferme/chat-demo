/*
 * CharacterEncodingConverter.cpp
 *
 *  Created on: 2023年3月6日
 *      Author: hou-lei
 */

#include "protocol/codec/StringConverter.h"

namespace chat {
namespace protocol {
namespace codec {

std::wstring to_wstring(const std::string& input) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(input);
}

std::string to_string(const std::wstring& input) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(input);
}

StringConverter::StringConverter() {
	// TODO Auto-generated constructor stub

}

StringConverter::~StringConverter() {
	// TODO Auto-generated destructor stub
}

} /* namespace codec */
} /* namespace protocol */
} /* namespace chat */
