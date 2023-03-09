/*
 * ListFriendsResponse.h
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_PACKAGE_BODY_LISTFRIENDSRESPONSE_H_
#define __PROTOCOL_PACKAGE_BODY_LISTFRIENDSRESPONSE_H_

#include <protocol/package/ChatResponse.h>
#include <protocol/package/body/ChatUser.h>
#include <vector>
#include <iostream>
#include <memory>

namespace chat {
namespace protocol {
namespace package {

class ListFriendsResponse final : public ChatResponse {
public:
	ListFriendsResponse();
	ListFriendsResponse(int errorCode, std::wstring message);
	ListFriendsResponse(int totalCount, int listSize, std::vector<ChatUser> friends);

	friend std::wostream& operator<<(std::wostream& os, const ListFriendsResponse& response);
	friend std::wostream& operator<<(std::wostream& os, const std::unique_ptr<ListFriendsResponse>& response);

	const std::vector<ChatUser>& getFriends() const;
	void setFriends(const std::vector<ChatUser> &friends);
	int getListSize() const;
	void setListSize(int listSize);
	int getTotalCount() const;
	void setTotalCount(int totalCount);

private:
	int totalCount;
	int listSize;
	std::vector<ChatUser> friends;
};

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_PACKAGE_BODY_LISTFRIENDSRESPONSE_H_ */
