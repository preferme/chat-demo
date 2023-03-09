//
// Created by NAVER on 2023/3/6.
//

#ifndef __PROTOCOL_CODEC_CODECEXCEPTION_H_
#define __PROTOCOL_CODEC_CODECEXCEPTION_H_

#include "exception/Exception.h"
#include <string>

namespace chat {
namespace protocol {
namespace codec {


class CodecException : public exception::Exception {
public:
    explicit CodecException(const std::string& message);
    virtual ~CodecException();
};

} /* namespace codec */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_CODEC_CODECEXCEPTION_H_ */
