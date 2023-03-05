/*
 * MakeFriendsRequest.h
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_PACKAGE_BODY_MAKEFRIENDSREQUEST_H_
#define __PROTOCOL_PACKAGE_BODY_MAKEFRIENDSREQUEST_H_

#include <protocol/package/PackageBody.h>
#include <protocol/package/body/ChatUser.h>
#include <iostream>
#include <memory>

namespace chat {
namespace protocol {
namespace package {

class MakeFriendsRequest final : public PackageBody {
public:
	MakeFriendsRequest();
	MakeFriendsRequest(ChatUser friends);

	friend std::wostream& operator<<(std::wostream& os, const MakeFriendsRequest& request);
	friend std::wostream& operator<<(std::wostream& os, const std::unique_ptr<MakeFriendsRequest>& request);

	const ChatUser& getFriends() const;
	void setFriends(const ChatUser &friends);

private:
	ChatUser friends;
};

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_PACKAGE_BODY_MAKEFRIENDSREQUEST_H_ */
