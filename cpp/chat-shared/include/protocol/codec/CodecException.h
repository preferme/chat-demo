/*
 * CodecException.h
 *
 *  Created on: 2023年3月6日
 *      Author: hou-lei
 */
#ifndef __PROTOCOL_CODEC_CODECEXCEPTION_H_
#define __PROTOCOL_CODEC_CODECEXCEPTION_H_

#include "exception/exception.hpp"
#include <string>

namespace chat {
namespace protocol {
namespace codec {


class CodecException : public exception::exception {
public:
    explicit CodecException(const std::string& message);
    virtual ~CodecException();
};

} /* namespace codec */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_CODEC_CODECEXCEPTION_H_ */
