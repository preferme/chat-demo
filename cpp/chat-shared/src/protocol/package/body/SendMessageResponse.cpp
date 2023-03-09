/*
 * SendMessageResponse.cpp
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#include "protocol/package/body/SendMessageResponse.h"

namespace chat {
namespace protocol {
namespace package {

SendMessageResponse::SendMessageResponse(): ChatResponse() {

}

SendMessageResponse::SendMessageResponse(int errorCode, std::wstring message)
		: ChatResponse(errorCode, message) {

}

std::wostream& operator<<(std::wostream& os, const SendMessageResponse& response) {
	return os << (ChatResponse)response;
}

std::wostream& operator<<(std::wostream& os, const std::unique_ptr<SendMessageResponse>& response) {
	return os << *response;
}

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
