#include "kernel.h"

extern char __bss[], __bss_end[], __stack_top_el2[], __stack_top_el1[];

void handle_trap(void) {
    PANIC("handle_trap");
}

void handle_syscall(void) {
    PANIC("handle_syscall");
}

void kernel_main() {
    unsigned int core_id = get_core_id();
    if (core_id == 0) {
        // clear bss
        memset(__bss, 0, __bss_end - __bss);

        printf("mode: %d\n", get_current_el());
        printf("Hello, world!%d\n", 123);
        if (!strcmp("abc", "abc")) {
            printf("strcmp OK\n");
        } else {
            printf("strcmp NG\n");
        }

        __asm__ __volatile__ ("svc #0");
        
        PANIC("booted!");
        PANIC("unreachable");
    } else {
        while(1) {}
    }
    while (1) {}
}