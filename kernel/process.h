#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "common.h"
#include "print.h"

struct process {
    int pid;                // プロセスID
    int state;              // プロセスの状態
    vaddr_t sp;             // コンテキストスイッチ時のスタックポインタ     
    uint8_t stack[8192];    // カーネルスタック
};

void yeild(void);

__attribute__((naked))
void switch_context (uint64_t *prev_sp, uint64_t *next_sp);

struct process *create_process(uint64_t pc);

extern struct process procs[PROCS_MAX];
extern struct process *current_proc;
extern struct process *idle_proc;

#endif // __PROCESS_H__