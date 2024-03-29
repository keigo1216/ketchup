#include "process.h"

struct process procs[PROCS_MAX];
struct process *current_proc;
struct process *idle_proc;

void yeild(void) {
    // 実行待ちになっているプロセスを探す
    struct process *next_proc = idle_proc;
    for (int i = 0; i < PROCS_MAX; i++) {
        struct process *proc = &procs[(current_proc->pid + i) % PROCS_MAX];
        if (proc->state == PROC_RUNNABLE && proc->pid > 0) {
            next_proc = proc;
            break;
        }
    }
    
    // 実行待ちのプロセスが存在しなかったら譲らずに実行を続ける
    if (next_proc == current_proc) {
        return;
    }

    /*
        コンテキストスイッチ
    */
    struct process *prev_proc = current_proc;
    current_proc = next_proc;
    printf("switch from %d to %d\n", prev_proc->pid, next_proc->pid);
    // ユーザ空間用のページテーブルを入れ替える
    // To do: ユーザー空間を作るときにこの部分を実装する（無効なページテーブルを指定してしまうとプログラムが動かなくなる）
    set_ttrbr0_el1(((uint64_t)next_proc->page_table - KERNEL_BASE_ADDR));
    switch_context(&prev_proc->sp, &next_proc->sp);
}

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
        "ldr x0, =0x00000000\n\t"
        "msr spsr_el1, x0\n\t"
        "ldr x0, [sp], #8\n\t"
        "msr elr_el1, x0\n\t"
        "eret\n\t"
    );
}

struct process *create_process(const void *image, size_t image_size) {
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
    *(--sp) = (uint64_t)USER_BASE; // プログラムカウンタ
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

    uint64_t *page_table = (uint64_t *)alloc_pages(1);
    // ユーザーのページをマッピングする
    for (usize64_t off = 0; off < image_size; off += PAGE_SIZE) {
        paddr_t page = alloc_pages(1);
        // printf("page = %x\n", page);
        memcpy((void *)page, image + off, PAGE_SIZE);
        map_page(page_table, USER_BASE + off, page, PAGE_RW | PAGE_ACCESS);
    }

    // printf("page_table = %x\n", page_table);

    proc->pid = i + 1;
    proc->state = PROC_RUNNABLE;
    proc->sp = (uint64_t) sp;
    proc->page_table = page_table;
    proc->left_time = INIT_TIME_QUANTUM;
    return proc;
}

void handle_timer_irq(void) {
    if (current_proc->pid > 0) {
        current_proc->left_time--;
        if (current_proc->left_time <= 0) {
            current_proc->left_time = INIT_TIME_QUANTUM;
            yeild();
        }
    }
}