/*
 * cerror_exception.hpp
 *
 *  Created on: 2023/4/16 21:49
 *      Author: hou-lei
 */
#ifndef __CHAT_DEMO_CERROR_EXCEPTION_HPP_
#define __CHAT_DEMO_CERROR_EXCEPTION_HPP_

#include "exception/exception.hpp"
#include <cstring>

namespace chat {
namespace exception {

class cerror_exception : public exception {
private:
    int m_error_no;

public:
    cerror_exception() :exception(strerror(errno)), m_error_no(errno) {

    }

    int get_error_no() const {
        return m_error_no;
    }

    friend std::ostream& operator<<(std::ostream& os, cerror_exception& ex) {
        char fmt[32] = { 0 }; tm stime {0};
        time_t timer = std::time(0);
        localtime_r(&timer, &stime);
        strftime(fmt, sizeof(fmt), "%Y-%m-%d %H:%M:%S", &stime);
        // %s\t%30s:%-5d[%s] : %s\n
        os << fmt << "\t" << std::setw(30) << std::right << ex.get_file_name();
        os << ":" << std::setw(5) << std::left << ex.get_line_number();
        os << "[" << ex.get_func_name() << "]";
        os << " ("<< ex.m_error_no << "): " << ex.get_message() << std::endl;
        return os;
    }

};

}
}

#endif //__CHAT_DEMO_CERROR_EXCEPTION_HPP_
