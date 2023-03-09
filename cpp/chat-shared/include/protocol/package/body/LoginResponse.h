/*
 * LoginResponse.h
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_PACKAGE_BODY_LOGINRESPONSE_H_
#define __PROTOCOL_PACKAGE_BODY_LOGINRESPONSE_H_

#include <protocol/package/ChatResponse.h>
#include <protocol/package/body/ChatUser.h>
#include <iostream>
#include <memory>

namespace chat {
namespace protocol {
namespace package {

class LoginResponse final : public ChatResponse {
public:
	LoginResponse();
	LoginResponse(int errorCode, std::wstring message);
	LoginResponse(ChatUser user);

	friend std::wostream& operator<<(std::wostream& os, const LoginResponse& response);
	friend std::wostream& operator<<(std::wostream& os, const std::unique_ptr<LoginResponse>& response);

	const ChatUser& getUser() const;
	void setUser(const ChatUser &user);

private:
	ChatUser user;
};

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_PACKAGE_BODY_LOGINRESPONSE_H_ */
