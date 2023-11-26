#include "common.h"
#include "asm.h"
#include "print.h"

#define PANIC(fmt, ...) \
    do { \
        printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        while(1) {} \
    } while(0)