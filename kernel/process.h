#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "common.h"
#include "print.h"
#include "alloc.h"
#include "asm.h"
#include "common.h"

#define USER_BASE 0x1000000

struct process {
    int pid;                // プロセスID
    int state;              // プロセスの状態
    vaddr_t sp;             // コンテキストスイッチ時のスタックポインタ  
    uint64_t *page_table;   // ページテーブル   
    uint8_t stack[8192];    // カーネルスタック
    uint32_t left_time;     // 残り実行時間
};

void yeild(void);

__attribute__((naked))
void switch_context (uint64_t *prev_sp, uint64_t *next_sp);

struct process *create_process(const void *image, size_t image_size);

void handle_timer_irq(void);

extern struct process procs[PROCS_MAX];
extern struct process *current_proc;
extern struct process *idle_proc;

#endif // __PROCESS_H__