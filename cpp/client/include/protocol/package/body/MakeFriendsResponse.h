/*
 * MakeFriendsResponse.h
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_PACKAGE_BODY_MAKEFRIENDSRESPONSE_H_
#define __PROTOCOL_PACKAGE_BODY_MAKEFRIENDSRESPONSE_H_

#include <protocol/package/ResponseBody.h>
#include <protocol/package/body/ChatUser.h>
#include <iostream>
#include <memory>

namespace chat {
namespace protocol {
namespace package {

class MakeFriendsResponse final : public ResponseBody {
public:
	MakeFriendsResponse();
	MakeFriendsResponse(int errorCode, std::wstring message);
	MakeFriendsResponse(ChatUser friends);

	friend std::wostream& operator<<(std::wostream& os, const MakeFriendsResponse& response);
	friend std::wostream& operator<<(std::wostream& os, const std::unique_ptr<MakeFriendsResponse>& response);

	const ChatUser& getFriends() const;
	void setFriends(const ChatUser &friends);

private:
	ChatUser friends;
};

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_PACKAGE_BODY_MAKEFRIENDSRESPONSE_H_ */
