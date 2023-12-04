#include "process.h"

struct process procs[PROCS_MAX];

__attribute__((naked))
void switch_context (uint64_t *prev_sp, uint64_t *next_sp) {
    __asm__ __volatile__ (
        "stp x20, x19, [sp, #-16]!\n"
        "stp x22, x21, [sp, #-16]!\n"
        "stp x24, x23, [sp, #-16]!\n"
        "stp x26, x25, [sp, #-16]!\n"
        "stp x28, x27, [sp, #-16]!\n"
        "stp x30, x29, [sp, #-16]!\n"
        "mov x9, sp\n"
        "str x9, [x0]\n"
        "ldr x9, [x1]\n"
        "mov sp, x9\n"
        "ldp x30, x29, [sp], #16\n"
        "ldp x28, x27, [sp], #16\n"
        "ldp x26, x25, [sp], #16\n"
        "ldp x24, x23, [sp], #16\n"
        "ldp x22, x21, [sp], #16\n"
        "ldp x20, x19, [sp], #16\n"
        "ret\n"
    );
}

__attribute__((naked))
void start_task(void) {
    __asm__ __volatile__ (
        "ldr x0, =0x00000005\n\t"
        "msr spsr_el1, x0\n\t"
        "ldr x0, [sp], #8\n\t"
        "msr elr_el1, x0\n\t"
        "eret\n\t"
    );
}

struct process *create_process(uint64_t pc) {
    // 空いているプロセス構造体を探す
    struct process *proc = NULL;
    int i;
    for (i = 0; i < PROCS_MAX; i++) {
        if (procs[i].state == PROC_UNUSED) {
            proc = &procs[i];
            break;
        }
    }

    if (!proc) {
        PANIC("no free process slots");
    }

    uint64_t *sp = (uint64_t *) &proc->stack[sizeof(proc->stack)];
    *(--sp) = pc; // プログラムカウンタ
    *(--sp) = 0;  // X19
    *(--sp) = 0;  // X20
    *(--sp) = 0;  // X21
    *(--sp) = 0;  // X22
    *(--sp) = 0;  // X23
    *(--sp) = 0;  // X24
    *(--sp) = 0;  // X25
    *(--sp) = 0;  // X26
    *(--sp) = 0;  // X27
    *(--sp) = 0;  // X28
    *(--sp) = 0;  // X29
    *(--sp) = (uint64_t) start_task;  // X30 (LR)　retでのリターンアドレスを入れるところ

    proc->pid = i + 1;
    proc->state = PROC_RUNNABLE;
    proc->sp = (uint64_t) sp;
    return proc;
}