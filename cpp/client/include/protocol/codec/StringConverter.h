/*
 * CharacterEncodingConverter.h
 *
 *  Created on: 2023年3月6日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_CODEC_STRING_CONVERTER_H_
#define __PROTOCOL_CODEC_STRING_CONVERTER_H_

#include <string>
#include <locale>
#include <codecvt>

namespace chat {
namespace protocol {
namespace codec {


std::wstring to_wstring(const std::string& input);

std::string to_string(const std::wstring& input);



class StringConverter final {
public:
	StringConverter();
	virtual ~StringConverter();
};


} /* namespace codec */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_CODEC_STRING_CONVERTER_H_ */
