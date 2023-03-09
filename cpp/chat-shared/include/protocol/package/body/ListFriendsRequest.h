/*
 * ListFriendsRequest.h
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_PACKAGE_BODY_LISTFRIENDSREQUEST_H_
#define __PROTOCOL_PACKAGE_BODY_LISTFRIENDSREQUEST_H_

#include <protocol/package/ChatRequest.h>
#include <iostream>
#include <memory>

namespace chat {
namespace protocol {
namespace package {

class ListFriendsRequest final : public ChatRequest {
public:
	ListFriendsRequest();
	ListFriendsRequest(int start, int display);

	friend std::wostream& operator<<(std::wostream& os, const ListFriendsRequest& request);
	friend std::wostream& operator<<(std::wostream& os, const std::unique_ptr<ListFriendsRequest>& request);

	int getDisplay() const;
	void setDisplay(int display);
	int getStart() const;
	void setStart(int start);

private:
	int start;
	int display;
};

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_PACKAGE_BODY_LISTFRIENDSREQUEST_H_ */
