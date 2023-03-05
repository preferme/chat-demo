/*
 * PackageBody.h
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_PACKAGE_PACKAGEBODY_H_
#define __PROTOCOL_PACKAGE_PACKAGEBODY_H_

#include<iostream>
#include <memory>

namespace chat {
namespace protocol {
namespace package {

class PackageBody {
public:
	PackageBody();
	virtual ~PackageBody();
	friend std::ostream& operator<<(std::ostream& os, const PackageBody& body);
	friend std::ostream& operator<<(std::ostream& os, const std::unique_ptr<PackageBody>& body);
};


} /* namespace package */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_PACKAGE_PACKAGEBODY_H_ */
