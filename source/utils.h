#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdarg.h>

typedef enum logLevel {
DEBUG   = 3,
INFO    = 2,
WARNING = 1,
ERROR   = 0,
TEST    = -100
} LogLevel;


extern LogLevel level;


int str2int(const char* cc);
char *trim (char *s);

void _log(const LogLevel level, const int id, const char *fmt, ...)
    __attribute__((format (printf, 3, 4)));


#define log(level, id, fmt, ...) _proxy_log(level, id, fmt"\n", ##__VA_ARGS__)

#endif //__UTILS_H__
