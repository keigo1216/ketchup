#ifndef __ALLOC_H__
#define __ALLOC_H__

#include "common.h"
#include "print.h"
#include "armv8def.h"
#include "process.h"

// #define KERNEL_BASE_ADDR 0xffff000000000000

paddr_t alloc_pages(uint32_t n);

/*
    * map page to page table
    *
    * @param proc current process 
    * @param vaddr 
    * @param paddr
    * @param flags
*/
void vm_map(
    struct process *proc,
    uint64_t vaddr,
    uint64_t paddr,
    uint64_t flags 
);

/*
    * ページテーブルにページをマップする
    * 
    * @param table1 一段目のページテーブルのベースアドレス
    * @param vaddr 仮想アドレス
    * @param paddr 物理アドレス
    * @param flags ページ属性
*/
void map_page(
    uint64_t *table1,
    uint64_t vaddr,
    uint64_t paddr,
    uint64_t flags
);

extern char __free_ram[], __free_ram_end[];
#endif // __ALLOC_H__