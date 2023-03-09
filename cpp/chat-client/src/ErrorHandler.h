//
// Created by NAVER on 2023/3/9.
//

#ifndef CPP_ERRORHANDLER_H
#define CPP_ERRORHANDLER_H


struct ErrorHandler {

    virtual void doHandler(const int error) = 0;

};


#endif //CPP_ERRORHANDLER_H
