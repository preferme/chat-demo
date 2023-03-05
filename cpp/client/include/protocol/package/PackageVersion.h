/*
 * PackageVersion.h
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_PACKAGE_PACKAGEVERSION_H_
#define __PROTOCOL_PACKAGE_PACKAGEVERSION_H_

#include<iostream>

namespace chat {
namespace protocol {
namespace package {


typedef enum PackageVersion {
	// ByteOrder + Version
	V01=(int)0xFEFF0001,
	V11=(int)0xFEFF0101,

	UnkownVersion=0
} PackageVersion;

PackageVersion parsePackageVersion(int value);
std::istream& operator>>(std::istream& is, PackageVersion& version);
std::ostream& operator<<(std::ostream& os, const PackageVersion& version);

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_PACKAGE_PACKAGEVERSION_H_ */
