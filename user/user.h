#ifndef __USER_H__
#define __USER_H__

#include "common.h"

__attribute__((noreturn)) void exit(void);
void putchar(char ch);
int syscall(int sysno, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6);

#endif // __USER_H__