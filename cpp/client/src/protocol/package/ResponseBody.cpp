/*
 * ResponseBody.cpp
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#include "protocol/package/ResponseBody.h"

namespace chat {
namespace protocol {
namespace package {

ResponseBody::ResponseBody():errorCode(0) {

}

ResponseBody::ResponseBody(int errorCode, std::wstring message)
		:errorCode(errorCode), message(message) {

}

ResponseBody::~ResponseBody() {

}

std::wostream& operator<<(std::wostream& os, const ResponseBody& body) {
	return os << L"{errorCode: " << body.getErrorCode() << L", message: \"" << body.getMessage() << L"\"}";
}

std::wostream& operator<<(std::wostream& os, const std::unique_ptr<ResponseBody>& body) {
	return os << *body;
}

int ResponseBody::getErrorCode() const {
	return errorCode;
}

void ResponseBody::setErrorCode(int errorCode) {
	this->errorCode = errorCode;
}

const std::wstring& ResponseBody::getMessage() const {
	return message;
}

void ResponseBody::setMessage(const std::wstring &message) {
	this->message = message;
}

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
