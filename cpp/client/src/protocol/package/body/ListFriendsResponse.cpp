/*
 * ListFriendsResponse.cpp
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#include "protocol/package/body/ListFriendsResponse.h"

namespace chat {
namespace protocol {
namespace package {

ListFriendsResponse::ListFriendsResponse()
		:ResponseBody(), totalCount(0), listSize(0) {

}

ListFriendsResponse::ListFriendsResponse(int errorCode, std::wstring message)
		:ResponseBody(errorCode, message), totalCount(0), listSize(0) {

}

ListFriendsResponse::ListFriendsResponse(int totalCount, int listSize, std::vector<ChatUser> friends)
		:ResponseBody(), totalCount(totalCount), listSize(listSize), friends(friends) {

}

std::wostream& operator<<(std::wostream& os, const ListFriendsResponse& response) {
	if(response.getErrorCode()) {
		os << (ResponseBody)response;
	} else {
		os << "{totalCount: " << response.getTotalCount();
		os << ", listSize: " << response.getListSize();
		os << ", friends: [";
		for(int i=0; response.friends.size(); i++) {
			if(i > 0) {
				os << ", ";
			}
			os << response.friends[i];
		}
		os << "]}";
	}
	return os;
}

std::wostream& operator<<(std::wostream& os, const std::unique_ptr<ListFriendsResponse>& response) {
	return os << *response;
}

const std::vector<ChatUser>& ListFriendsResponse::getFriends() const {
	return friends;
}

void ListFriendsResponse::setFriends(const std::vector<ChatUser> &friends) {
	this->friends = friends;
}

int ListFriendsResponse::getListSize() const {
	return listSize;
}

void ListFriendsResponse::setListSize(int listSize) {
	this->listSize = listSize;
}

int ListFriendsResponse::getTotalCount() const {
	return totalCount;
}

void ListFriendsResponse::setTotalCount(int totalCount) {
	this->totalCount = totalCount;
}

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
