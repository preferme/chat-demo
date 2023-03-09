/*
 * PackageCodec.h
 *
 *  Created on: 2023年3月6日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_CODEC_PACKAGE_CODEC_H_
#define __PROTOCOL_CODEC_PACKAGE_CODEC_H_

#include "protocol/package/Package.h"
#include "protocol/codec/CodecException.h"
#include <iostream>

using namespace chat::protocol::package;
using namespace chat::protocol::codec;

class PackageCodec {
public:

    int encode(std::unique_ptr<Package>& package, std::ostream& os) throw(CodecException);
    int decode(std::istream& is, std::unique_ptr<Package>& package) throw(CodecException);

};


#endif /* __PROTOCOL_CODEC_PACKAGE_CODEC_H_ */
