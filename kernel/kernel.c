#include "kernel.h"

extern char __bss[], __bss_end[], __stack_top[];

void kernel_main() {
    unsigned int core_id = get_core_id();
    if (core_id == 0) {
        printf("Hello, world!%d\n", 123);
        if (!strcmp("abc", "abc")) {
            printf("strcmp OK\n");
        } else {
            printf("strcmp NG\n");
        }
        PANIC("booted!");
        PANIC("unreachable");
    } else {
        while(1) {}
    }
    while (1) {}
}

__attribute__((section(".text.boot")))
__attribute__((naked))
void boot(void) {
    __asm__ __volatile__(
        "ldr X8, =__stack_top\n"
        "mov sp, X8\n"
        "bl kernel_main\n"
        :
        :
        : "r0"
    );
}