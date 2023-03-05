/*
 * RegistRequest.cpp
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#include "protocol/package/body/RegistRequest.h"

namespace chat {
namespace protocol {
namespace package {

RegistRequest::RegistRequest() {

}

RegistRequest::RegistRequest(std::wstring loginName, std::wstring loginPass, std::wstring nickName)
		:loginName(loginName), loginPass(loginPass), nickName(nickName){

}

std::wostream& operator<<(std::wostream& os, const RegistRequest& request) {
	return os << "{loginName: \"" << request.loginName << "\", loginPass: \"" << request.loginPass << "\", nickName: \"" << request.nickName << "\"}";
}

std::wostream& operator<<(std::wostream& os, const std::unique_ptr<RegistRequest>& request) {
	return os << *request;
}

const std::wstring& RegistRequest::getLoginName() const {
	return loginName;
}

void RegistRequest::setLoginName(const std::wstring &loginName) {
	this->loginName = loginName;
}

const std::wstring& RegistRequest::getLoginPass() const {
	return loginPass;
}

void RegistRequest::setLoginPass(const std::wstring &loginPass) {
	this->loginPass = loginPass;
}

const std::wstring& RegistRequest::getNickName() const {
	return nickName;
}

void RegistRequest::setNickName(const std::wstring &nickName) {
	this->nickName = nickName;
}

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
