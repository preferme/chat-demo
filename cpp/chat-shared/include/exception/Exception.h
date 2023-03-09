/*
 * Exception.h
 *
 *  Created on: 2023年3月6日
 *      Author: hou-lei
 */

#ifndef __EXCEPTION_EXCEPTION_H_
#define __EXCEPTION_EXCEPTION_H_

#include <exception>
#include <string>

namespace chat {
namespace exception {

class Exception : public std::exception {
public:
    explicit Exception(const std::string& message);
    Exception(const std::string message, const std::string funName, const std::string file, int lineNumber);
    virtual ~Exception();

    // @Override from std::exception
    const char* what() const noexcept override;

    Exception& setMessage(std::string message);
    std::string& getMessage();

    Exception& setFunName(std::string funName);
    std::string& getFunName();

    Exception& setFileName(std::string fileName);
    std::string& getFileName();

    Exception& setLineNumber(int lineNumber);
    int getLineNumber();

protected:
    std::string message;
    std::string funName;
    std::string fileName;
    int lineNumber;
};

// utils for throw exception
#define THROW(EX_CLASS, ...)     \
throw EX_CLASS(__VA_ARGS__).setFunName(__PRETTY_FUNCTION__).setFileName(__FILE__).setLineNumber(__LINE__);

#include <ctime>
#include <cstdio>
// utils for print exception form THROW(EX_CLASS, ...)
#define PRINT(EX)                                        \
char fmt[32] = { 0 }; tm stime {0};                      \
time_t timer = std::time(0);                             \
localtime_r(&timer, &stime);                             \
strftime(fmt, sizeof(fmt), "%Y-%m-%d %H:%M:%S", &stime); \
fprintf(stderr, "%s\t%30s:%-5d[%s] : %s\n", fmt, ex.getFileName().c_str(), ex.getLineNumber(), ex.getFunName().c_str(), ex.getMessage().c_str());


}
}

#endif /* __EXCEPTION_EXCEPTION_H_ */
