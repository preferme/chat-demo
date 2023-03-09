/*
 * LoginRequest.h
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_PACKAGE_BODY_LOGINREQUEST_H_
#define __PROTOCOL_PACKAGE_BODY_LOGINREQUEST_H_

#include <protocol/package/ChatRequest.h>
#include <string>
#include <iostream>
#include <memory>

namespace chat {
namespace protocol {
namespace package {

class LoginRequest final : public ChatRequest {
public:
	LoginRequest();
	LoginRequest(std::wstring loginName, std::wstring loginPass);

	friend std::wostream& operator<<(std::wostream& os, const LoginRequest& request);
	friend std::wostream& operator<<(std::wostream& os, const std::unique_ptr<LoginRequest>& request);

	const std::wstring& getLoginName() const;
	void setLoginName(const std::wstring &loginName);
	const std::wstring& getLoginPass() const;
	void setLoginPass(const std::wstring &loginPass);

private:
	std::wstring loginName;
	std::wstring loginPass;
};

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_PACKAGE_BODY_LOGINREQUEST_H_ */
