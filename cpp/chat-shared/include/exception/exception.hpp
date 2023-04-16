/*
 * exception.hpp
 *
 *  Created on: 2023/4/16 21:36
 *      Author: hou-lei
 */
#ifndef __CHAT_DEMO_EXCEPTION_HPP_
#define __CHAT_DEMO_EXCEPTION_HPP_

#include <exception>
#include <string>
#include <ostream>
#include <ctime>
#include <iomanip>

namespace chat {
namespace exception {

    class exception : public std::exception {
    protected:
        std::string m_message;
        std::string m_func_name;
        std::string m_file_name;
        int m_line_number;

    public:
        explicit exception(const std::string message)
                :m_message(message), m_func_name(), m_file_name(), m_line_number(0) {

        }

        exception(const std::string message, const std::string func_name, const std::string file_name, int line_number)
                :m_message(message), m_func_name(func_name), m_file_name(file_name), m_line_number(line_number) {

        }

        virtual ~exception() {}

        // @Override from std::exception
        const char* what() const noexcept override {
            return m_message.c_str();
        }

        exception& set_message(std::string message) {
            m_message = message;
            return *this;
        }
        std::string& get_message() {
            return m_message;
        }

        exception& set_func_name(std::string func_name) {
            m_func_name = func_name;
            return *this;
        }
        std::string& get_func_name() {
            return m_func_name;
        }

        exception& set_file_name(std::string file_name) {
            m_file_name = file_name;
            return *this;
        }
        std::string& get_file_name() {
            return m_file_name;
        }

        exception& set_line_number(int line_number) {
            m_line_number = line_number;
            return *this;
        }
        int get_line_number() {
            return m_line_number;
        }

        friend std::ostream& operator<<(std::ostream& os, exception& ex) {
            char fmt[32] = { 0 }; tm stime {0};
            time_t timer = std::time(0);
            localtime_r(&timer, &stime);
            strftime(fmt, sizeof(fmt), "%Y-%m-%d %H:%M:%S", &stime);
            // %s\t%30s:%-5d[%s] : %s\n
            os << fmt << "\t" << std::setw(30) << std::right << ex.m_file_name;
            os << ":" << std::setw(5) << std::left << ex.m_line_number;
            os << "[" << ex.m_func_name << "]";
            os << " : " << ex.m_message << std::endl;
            return os;
        }

    };

// utils for throw exception
#define THROW_EXCEPTION(EX_CLASS, ...)  \
throw EX_CLASS(__VA_ARGS__).set_func_name(__PRETTY_FUNCTION__).set_file_name(__FILE__).set_line_number(__LINE__);


}
}

#endif //__CHAT_DEMO_EXCEPTION_HPP_
