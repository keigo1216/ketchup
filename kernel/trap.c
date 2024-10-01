#include "trap.h"

void interrupt_trap(void) {
    uint64_t int_id = get_core0_interrupt_source();
    if (int_id == 0x02) { // CNTVIRQ interrupt (Non-secure physical timer event)
        set_timer();
        handle_timer_irq();
    } else {
        printf("unknown interrupt: int_id=%x\n", int_id); // To do: implement interrupt handler
    }
}

void handle_trap(void) {
    uint64_t esr_el1 = get_esr_el1();
    uint64_t far_el1 = get_far_el1();
    uint64_t elr_el1 = get_elr_el1();

    uint64_t ec = esr_el1 >> 26;

    /*
        Document of EC fields
        https://developer.arm.com/documentation/ddi0595/2020-12/AArch64-Registers/ESR-EL1--Exception-Syndrome-Register--EL1-?lang=en#fieldset_0-31_26
    */

    switch (ec) {
        // TODO: define macro
        case (uint64_t)(0b100000): // PAGE Fault
            printf("Page falut");
            break;
        default:
            PANIC("unexpected trap ec=%x, far=%x, elr=%x\n", ec, far_el1, elr_el1);
            break;
    }
}