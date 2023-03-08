//
// Created by NAVER on 2023/3/8.
//

#include "protocol/package/ChatRequest.h"

namespace chat {
namespace protocol {
namespace package {

ChatRequest::ChatRequest() {

}

ChatRequest::~ChatRequest() {

}

std::wostream& operator<<(std::wostream& os, const ChatRequest& body) {
    return os << "{}";
}

std::wostream& operator<<(std::wostream& os, const std::unique_ptr<ChatRequest>& body) {
    return os << *body;
}

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
