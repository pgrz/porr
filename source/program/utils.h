#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdarg.h>

int str2int(const char* cc);
char *trim (char *s);

void _log(const int id, const char *fmt, ...)
    __attribute__((format (printf, 2, 3)));

#define log(id, fmt, ...) _proxy_log(id, fmt"\n", ##__VA_ARGS__)

#endif //__UTILS_H__
