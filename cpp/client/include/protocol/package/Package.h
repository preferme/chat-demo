/*
 * Package.h
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_PACKAGE_PACKAGE_H_
#define __PROTOCOL_PACKAGE_PACKAGE_H_

#include "PackageHeader.h"
#include "PackageBody.h"
#include <memory>

namespace chat {
namespace protocol {
namespace package {

class Package final {
private:
	std::unique_ptr<PackageHeader> header;
	std::unique_ptr<PackageBody> body;
public:
	Package(std::unique_ptr<PackageHeader> &header, std::unique_ptr<PackageBody> &body);
	Package(Package &&other);
	Package& operator=(Package &&other);

	friend std::ostream& operator<<(std::ostream& os, const Package& package);
	friend std::ostream& operator<<(std::ostream& os, const std::unique_ptr<Package>& package);

	const std::unique_ptr<PackageBody>& getBody() const;
	const std::unique_ptr<PackageHeader>& getHeader() const;
};

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_PACKAGE_PACKAGE_H_ */
