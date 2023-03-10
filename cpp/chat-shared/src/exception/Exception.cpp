//
// Created by NAVER on 2023/3/6.
//

#include "exception/Exception.h"
#include <string>
#include <ctime>
#include <cstdio>
#include <iomanip>

namespace chat {
namespace exception {

Exception::Exception(const ::std::string message)
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

std::ostream& operator<<(std::ostream& os, Exception& ex) {
    char fmt[32] = { 0 }; tm stime {0};
    time_t timer = std::time(0);
    localtime_r(&timer, &stime);
    strftime(fmt, sizeof(fmt), "%Y-%m-%d %H:%M:%S", &stime);
    // %s\t%30s:%-5d[%s] : %s\n
    os << fmt << std::setw(30) << std::left << ex.fileName;
    os << ":" << std::setw(5) << std::right << ex.lineNumber;
    os << "[" << ex.funName << "]";
    os << " : " << ex.message << std::endl;
    return os;
}

}
}