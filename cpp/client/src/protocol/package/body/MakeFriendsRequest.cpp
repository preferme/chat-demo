/*
 * MakeFriendsRequest.cpp
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#include "protocol/package/body/MakeFriendsRequest.h"

namespace chat {
namespace protocol {
namespace package {

MakeFriendsRequest::MakeFriendsRequest() {

}

MakeFriendsRequest::MakeFriendsRequest(ChatUser friends)
		:friends(friends){

}

std::wostream& operator<<(std::wostream& os, const MakeFriendsRequest& request) {
	return os << "{friends: " << request.friends << "}";
}

std::wostream& operator<<(std::wostream& os, const std::unique_ptr<MakeFriendsRequest>& request) {
	return os << *request;
}

const ChatUser& MakeFriendsRequest::getFriends() const {
	return friends;
}

void MakeFriendsRequest::setFriends(const ChatUser &friends) {
	this->friends = friends;
}

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
