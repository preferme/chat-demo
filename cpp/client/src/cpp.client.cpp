//============================================================================
// Name        : client.cpp
// Author      : HL
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <memory>
#include <iostream>
#include <vector>
using namespace std;

#include "protocol/package/Package.h"
using namespace chat::protocol::package;


int main() {

	std::unique_ptr<PackageHeader> header = std::make_unique<PackageHeader>(PackageVersion::V01, 1, PackageType::LoginResponse, 2);
	std::unique_ptr<PackageBody> body = std::make_unique<PackageBody>();
	std::unique_ptr<Package> package  = std::make_unique<Package>(header, body);
	header = std::make_unique<PackageHeader>(PackageVersion::V11, 2, PackageType::RegistResponse, 3);
	body   = std::make_unique<PackageBody>();
	std::unique_ptr<Package> package1  = std::make_unique<Package>(header, body);
	std::vector<unique_ptr<Package>> vector;
	vector.push_back(std::move(package));
	vector.push_back(std::move(package1));
//	vector.push_back(package1);
//	cout << "Package : " << package << endl;
//	cout << "Package : " << package1 << endl;
//	cout << "Vector : " << vector << endl;

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
