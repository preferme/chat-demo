//
// Created by NAVER on 2023/3/10.
//

#ifndef CPP_CERROREXCEPTION_H
#define CPP_CERROREXCEPTION_H

#include "Exception.h"

namespace chat {
namespace exception {

class CErrorException : public Exception {
public:
    friend std::ostream& operator<<(std::ostream& os, CErrorException& ex);

    CErrorException();
    int getErrorNo() const;

private:
    int errorno;
};

}
}
#endif //CPP_CERROREXCEPTION_H
