/*
 * PackageHeader.h
 *
 *  | B/L Endian | Version | Sequence | PackageType | BodyLength |
 *  | 2 Bytes    | 2 Bytes | 4 Bytes  | 4 Bytes     | 4 Bytes    |
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_PACKAGE_PACKAGEHEADER_H_
#define __PROTOCOL_PACKAGE_PACKAGEHEADER_H_

#include "PackageVersion.h"
#include "PackageType.h"
#include <iostream>
#include <memory>

namespace chat {
namespace protocol {
namespace package {

class PackageHeader final {
private:
	PackageVersion version;
	int sequence;
	PackageType type;
	int length;
public:
	PackageHeader();
	PackageHeader(PackageVersion version, int sequence, PackageType type, int length);

	friend std::wostream& operator<<(std::wostream& os, const PackageHeader& header);
	friend std::wostream& operator<<(std::wostream& os, const std::unique_ptr<PackageHeader>& header);

	int getLength() const;
	PackageHeader& setLength(int length);
	int getSequence() const;
	PackageHeader& setSequence(int sequence);
	PackageType getType() const;
	PackageHeader& setType(PackageType type);
	PackageVersion getVersion() const;
	PackageHeader& setVersion(PackageVersion version);

};

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_PACKAGE_PACKAGEHEADER_H_ */
