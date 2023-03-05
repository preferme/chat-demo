/*
 * PackageType.cpp
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */
#include "protocol/package/PackageType.h"

namespace chat {
namespace protocol {
namespace package {


PackageType parsePackageType(int value) {
	switch(value){
		case (int)PackageType::HeartBeatRequest:
		case (int)PackageType::HeartBeatResponse:
		case (int)PackageType::LoginRequest:
		case (int)PackageType::LoginResponse:
		case (int)PackageType::ListFriendsRequest:
		case (int)PackageType::ListFriendsResponse:
		case (int)PackageType::MakeFriendsRequest:
		case (int)PackageType::MakeFriendsResponse:
		case (int)PackageType::SendMessageRequest:
		case (int)PackageType::SendMessageResponse:
			return (PackageType)value;
		default:
			return PackageType::UnkownType;
		}
}

std::istream& operator>>(std::istream& is, PackageType& type) {
	int value = 0;
	is >> value;
	type = parsePackageType(value);
	return is;
}

std::ostream& operator<<(std::ostream& os, const PackageType& type) {
	switch(type){
		case PackageType::HeartBeatRequest:
			os << "HeartBeatRequest"; break;
		case PackageType::HeartBeatResponse:
			os << "HeartBeatResponse"; break;
		case PackageType::LoginRequest:
			os << "LoginRequest"; break;
		case PackageType::LoginResponse:
			os << "LoginResponse"; break;
		case PackageType::ListFriendsRequest:
			os << "ListFriendsRequest"; break;
		case PackageType::ListFriendsResponse:
			os << "ListFriendsResponse"; break;
		case PackageType::MakeFriendsRequest:
			os << "MakeFriendsRequest"; break;
		case PackageType::MakeFriendsResponse:
			os << "MakeFriendsResponse"; break;
		case PackageType::SendMessageRequest:
			os << "SendMessageRequest"; break;
		case PackageType::SendMessageResponse:
			os << "SendMessageResponse"; break;
		default:
			os << "UnkownType";
	}
	os << "(0x" << std::uppercase << std::hex;
	os.width(8);
	os.fill('0');
	os << (unsigned int)type << ")";
	return os;
}


} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
