/*
 * ListFriendsRequest.cpp
 *
 *  Created on: 2023年3月5日
 *      Author: hou-lei
 */

#include "protocol/package/body/ListFriendsRequest.h"

namespace chat {
namespace protocol {
namespace package {

ListFriendsRequest::ListFriendsRequest()
		:start(0), display(0) {

}

ListFriendsRequest::ListFriendsRequest(int start, int display)
		:start(start), display(display) {

}

std::wostream& operator<<(std::wostream& os, const ListFriendsRequest& request) {
	return os << "{start: " << request.getStart() << ", display: " << request.getDisplay() << "}";
}

std::wostream& operator<<(std::wostream& os, const std::unique_ptr<ListFriendsRequest>& request) {
	return os << *request;
}

int ListFriendsRequest::getDisplay() const {
	return display;
}

void ListFriendsRequest::setDisplay(int display) {
	this->display = display;
}

int ListFriendsRequest::getStart() const {
	return start;
}

void ListFriendsRequest::setStart(int start) {
	this->start = start;
}

} /* namespace package */
} /* namespace protocol */
} /* namespace chat */
