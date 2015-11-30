#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX


LogLevel level=DEBUG;

void _proxy_log(LogLevel msglev, int id, const char *fmt, ...)
{
    if(msglev > level)
    {
        return;
    }

    va_list arg;

    char colbuf[10] = "\0";
    char buffer[1024];

    va_start(arg, fmt);
    vsnprintf(buffer, sizeof buffer, fmt, arg);
    va_end(arg);

    if (++id != 0)
    {
        sprintf(colbuf, "\x1B[3%dm", id);
    }

    printf("%s%s\033[0m", colbuf, buffer);
}

char *trim (char *s)
{
    int i = strlen(s)-1;
    if ((i > 0) && (s[i] == '\n'))
    {
        s[i] = '\0';
    }

    return s;
}

int str2int(const char* cc)
{
    char *p;

    errno = 0;

    long conv = strtol(cc, &p, 10);

    if (errno != 0 || *p != '\0' || conv > INT_MAX)
    {
        usage();
    }
    else
    {
        return conv;
    }
}

