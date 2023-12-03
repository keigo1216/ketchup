#include "kernel.h"

extern char __bss[], __bss_end[], __free_ram[], __free_ram_end[];

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

// paddr_t alloc_pages(uint32_t n) {
//     static paddr_t next_paddr = (paddr_t) __free_ram;
//     paddr_t paddr = next_paddr;
//     next_paddr += n * PAGE_SIZE;

//     if (next_paddr > (paddr_t) __free_ram_end) {
//         PANIC("out of memory");
//     }

//     memset((void *)paddr, 0, n * PAGE_SIZE);
//     return paddr;
// }

void kernel_main() {
    unsigned int core_id = get_core_id();
    if (core_id == 0) {
        // clear bss
        memset(__bss, 0, __bss_end - __bss);

        paddr_t paddr0 = alloc_pages(2);
        paddr_t paddr1 = alloc_pages(1);
        printf("alloc_pages test: paddr0=%x\n", paddr0);
        printf("alloc_pages test: paddr1=%x\n", paddr1);

        PANIC("booted!");
        PANIC("unreachable");
    } else {
        while(1) {}
    }
    while (1) {}
}