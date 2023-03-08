/*
 * PackageBody.h
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_PACKAGE_PACKAGEBODY_H_
#define __PROTOCOL_PACKAGE_PACKAGEBODY_H_

#include <iostream>
#include <memory>

namespace chat {
namespace protocol {
namespace package {

class PackageBody {
public:
	PackageBody();
	virtual ~PackageBody();
	friend std::wostream& operator<<(std::wostream& os, const PackageBody& body);
	friend std::wostream& operator<<(std::wostream& os, const std::shared_ptr<PackageBody>& body);
};


} /* namespace package */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_PACKAGE_PACKAGEBODY_H_ */
