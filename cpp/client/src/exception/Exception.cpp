//
// Created by NAVER on 2023/3/6.
//

#include "exception/Exception.h"
#include <string>

namespace chat {
namespace exception {

Exception::Exception(const ::std::string& message)
        :message(message), funName(), fileName(), lineNumber(0) {

}

Exception::Exception(const ::std::string message, const ::std::string funName, const ::std::string fileName, int lineNumber)
        :message(message), funName(funName), fileName(fileName), lineNumber(0) {

}

// @Override from std::exception
Exception::~Exception() {

}

// @Override from std::exception
const char* Exception::what() const noexcept {
    return message.c_str();
}

Exception& Exception::setMessage(::std::string message) {
    this->message = message;
    return *this;
}

::std::string& Exception::getMessage() {
    return this->message;
}

Exception& Exception::setFunName(::std::string funName) {
    this->funName = funName;
    return *this;
}

::std::string& Exception::getFunName() {
    return this->funName;
}

Exception& Exception::setFileName(::std::string fileName) {
    this->fileName = fileName;
    return *this;
}

::std::string& Exception::getFileName() {
    return this->fileName;
}

Exception& Exception::setLineNumber(int lineNumber) {
    this->lineNumber = lineNumber;
    return *this;
}

int Exception::getLineNumber() {
    return this->lineNumber;
}

}
}