#include "kernel.h"

extern char __bss[], __bss_end[], __free_ram[], __free_ram_end[];

struct process *proc_a;
struct process *proc_b;

void proc_a_entry(void) {
    printf("starting proc_a\n");
    printf("sp address: %x\n", &proc_a->sp);
    printf("sp register: %x\n", get_sp());
    while (1) {
        putchar('A');
        switch_context(&proc_a->sp, &proc_b->sp);

        for (int i = 0; i < 1000000; i++) {
            __asm__ __volatile__("nop");
        }
    }
}

void proc_b_entry(void) {
    printf("starting proc_b\n");
    printf("sp address: %x\n", proc_b->sp);
    printf("sp register: %x\n", get_sp());
    while (1) {
        putchar('B');
        switch_context(&proc_b->sp, &proc_a->sp);

        for (int i = 0; i < 1000000; i++) {
            __asm__ __volatile__("nop");
        }
    }
}

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
        printf("current el = %d\n", get_current_el());
        memset(__bss, 0, __bss_end - __bss);

        // paddr_t paddr0 = alloc_pages(2);
        // paddr_t paddr1 = alloc_pages(1);
        // printf("alloc_pages test: paddr0=%x\n", paddr0);
        // printf("alloc_pages test: paddr1=%x\n", paddr1);

        proc_a = create_process((uint64_t) proc_a_entry);
        proc_b = create_process((uint64_t) proc_b_entry);
        proc_a_entry();

        PANIC("booted!");
        PANIC("unreachable");
    } else {
        while(1) {}
    }
    while (1) {}
}