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
#include <ostream>

namespace chat {
namespace exception {

class Exception : public std::exception {
public:
    explicit Exception(const std::string message);
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

    friend std::ostream& operator<<(std::ostream& os, Exception& ex);
protected:
    std::string message;
    std::string funName;
    std::string fileName;
    int lineNumber;
};

// utils for throw exception
#define THROW_EXCEPTION(EX_CLASS, ...)  \
throw EX_CLASS(__VA_ARGS__).setFunName(__PRETTY_FUNCTION__).setFileName(__FILE__).setLineNumber(__LINE__);

}
}

#endif /* __EXCEPTION_EXCEPTION_H_ */
