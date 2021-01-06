#ifndef ___HNET___LOG____
#define ___HNET___LOG____
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
namespace hnet
{
static void __inline hnet_log(const char* fmt, ...)
{
//    time_t t = time(NULL);
    va_list args;
    va_start(args, fmt);
    printf(fmt, args);
    va_end(args);
    printf("\n");
}

#define HENT_LOG(fmt, ...) printf("[HNET:%d]:" fmt "[%s.%d]\n" , time(NULL), ##__VA_ARGS__,__FILE__,__LINE__)
//printf("[HNET:%l.%s.%d]:",time(NULL), fmt ,__func__ ,__LINE__, ##__VA_ARGS__)
}
#endif
