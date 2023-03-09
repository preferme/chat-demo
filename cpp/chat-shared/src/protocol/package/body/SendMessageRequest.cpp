/*
 * SendMessageRequest.cpp
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#include "protocol/package/body/SendMessageRequest.h"

namespace chat {
namespace protocol {
namespace package {

SendMessageRequest::SendMessageRequest() {

}

SendMessageRequest::SendMessageRequest(ChatUser user, std::wstring message)
		:user(user), message(message) {

}

std::wostream& operator<<(std::wostream& os, const SendMessageRequest& request) {
	return os << "{user: " << request.user << ", message: \"" << request.message << "\"}";
}

std::wostream& operator<<(std::wostream& os, const std::unique_ptr<SendMessageRequest>& request) {
	return os << *request;
}


const std::wstring& SendMessageRequest::getMessage() const {
	return message;
}

void SendMessageRequest::setMessage(const std::wstring &message) {
	this->message = message;
}

const ChatUser& SendMessageRequest::getUser() const {
	return user;
}

void SendMessageRequest::setUser(const ChatUser &user) {
	this->user = user;
}

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
