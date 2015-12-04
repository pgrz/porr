#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdarg.h>

typedef enum logLevel {
TEST    = 10,
DEBUG   = 3,
INFO    = 2,
WARNING = 1,
ERROR   = 0
} LogLevel;


extern LogLevel level;


int str2int(const char* cc);
char *trim (char *s);

void _log(const LogLevel level, const int id, const char *fmt, ...)
    __attribute__((format (printf, 3, 4)));


#define log(level, id, fmt, ...) _proxy_log(level, id, "%s(%d):%s() "fmt"\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#endif //__UTILS_H__
