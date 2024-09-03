#ifndef __USER_H__
#define __USER_H__

#include "common.h"

__attribute__((noreturn)) void exit(void);
void putchar(char ch);
process_t process_create(uint64_t kernel_entry);
void map_page(process_t pid, uint64_t vaddr, uint64_t paddr, uint64_t flags);
int syscall(uint64_t sysno, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6);

#endif // __USER_H__