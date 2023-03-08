/*
 * ChatResponse.cpp
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#include "protocol/package/ChatResponse.h"

namespace chat {
namespace protocol {
namespace package {

ChatResponse::ChatResponse(): errorCode(0) {

}

ChatResponse::ChatResponse(int errorCode, std::wstring message)
		:errorCode(errorCode), message(message) {

}

ChatResponse::~ChatResponse() {

}

std::wostream& operator<<(std::wostream& os, const ChatResponse& body) {
	return os << L"{errorCode: " << body.getErrorCode() << L", message: \"" << body.getMessage() << L"\"}";
}

std::wostream& operator<<(std::wostream& os, const std::unique_ptr<ChatResponse>& body) {
	return os << *body;
}

int ChatResponse::getErrorCode() const {
	return errorCode;
}

void ChatResponse::setErrorCode(int errorCode) {
	this->errorCode = errorCode;
}

const std::wstring& ChatResponse::getMessage() const {
	return message;
}

void ChatResponse::setMessage(const std::wstring &message) {
	this->message = message;
}

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
