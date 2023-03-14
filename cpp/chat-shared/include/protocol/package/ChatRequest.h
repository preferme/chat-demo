/*
 * CodecUtils.h
 *
 *  Created on: 2023年3月8日
 *      Author: hou-lei
 */

#ifndef CPP_CHATREQUEST_H
#define CPP_CHATREQUEST_H

#include <protocol/package/PackageBody.h>

namespace chat {
namespace protocol {
namespace package {

class ChatRequest : public PackageBody {
public:
    ChatRequest();
    ~ChatRequest();

    friend std::wostream& operator<<(std::wostream& os, const ChatRequest& body);
    friend std::wostream& operator<<(std::wostream& os, const std::unique_ptr<ChatRequest>& body);

};

}
}
}



#endif //CPP_CHATREQUEST_H
