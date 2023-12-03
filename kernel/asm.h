#include "common.h"

static inline uint64_t get_core_id(void) {
    uint64_t core_id;
    __asm__ __volatile__ (
        "mrs %0, mpidr_el1"
        : "=r" (core_id)
    );
    return core_id & 0x3;
}

uint64_t get_current_el() {
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