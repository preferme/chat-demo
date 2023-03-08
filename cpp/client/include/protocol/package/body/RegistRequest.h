/*
 * RegistRequest.h
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_PACKAGE_BODY_REGISTREQUEST_H_
#define __PROTOCOL_PACKAGE_BODY_REGISTREQUEST_H_

#include <protocol/package/ChatRequest.h>
#include <string>
#include <iostream>
#include <memory>

namespace chat {
namespace protocol {
namespace package {

class RegistRequest final : public ChatRequest {
public:
	RegistRequest();
	RegistRequest(std::wstring loginName, std::wstring loginPass, std::wstring nickName);

	friend std::wostream& operator<<(std::wostream& os, const RegistRequest& request);
	friend std::wostream& operator<<(std::wostream& os, const std::unique_ptr<RegistRequest>& request);

	const std::wstring& getLoginName() const;
	void setLoginName(const std::wstring &loginName);
	const std::wstring& getLoginPass() const;
	void setLoginPass(const std::wstring &loginPass);
	const std::wstring& getNickName() const;
	void setNickName(const std::wstring &nickName);

private:
	std::wstring loginName;
	std::wstring loginPass;
	std::wstring nickName;
};

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_PACKAGE_BODY_REGISTREQUEST_H_ */
