/*
 * RegistResponse.cpp
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#include "protocol/package/body/RegistResponse.h"

namespace chat {
namespace protocol {
namespace package {

RegistResponse::RegistResponse(): ChatResponse() {

}

RegistResponse::RegistResponse(int errorCode, std::wstring message)
		: ChatResponse(errorCode, message) {

}

RegistResponse::RegistResponse(ChatUser user)
		: ChatResponse(), user(user){

}

std::wostream& operator<<(std::wostream& os, const RegistResponse& response) {
	if(response.errorCode) {
		os << (ChatResponse)response;
	} else {
		os << "{user: " << response.user << "}";
	}
	return os;
}

std::wostream& operator<<(std::wostream& os, const std::unique_ptr<RegistResponse>& response) {
	return os << *response;
}


const ChatUser& RegistResponse::getUser() const {
	return user;
}

void RegistResponse::setUser(const ChatUser &user) {
	this->user = user;
}

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
