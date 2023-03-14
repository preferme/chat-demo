/*
 * CErrorException.cpp
 *
 *  Created on: 2023年3月10日
 *      Author: hou-lei
 */

#include "exception/CErrorException.h"
#include <cerrno>
#include <cstring>
#include <iomanip>


namespace chat {
namespace exception {

CErrorException::CErrorException()
    :Exception(strerror(errno)), errorno(errno) {
//    extern int errno;
//    this->errorno = errno;
}

std::ostream& operator<<(std::ostream& os, CErrorException& ex) {
    char fmt[32] = { 0 }; tm stime {0};
    time_t timer = std::time(0);
    localtime_r(&timer, &stime);
    strftime(fmt, sizeof(fmt), "%Y-%m-%d %H:%M:%S", &stime);
    // %s\t%30s:%-5d[%s] : %s\n
    os << fmt << "\t" << std::setw(30) << std::right << ex.getFileName();
    os << ":" << std::setw(5) << std::left << ex.getLineNumber();
    os << "[" << ex.getFunName() << "]";
    os << " ("<< ex.errorno << "): " << ex.getMessage() << std::endl;
    return os;
}

int CErrorException::getErrorNo() const {
    return errorno;
}

}
}