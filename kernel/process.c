#include "process.h"

struct process procs[PROCS_MAX];
struct process *current_proc;
struct process idle_proc;
list_t runqueue = LIST_INIT(runqueue);

/*
    Round robin method
*/
static struct process *scheduler(void) {
    struct process *next = LIST_POP_FRONT(&runqueue, struct process, waitqueue_next);

    // If find next process
    if (next) {
        return next;
    }

    // TODO : Add cheking current_proc is not destroyed
    if (current_proc->state == PROC_RUNNABLE) {
        return current_proc;
    }

    return &idle_proc;
} 

void yeild(void) {
    // 実行待ちになっているプロセスを探す
    struct process *prev_proc = current_proc;
    struct process *next_proc = scheduler();

    if (next_proc != &idle_proc) {
        next_proc->left_time = INIT_TIME_QUANTUM;
    }
    
    // 実行待ちのプロセスが存在しなかったら譲らずに実行を続ける
    if (next_proc == current_proc) {
        return;
    }

    if (prev_proc->state == PROC_RUNNABLE) {
        list_push_back(&runqueue, &prev_proc->waitqueue_next);
    }

    /*
        コンテキストスイッチ
    */
    current_proc = next_proc;
    printf("switch from %d to %d\n", prev_proc->pid, next_proc->pid);

    // TODO : split hardware dependent code
    // ユーザ空間用のページテーブルを入れ替える
    // TODO : ユーザー空間を作るときにこの部分を実装する（無効なページテーブルを指定してしまうとプログラムが動かなくなる）
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

void init_process_struct(struct process *proc, int pid) {
    // TODO : split hardware dependent code
    // initialize stack pointer for new process
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

    // TODO : no need to set user page to idel process
    // map page for user space
    uint64_t *page_table = (uint64_t *)alloc_pages(1);

    // printf("page_table = %x\n", page_table);

    // Initialize process struct
    proc->pid = pid;
    proc->state = PROC_BLOCKED;
    proc->wait_for = IPC_ANY;
    proc->sp = (uint64_t) sp;
    proc->page_table = page_table;
    proc->left_time = 0;

    list_init(&proc->senders);
    list_elem_init(&proc->waitqueue_next);
}

/*
    alloc pid (range 1 to PROCS_MAX)
    when return 0, it is error
*/
static process_t alloc_pid(void) {
    for (process_t pid = 0; pid < PROCS_MAX; pid++ ) {
        if (procs[pid].state == PROC_UNUSED) {
            return pid + 1;
        }
    }

    return 0; // error
}

struct process *process_create() {
    process_t pid = alloc_pid();
    if (!pid) {
        PANIC("no free process slots");
    }

    struct process *proc = &procs[pid-1];

    init_process_struct(proc, pid);
    // TODO : error handling

    process_resume(proc);
    
    // TODO : return pid
    return proc;
}

void process_block(struct process *proc) {
    proc -> state = PROC_BLOCKED;
}

void process_resume(struct process *proc) {
    proc -> state = PROC_RUNNABLE;

    list_push_back(&runqueue, &proc->waitqueue_next);
}

struct process *process_find(process_t pid) {
    if (pid <= 0 && pid > PROCS_MAX) {
        // TODO : return error
        PANIC("invalid pid");
    }

    return &procs[pid - 1];
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