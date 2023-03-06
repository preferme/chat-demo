//============================================================================
// Name        : main.cpp
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
#include <protocol/package/body/ChatUser.h>
using namespace chat::protocol::package;


int main() {
    ChatUser user(1,L"",L"");
    std::unique_ptr<PackageHeader> header = std::make_unique<PackageHeader>(PackageVersion::V01, 1, PackageType::LoginResponse, 2);
    std::unique_ptr<PackageBody> body = std::make_unique<PackageBody>();

    wcout << "header : " << header << endl;
    wcout << "body : " << body << endl;
//	cout << "Vector : " << vector << endl;

    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
    return 0;
}