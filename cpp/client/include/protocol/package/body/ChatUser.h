/*
 * ChatUser.h
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_PACKAGE_BODY_CHATUSER_H_
#define __PROTOCOL_PACKAGE_BODY_CHATUSER_H_

#include <string>
#include <iostream>
#include <memory>

namespace chat {
namespace protocol {
namespace package {

class ChatUser final {
public:
	ChatUser();
	ChatUser(int userId, std::wstring loginName, std::wstring nickName);

	friend std::wostream& operator<<(std::wostream& os, const ChatUser& user);
	friend std::wostream& operator<<(std::wostream& os, const std::unique_ptr<ChatUser>& user);

	const std::wstring& getLoginName() const;
	void setLoginName(const std::wstring &loginName);
	const std::wstring& getNickName() const;
	void setNickName(const std::wstring &nickName);
	int getUserId() const;
	void setUserId(int userId);

private:
	int userId;
	std::wstring loginName;
	std::wstring nickName;
};

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_PACKAGE_BODY_CHATUSER_H_ */
