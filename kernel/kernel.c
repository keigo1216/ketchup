#include "kernel.h"

extern char __bss[], __bss_end[], __stack_top_el2[], __stack_top_el1[];

void handle_trap(void) {
    uint64_t esr_el1 = get_esr_el2();
    uint64_t far_el1 = get_far_el1();
    uint64_t elr_el1 = get_elr_el1();

    uint64_t ec = esr_el1 >> 26;

    switch (ec) {
        case 0x15: // EC = 0x15, Instruction abort from lower EL
            PANIC("Instruction abort from lower EL\n");
            break;
        default:
            PANIC("unexpected trap ec=%x, far=%x, elr=%x\n", ec, far_el1, elr_el1);
            break;
    }

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