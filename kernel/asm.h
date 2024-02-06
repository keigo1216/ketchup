#ifndef __ASM_H__
#define __ASM_H__

#include "common.h"

static inline uint64_t get_core_id(void) {
    uint64_t core_id;
    __asm__ __volatile__ (
        "mrs %0, mpidr_el1"
        : "=r" (core_id)
    );
    return core_id & 0x3;
}

static inline uint64_t get_current_el() {
    uint64_t el;
    __asm__ (
        "mrs %0, CurrentEL\n\t"
        "lsr %0, %0, #2"
        : "=r" (el) /* 出力オペランド */
        : /* 入力オペランドはなし */
        : /* 破壊されるレジスタはなし */
    );
    return el;
}

static inline uint64_t get_cntp_tval_el0(void) {
    uint64_t cntp_tval;
    __asm__ (
        "mrs %0, CNTP_TVAL_EL0"
        : "=r" (cntp_tval)
    );
    return cntp_tval;
}

static inline uint64_t get_cntp_ctl_el0(void) {
    uint64_t cntp_ctl_el0;
    __asm__ (
        "mrs %0, cntv_ctl_el0"
        : "=r" (cntp_ctl_el0)
    );
    return cntp_ctl_el0;
}

static inline uint64_t get_esr_el2(void) {
    uint64_t esr;
    __asm__ __volatile__ (
        "mrs %0, esr_el2"
        : "=r" (esr)
    );
    return esr;
}

/*
    ESR_EL1 registers is 32bit register.
    +-----------+---------+------------+
    | EC [31:26]| IL [25] | ISS [24:0] |
    +-----------+---------+------------+

    EC: Exception Class. Indicates the reason for the exception.
    - 0x15: SVC instruction execution
*/
static inline uint64_t get_esr_el1(void) {
    uint64_t esr;
    __asm__ __volatile__ (
        "mrs %0, esr_el1"
        : "=r" (esr)
    );
    return esr;
}

static inline uint64_t get_far_el1(void) {
    uint64_t far;
    __asm__ __volatile__ (
        "mrs %0, far_el1"
        : "=r" (far)
    );
    return far;
}

static inline uint64_t get_elr_el1(void) {
    uint64_t elr;
    __asm__ __volatile__ (
        "mrs %0, elr_el1"
        : "=r" (elr)
    );
    return elr;
}

static inline uint64_t get_sp(void) {
    uint64_t sp;
    __asm__ __volatile__ (
        "mov %0, sp"
        : "=r" (sp)
    );
    return sp;
}

static inline void set_sp(uint64_t sp) {
    __asm__ __volatile__ (
        "mov sp, %0"
        : /* 出力オペランドはなし */
        : "r" (sp) /* 入力オペランド */
        : /* 破壊されるレジスタはなし */
    );
}

static inline void set_ttrbr0_el1(uint64_t ttbr0) {
    __asm__ __volatile__ (
        "msr ttbr0_el1, %0"
        : /* 出力オペランドはなし */
        : "r" (ttbr0) /* 入力オペランド */
        : /* 破壊されるレジスタはなし */
    );
}

static inline uint64_t get_cntfrq_el0(void) {
    uint64_t cntfrq;
    __asm__ __volatile__ (
        "mrs %0, cntfrq_el0"
        : "=r" (cntfrq)
    );
    return cntfrq;
}

static inline uint64_t get_cntpct_el0(void) {
    uint64_t cntpct;
    __asm__ __volatile__ (
        "mrs %0, cntpct_el0"
        : "=r" (cntpct)
    );
    return cntpct;
}

static inline void set_cntp_cval_el0(uint64_t cntp_cval) {
    __asm__ __volatile__ (
        "msr cntv_tval_el0, %0"
        : /* 出力オペランドはなし */
        : "r" (cntp_cval) /* 入力オペランド */
        : /* 破壊されるレジスタはなし */
    );
}

static inline uint64_t get_cntp_cval_el0(void) {
    uint64_t cntp_cval;
    __asm__ __volatile__ (
        "mrs %0, cntv_cval_el0"
        : "=r" (cntp_cval)
    );
    return cntp_cval;
}

static inline uint64_t get_core0_interrupt_source(void) {
    return *((uint64_t *)0x40000060);
}

//https://developer.arm.com/documentation/ddi0595/2021-03/AArch64-Registers/ICC-IAR0-EL1--Interrupt-Controller-Interrupt-Acknowledge-Register-0?lang=en
// static inline uint64_t get_INTID(void) {
//     // uint64_t INTID;
//     // __asm__ __volatile__ (
//     //     "mrs %0, ICC_IAR0_EL1"
//     //     : "=r" (INTID)
//     // );
//     // return INTID;
//     // MRS <Xt>, ICC_NMIAR1_EL1
//     uint64_t int_id;
//     // __asm__ __volatile__ (
//     //     "mrs %0, ICC_IAR1_EL1"
//     //     : "=r" (int_id)
//     // );
//     __asm__ __volatile__(
//         "mrs %0, ICC_IAR1_EL1" 
//         : "=r" (int_id)
//     );
//     return int_id;
// }

#endif // __ASM_H__