//
// Created by NAVER on 2023/3/9.
//

#ifndef CPP_POLLEVENTHANDLER_H
#define CPP_POLLEVENTHANDLER_H

typedef void (*PollEventsHandler)(const int fd, const short events, const short revents);

#endif //CPP_POLLEVENTHANDLER_H
