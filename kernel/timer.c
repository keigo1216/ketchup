#include "timer.h"

void init_timer_handler() {
    set_timer(); // set timer
    enable_timer(); // enable timer
}

void enable_timer() {
    __asm__ __volatile__ (
        // enable timer register
        "mov x0, #1\n\t"
        "msr cntv_ctl_el0, x0\n\t"
        // enable GIC interrupt
        "msr DAIFClr, #2\n\t"
        // enable timer interrupt
        "LDR X1, =0x40000000\n\t"
        "MOV X2, #0x40\n\t"
        "ADD X1, X1, X2\n\t"
        "MOV X2, #2\n\t"
        "STR X2, [X1]\n\t"
    );
}

void set_timer() {
    uint64_t cntfrq_el0 = get_cntfrq_el0(); // get timer freq

    uint64_t next_tick = cntfrq_el0; // set next tick
    set_cntp_cval_el0(next_tick);

    // clear timer interrupt
    __asm__ __volatile__ (
        "ldr x0, =0x01\n\t"
        "msr cntp_ctl_el0, x0\n\t"
    );
}