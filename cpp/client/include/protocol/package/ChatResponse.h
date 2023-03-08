/*
 * ChatResponse.h
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_PACKAGE_CHATRESPONSE_H_
#define __PROTOCOL_PACKAGE_CHATRESPONSE_H_

#include <protocol/package/PackageBody.h>
#include <string>

namespace chat {
namespace protocol {
namespace package {

class ChatResponse : public PackageBody {
public:
	ChatResponse();
	ChatResponse(int errorCode, std::wstring message);
	virtual ~ChatResponse();

	friend std::wostream& operator<<(std::wostream& os, const ChatResponse& body);
	friend std::wostream& operator<<(std::wostream& os, const std::unique_ptr<ChatResponse>& body);

	int getErrorCode() const;
	void setErrorCode(int errorCode);
	const std::wstring& getMessage() const;
	void setMessage(const std::wstring &message);

protected:
	int errorCode;
	std::wstring message;
};

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_PACKAGE_CHATRESPONSE_H_ */
