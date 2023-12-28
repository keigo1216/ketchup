#ifndef __ALLOC_H__
#define __ALLOC_H__

#include "common.h"
#include "print.h"
#include "armv8def.h"

// #define KERNEL_BASE_ADDR 0xffff000000000000

paddr_t alloc_pages(uint32_t n);


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