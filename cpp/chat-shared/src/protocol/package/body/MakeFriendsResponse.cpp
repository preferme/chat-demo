/*
 * MakeFriendsResponse.cpp
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#include "protocol/package/body/MakeFriendsResponse.h"

namespace chat {
namespace protocol {
namespace package {

MakeFriendsResponse::MakeFriendsResponse(): ChatResponse() {

}

MakeFriendsResponse::MakeFriendsResponse(int errorCode, std::wstring message)
		: ChatResponse(errorCode, message) {

}

MakeFriendsResponse::MakeFriendsResponse(ChatUser friends)
	: ChatResponse(), friends(friends){

}

std::wostream& operator<<(std::wostream& os, const MakeFriendsResponse& response) {
	if(response.errorCode) {
		os << (ChatResponse)response;
	} else {
		os << "{friends: " << response.friends << "}";
	}
	return os;
}

std::wostream& operator<<(std::wostream& os, const std::unique_ptr<MakeFriendsResponse>& response) {
	return os << *response;
}

const ChatUser& MakeFriendsResponse::getFriends() const {
	return friends;
}

void MakeFriendsResponse::setFriends(const ChatUser &friends) {
	this->friends = friends;
}

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
