/*
 * PackageBody.cpp
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#include "protocol/package/PackageBody.h"

namespace chat {
namespace protocol {
namespace package {

PackageBody::PackageBody() {

}

PackageBody::~PackageBody() {

}

std::wostream& operator<<(std::wostream& os, const PackageBody& body) {
	return os << "{}";
}

std::wostream& operator<<(std::wostream& os, const std::unique_ptr<PackageBody>& body) {
	return os << *body;
}

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
