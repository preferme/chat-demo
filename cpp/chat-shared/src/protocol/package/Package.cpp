/*
 * Package.cpp
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#include "protocol/package/Package.h"

namespace chat {
namespace protocol {
namespace package {


Package::Package(std::shared_ptr<PackageHeader> &header, std::shared_ptr<PackageBody> &body)
		:header(std::move(header)), body(std::move(body)) {

}

Package::Package(Package &&other)
		:header(nullptr), body(nullptr) {
	std::cout<< "Package Move" << std::endl;
	*this = std::move(other);
}

Package& Package::operator=(Package &&other) {
	std::cout<< "Package Move" << std::endl;
	if(this != &other) {
		this->header.swap(other.header);
		this->body.swap(other.body);
		std::cout<< "swap" << std::endl;
	}
	std::cout<< "Package Move." << std::endl;
	return *this;
}

std::wostream& operator<<(std::wostream& os, const Package& package) {
	os << "{header: " << package.getHeader();
	os << ", body: " << package.getBody();
	os << '}';
	return os;
}

std::wostream& operator<<(std::wostream& os, const std::shared_ptr<Package>& package) {
	return os << *package;
}

const std::shared_ptr<PackageBody>& Package::getBody() const {
	return body;
}

const std::shared_ptr<PackageHeader>& Package::getHeader() const {
	return header;
}


} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
