#include "common.h"

static inline uint64_t get_core_id(void) {
    uint64_t core_id;
    __asm__ __volatile__ (
        "mrs %0, mpidr_el1"
        : "=r" (core_id)
    );
    return core_id & 0x3;
}