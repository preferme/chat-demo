/*
 * ResponseBody.h
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_PACKAGE_RESPONSEBODY_H_
#define __PROTOCOL_PACKAGE_RESPONSEBODY_H_

#include <protocol/package/PackageBody.h>
#include <string>

namespace chat {
namespace protocol {
namespace package {

class ResponseBody : public PackageBody {
public:
	ResponseBody();
	ResponseBody(int errorCode, std::wstring message);
	virtual ~ResponseBody();

	friend std::wostream& operator<<(std::wostream& os, const ResponseBody& body);
	friend std::wostream& operator<<(std::wostream& os, const std::unique_ptr<ResponseBody>& body);

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

#endif /* __PROTOCOL_PACKAGE_RESPONSEBODY_H_ */
