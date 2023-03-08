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
public:
	Package(std::shared_ptr<PackageHeader> &header, std::shared_ptr<PackageBody> &body);
	Package(Package &&other);
	Package& operator=(Package &&other);

	friend std::wostream& operator<<(std::wostream& os, const Package& package);
	friend std::wostream& operator<<(std::wostream& os, const std::shared_ptr<Package>& package);

	const std::shared_ptr<PackageBody>& getBody() const;
	const std::shared_ptr<PackageHeader>& getHeader() const;

private:
    std::shared_ptr<PackageHeader> header;
    std::shared_ptr<PackageBody> body;
};

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */

#endif /* __PROTOCOL_PACKAGE_PACKAGE_H_ */
