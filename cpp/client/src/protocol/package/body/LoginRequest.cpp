/*
 * LoginRequest.cpp
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#include "protocol/package/body/LoginRequest.h"

namespace chat {
namespace protocol {
namespace package {

LoginRequest::LoginRequest() {

}

LoginRequest::LoginRequest(std::wstring loginName, std::wstring loginPass)
		:loginName(loginName), loginPass(loginPass) {

}

std::wostream& operator<<(std::wostream& os, const LoginRequest& request) {
	return os << "{loginName: \"" << request.loginName << "\", loginPass: \"" << request.loginPass << "\"}";
}

std::wostream& operator<<(std::wostream& os, const std::unique_ptr<LoginRequest>& request) {
	return os << *request;
}

const std::wstring& LoginRequest::getLoginName() const {
	return loginName;
}

void LoginRequest::setLoginName(const std::wstring &loginName) {
	this->loginName = loginName;
}

const std::wstring& LoginRequest::getLoginPass() const {
	return loginPass;
}

void LoginRequest::setLoginPass(const std::wstring &loginPass) {
	this->loginPass = loginPass;
}

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
