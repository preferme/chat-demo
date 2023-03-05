/*
 * PackageType.h
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_PACKAGE_PACKAGETYPE_H_
#define __PROTOCOL_PACKAGE_PACKAGETYPE_H_

#include<iostream>

namespace chat {
namespace protocol {
namespace package {


typedef enum PackageType {
	HeartBeatRequest	= (int)0x00000001,
	HeartBeatResponse	= (int)0x80000001,
	RegistRequest		= (int)0x00000002,
	RegistResponse		= (int)0x80000002,
	LoginRequest		= (int)0x00000003,
	LoginResponse		= (int)0x80000003,
	ListFriendsRequest	= (int)0x00000004,
	ListFriendsResponse	= (int)0x80000004,
	MakeFriendsRequest	= (int)0x00000005,
	MakeFriendsResponse	= (int)0x80000005,
	SendMessageRequest	= (int)0x00000006,
	SendMessageResponse	= (int)0x80000006,

	UnkownType			= (int)0x00000000
} PackageType;

PackageType parsePackageType(int value);
std::istream& operator>>(std::istream& is, PackageType& type);
std::ostream& operator<<(std::ostream& os, const PackageType& type);
std::wostream& operator<<(std::wostream& os, const PackageType& type);

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_PACKAGE_PACKAGETYPE_H_ */
