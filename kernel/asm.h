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

static inline uint64_t get_esr_el2(void) {
    uint64_t esr;
    __asm__ __volatile__ (
        "mrs %0, esr_el2"
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

#endif // __ASM_H__