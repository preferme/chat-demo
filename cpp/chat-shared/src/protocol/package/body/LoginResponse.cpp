/*
 * LoginResponse.cpp
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#include "protocol/package/body/LoginResponse.h"

namespace chat {
namespace protocol {
namespace package {

LoginResponse::LoginResponse(): ChatResponse() {

}

LoginResponse::LoginResponse(int errorCode, std::wstring message)
		: ChatResponse(errorCode, message) {

}

LoginResponse::LoginResponse(ChatUser user)
		: ChatResponse(), user(user){

}

std::wostream& operator<<(std::wostream& os, const LoginResponse& response) {
	if(response.errorCode) {
		os << (ChatResponse)response;
	} else {
		os << "{user: " << response.user << "}";
	}
	return os;
}

std::wostream& operator<<(std::wostream& os, const std::unique_ptr<LoginResponse>& response) {
	return os << *response;
}

const ChatUser& LoginResponse::getUser() const {
	return user;
}

void LoginResponse::setUser(const ChatUser &user) {
	this->user = user;
}

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
