#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "common.h"
#include "asm.h"
#include "print.h"
#include "alloc.h"
#include "process.h"
#include "test.h"

#define WRITE_CSR(reg, value)                                                  \
    __asm__ __volatile__("msr " #reg ", %0" ::"r"(value))

#define READ_CSR(reg)                                                         \
    ({                                                                         \
        uint64_t value;                                                        \
        __asm__ __volatile__("mrs %0, " #reg : "=r"(value));                   \
        value;                                                                 \
    })

extern char _binary_shell_bin_start[], _binary_shell_bin_size[], _binary_shell_bin_end[];

#endif // __KERNEL_H__