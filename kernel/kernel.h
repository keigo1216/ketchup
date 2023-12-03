#include "common.h"
#include "asm.h"
#include "print.h"

#define PANIC(fmt, ...) \
    do { \
        printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        while(1) {} \
    } while(0)

#define WRITE_CSR(reg, value)                                                  \
    __asm__ __volatile__("msr " #reg ", %0" ::"r"(value))

#define READ_CSR(reg)                                                         \
    ({                                                                         \
        uint64_t value;                                                        \
        __asm__ __volatile__("mrs %0, " #reg : "=r"(value));                   \
        value;                                                                 \
    })