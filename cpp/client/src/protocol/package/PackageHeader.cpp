/*
 * PackageHeader.cpp
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#include "protocol/package/PackageHeader.h"

namespace chat {
namespace protocol {
namespace package {


PackageHeader::PackageHeader()
		:version(PackageVersion::V01), sequence(0), type(PackageType::UnkownType), length(0) {

}

PackageHeader::PackageHeader(PackageVersion version, int sequence, PackageType type, int length)
		:version(version), sequence(sequence), type(type), length(length) {

}

std::ostream& operator<<(std::ostream& os, const PackageHeader& header) {
	os << '{'  << "version: " << header.getVersion();
	os << ", " << "sequence: " << header.getSequence();
	os << ", " << "type: " << header.getType();
	os << ", " << "length: " << header.getLength();
	os << '}';
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::unique_ptr<PackageHeader>& header) {
	return os << *header;
}

int PackageHeader::getLength() const {
	return length;
}

PackageHeader& PackageHeader::setLength(int length) {
	this->length = length;
	return *this;
}

int PackageHeader::getSequence() const {
	return sequence;
}

PackageHeader& PackageHeader::setSequence(int sequence) {
	this->sequence = sequence;
	return *this;
}

PackageType PackageHeader::getType() const {
	return type;
}

PackageHeader& PackageHeader::setType(PackageType type) {
	this->type = type;
	return *this;
}

PackageVersion PackageHeader::getVersion() const {
	return version;
}

PackageHeader& PackageHeader::setVersion(PackageVersion version) {
	this->version = version;
	return *this;
}


} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
