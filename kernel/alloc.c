#include "alloc.h"

paddr_t alloc_pages(uint32_t n) {
    static paddr_t next_paddr = (paddr_t) __free_ram;
    paddr_t paddr = next_paddr;
    next_paddr += n * PAGE_SIZE;

    if (next_paddr > (paddr_t) __free_ram_end) {
        PANIC("out of memory");
    }

    memset((void *)paddr, 0, n * PAGE_SIZE);
    return paddr;
}

void vm_map(
    struct process *proc,
    uint64_t vaddr,
    uint64_t paddr,
    uint64_t flags 
) {
    // page table pointer
    uint64_t *table0 = proc->page_table;

    if (!is_aligned(vaddr, PAGE_SIZE)) {
        PANIC("vaddr must be aligned by page size");
    }
    if (!is_aligned(paddr, PAGE_SIZE)) {
        PANIC("paddr must be aligned by page size");
    }

    /*
        仮想アドレスの構成
        +------+--------+------+------+------+-------+-----------+
        | 4bit | ignore | 9bit | 9bit | 9bit |  9bit |   12bit   |
        | TTBR | ignore |  L0  |  L1  |  L2  |  L3   |  PA[12:0] |
        +------+--------+------+------+------+-------+-----------+
    */

    uint64_t l0_index = (vaddr >> 39) & 0x1ff;
    uint64_t l1_index = (vaddr >> 30) & 0x1ff;
    uint64_t l2_index = (vaddr >> 21) & 0x1ff;
    uint64_t l3_index = (vaddr >> 12) & 0x1ff;

    /*
        Create L0 table entry
    */
    // check whether L0 table entry is present
    if ((table0[l0_index] & PAGE_V) == 0) {
        // L0 table entry is not present, so allocate L1 table
        paddr_t l1_paddr = alloc_pages(1);
        table0[l0_index] = (l1_paddr | PD_TABLE) - KERNEL_BASE_ADDR;
    }

    /*
        Create L1 table entry
    */
   uint64_t *table1 = (uint64_t *)(table0[l0_index] & PAGE_NEXT_TABLE_ADDR_MASK);
    // check whether L1 table entry is present
    if ((table1[l1_index] & PAGE_V) == 0) {
        // L1 table entry is not present, so allocate L2 table
        paddr_t l2_paddr = alloc_pages(1);
        table1[l1_index] = (l2_paddr | PD_TABLE) - KERNEL_BASE_ADDR;
    }

    /*
        Create L2 table entry
    */
    uint64_t *table2 = (uint64_t *)(table1[l1_index] & PAGE_NEXT_TABLE_ADDR_MASK);
    // check whether L2 table entry is present
    if ((table2[l2_index] & PAGE_V) == 0) {
        // L2 table entry is not present, so allocate L3 table
        paddr_t l3_paddr = alloc_pages(1);
        table2[l2_index] = (l3_paddr | PD_TABLE) - KERNEL_BASE_ADDR;
    }

    /*
        Create L3 table entry
    */
    uint64_t *table3 = (uint64_t *)(table2[l2_index] & PAGE_NEXT_TABLE_ADDR_MASK);
    table3[l3_index] = (paddr | flags | PAGE_ENTRY | PD_ACCESS) - KERNEL_BASE_ADDR;

    return;
}

void map_page(
    uint64_t *table0,
    uint64_t vaddr,
    uint64_t paddr,
    uint64_t flags
) { 
    if (!is_aligned(vaddr, PAGE_SIZE)) {
        PANIC("vaddr must be aligned by page size");
    }
    if (!is_aligned(paddr, PAGE_SIZE)) {
        PANIC("paddr must be aligned by page size");
    }

    /*
        仮想アドレスの構成
        +------+--------+------+------+------+-------+-----------+
        | 4bit | ignore | 9bit | 9bit | 9bit |  9bit |   12bit   |
        | TTBR | ignore |  L0  |  L1  |  L2  |  L3   |  PA[12:0] |
        +------+--------+------+------+------+-------+-----------+
    */

    uint64_t l0_index = (vaddr >> 39) & 0x1ff;
    uint64_t l1_index = (vaddr >> 30) & 0x1ff;
    uint64_t l2_index = (vaddr >> 21) & 0x1ff;
    uint64_t l3_index = (vaddr >> 12) & 0x1ff;

    /*
        Create L0 table entry
    */
    // check whether L0 table entry is present
    if ((table0[l0_index] & PAGE_V) == 0) {
        // L0 table entry is not present, so allocate L1 table
        paddr_t l1_paddr = alloc_pages(1);
        table0[l0_index] = (l1_paddr | PD_TABLE) - KERNEL_BASE_ADDR;
    }

    /*
        Create L1 table entry
    */
   uint64_t *table1 = (uint64_t *)(table0[l0_index] & PAGE_NEXT_TABLE_ADDR_MASK);
    // check whether L1 table entry is present
    if ((table1[l1_index] & PAGE_V) == 0) {
        // L1 table entry is not present, so allocate L2 table
        paddr_t l2_paddr = alloc_pages(1);
        table1[l1_index] = (l2_paddr | PD_TABLE) - KERNEL_BASE_ADDR;
    }

    /*
        Create L2 table entry
    */
    uint64_t *table2 = (uint64_t *)(table1[l1_index] & PAGE_NEXT_TABLE_ADDR_MASK);
    // check whether L2 table entry is present
    if ((table2[l2_index] & PAGE_V) == 0) {
        // L2 table entry is not present, so allocate L3 table
        paddr_t l3_paddr = alloc_pages(1);
        table2[l2_index] = (l3_paddr | PD_TABLE) - KERNEL_BASE_ADDR;
    }

    /*
        Create L3 table entry
    */
    uint64_t *table3 = (uint64_t *)(table2[l2_index] & PAGE_NEXT_TABLE_ADDR_MASK);
    table3[l3_index] = (paddr | flags | PAGE_ENTRY | PD_ACCESS) - KERNEL_BASE_ADDR;

    return;
}