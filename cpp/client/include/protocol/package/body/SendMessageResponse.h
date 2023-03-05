/*
 * SendMessageResponse.h
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_PACKAGE_BODY_SENDMESSAGERESPONSE_H_
#define __PROTOCOL_PACKAGE_BODY_SENDMESSAGERESPONSE_H_

#include <protocol/package/ResponseBody.h>
#include <iostream>
#include <memory>

namespace chat {
namespace protocol {
namespace package {

class SendMessageResponse final : public ResponseBody {
public:
	SendMessageResponse();
	SendMessageResponse(int errorCode, std::wstring message);

	friend std::wostream& operator<<(std::wostream& os, const SendMessageResponse& response);
	friend std::wostream& operator<<(std::wostream& os, const std::unique_ptr<SendMessageResponse>& response);
};

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_PACKAGE_BODY_SENDMESSAGERESPONSE_H_ */
