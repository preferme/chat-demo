/*
 * PackageCodec.cpp
 *
 *  Created on: 2023年3月6日
 *      Author: hou-lei
 */

#include "protocol/codec/CodecException.h"

namespace chat {
namespace protocol {
namespace codec {

CodecException::CodecException(const std::string& message)
        :exception::exception(message) {

}

CodecException::~CodecException() {

}

} /* namespace codec */
} /* namespace protocol */
} /* namespace chat */