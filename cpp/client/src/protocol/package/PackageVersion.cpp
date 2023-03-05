/*
 * PackageVersion.cpp
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */
#include "protocol/package/PackageVersion.h"
#include "protocol/codec/StringConverter.h"
#include <sstream>

namespace chat {
namespace protocol {
namespace package {

PackageVersion parsePackageVersion(int value) {
	switch(value){
	case (int)PackageVersion::V01:
	case (int)PackageVersion::V11:
		return (PackageVersion)value;
	default:
		return PackageVersion::UnkownVersion;
	}
}

std::istream& operator>>(std::istream& is, PackageVersion& version) {
	int value = 0;
	is >> value;
	version = parsePackageVersion(value);
	return is;
}

std::ostream& operator<<(std::ostream& os, const PackageVersion& version) {
	std::wstringstream wss;
	wss << version;
	os << chat::protocol::codec::to_string(wss.str());
	return os;
}
std::wostream& operator<<(std::wostream& os, const PackageVersion& version) {
	switch(version) {
	case PackageVersion::V01 :
		os << "V01"; break;
	case PackageVersion::V11 :
		os << "V11"; break;
	default:
		os << "UnkownVersion";
	}
	os << "(0x" << std::uppercase << std::hex;
	os.width(8);
	os.fill('0');
	os << (unsigned int)version << ")";
	return os;
}

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
