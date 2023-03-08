/*
 * SendMessageRequest.h
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_PACKAGE_BODY_SENDMESSAGEREQUEST_H_
#define __PROTOCOL_PACKAGE_BODY_SENDMESSAGEREQUEST_H_

#include <protocol/package/ChatRequest.h>
#include <protocol/package/body/ChatUser.h>
#include <string>
#include <iostream>
#include <memory>

namespace chat {
namespace protocol {
namespace package {

class SendMessageRequest final : public ChatRequest {
public:
	SendMessageRequest();
	SendMessageRequest(ChatUser user, std::wstring message);

	friend std::wostream& operator<<(std::wostream& os, const SendMessageRequest& request);
	friend std::wostream& operator<<(std::wostream& os, const std::unique_ptr<SendMessageRequest>& request);

	const std::wstring& getMessage() const;
	void setMessage(const std::wstring &message);
	const ChatUser& getUser() const;
	void setUser(const ChatUser &user);

private:
	ChatUser user;
	std::wstring message;
};

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_PACKAGE_BODY_SENDMESSAGEREQUEST_H_ */
