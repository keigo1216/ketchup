#include "user.h"

extern char __stack_top[];

__attribute__((noreturn)) void exit(void) {
    for (;;);
}

int syscall(uint64_t sysno, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6) {
    register uint64_t x0 __asm__("x0") = sysno;
    register uint64_t x1 __asm__("x1") = arg1;
    register uint64_t x2 __asm__("x2") = arg2;
    register uint64_t x3 __asm__("x3") = arg3;
    register uint64_t x4 __asm__("x4") = arg4;
    register uint64_t x5 __asm__("x5") = arg5;
    register uint64_t x6 __asm__("x6") = arg6;

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

process_t process_create(uint64_t kernel_entry) {
    return syscall(SYS_CREATE_PROCESS, kernel_entry, 0, 0, 0, 0, 0);
}

void map_page(process_t pid, uint64_t vaddr, uint64_t paddr, uint64_t flags) {
    syscall(SYS_MAP_PAGE, pid, vaddr, paddr, flags, 0, 0);
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