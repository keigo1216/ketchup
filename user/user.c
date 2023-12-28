#include "user.h"

extern char __stack_top[];

__attribute__((noreturn)) void exit(void) {
    for (;;);
}

int syscall(int sysno, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6) {
    register int x0 __asm__("x0") = sysno;
    register int x1 __asm__("x1") = arg1;
    register int x2 __asm__("x2") = arg2;
    register int x3 __asm__("x3") = arg3;
    register int x4 __asm__("x4") = arg4;
    register int x5 __asm__("x5") = arg5;
    register int x6 __asm__("x6") = arg6;

    __asm__ __volatile__ (
        "svc #0\n\t"
        : "+r"(x0)
        : "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5), "r"(x6)
        : "memory"
    );

    return x0;
}

void putchar(char c) {
    syscall(SYS_PUTCHAR, c, 0, 0, 0, 0, 0);
}

__attribute__((section(".text.start")))
__attribute__((naked))
void start(void) {
    __asm__ __volatile__ (
        "ldr x0, =__stack_top\n\t"
        "mov sp, x0\n\t"
        "bl main\n\t"
        "bl exit\n\t"
    );
}