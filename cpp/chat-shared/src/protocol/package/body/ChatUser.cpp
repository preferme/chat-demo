/*
 * ChatUser.cpp
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#include "protocol/package/body/ChatUser.h"

namespace chat {
namespace protocol {
namespace package {

ChatUser::ChatUser():userId(0) {

}

ChatUser::ChatUser(int userId, std::wstring loginName, std::wstring nickName)
		:userId(userId), loginName(loginName), nickName(nickName){

}

std::wostream& operator<<(std::wostream& os, const ChatUser& user) {
	os << "{userId: " << user.getUserId();
	os << ", loginName: \"" << user.getLoginName();
	os << "\", nickName: \"" << user.getNickName();
	os << "\"}";
	return os;
}

std::wostream& operator<<(std::wostream& os, const std::unique_ptr<ChatUser>& user){
	return os << *user;
}

const std::wstring& ChatUser::getLoginName() const {
	return loginName;
}

void ChatUser::setLoginName(const std::wstring &loginName) {
	this->loginName = loginName;
}

const std::wstring& ChatUser::getNickName() const {
	return nickName;
}

void ChatUser::setNickName(const std::wstring &nickName) {
	this->nickName = nickName;
}

int ChatUser::getUserId() const {
	return userId;
}

void ChatUser::setUserId(int userId) {
	this->userId = userId;
}

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
