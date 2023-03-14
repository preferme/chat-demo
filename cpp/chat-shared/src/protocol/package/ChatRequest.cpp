/*
 * ChatRequest.cpp
 *
 *  Created on: 2023年3月8日
 *      Author: hou-lei
 */

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
