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

void _proxy_log(const LogLevel level, const int id, const char *fmt, ...)
    __attribute__((format (printf, 3, 4)));


#define log(level, id, fmt, ...) _proxy_log(level, id, "\x1B[3%dmT%d: %s(%d):%s() "fmt"\033[0m", id + 1, id,  __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define log_d(id, fmt, ...) log(DEBUG, id, fmt, ##__VA_ARGS__)
#define log_i(id, fmt, ...) log(INFO, id, fmt, ##__VA_ARGS__)
#define log_w(id, fmt, ...) log(WARNING, id, fmt, ##__VA_ARGS__)
#define log_e(id, fmt, ...) log(ERROR, id, fmt, ##__VA_ARGS__)
#define log_t(id, fmt, ...) _proxy_log(TEST, id, "T%d: "fmt, id, ##__VA_ARGS__)

#endif //__UTILS_H__
