//
// Created by NAVER on 2023/3/6.
//

#include "protocol/codec/CodecException.h"

namespace chat {
namespace protocol {
namespace codec {

CodecException::CodecException(const std::string& message)
        :Exception(message) {

}

CodecException::~CodecException() {

}

} /* namespace codec */
} /* namespace protocol */
} /* namespace chat */