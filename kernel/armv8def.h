#ifndef __ARMV8DEF_H__
#define __ARMV8DEF_H__

/*
 * PSR bits
 */
#define PSR_MODE_EL0t 0x00000000
#define PSR_MODE_EL1t 0x00000004
#define PSR_MODE_EL1h 0x00000005
#define PSR_MODE_EL2t 0x00000008
#define PSR_MODE_EL2h 0x00000009
#define PSR_MODE_EL3t 0x0000000c
#define PSR_MODE_EL3h 0x0000000d
#define PSR_MODE_MASK 0x0000000f

/* AArch64 SPSR bits */
#define PSR_F_BIT 0x00000040
#define PSR_I_BIT 0x00000080
#define PSR_A_BIT 0x00000100

/* SCR_EL3 bits */
#define SCR_EL3_RW 0x00000400
#define SCR_EL3_NS 0x00000001

/* HCR_EL2 bits */
#define HCR_EL2_RW 0x80000000

/* TCR_EL1 bits */
#define TCR_ELL1_REGION_48bit (((64 - 48) << 0) | ((64 - 48) << 16))
#define TCR_EL1_PAGESIZE_4KB ((0b00 << 14) |  (0b10 << 30))

/* MAIR_EL1 bits */
#define MAIR_EL1_DEVICE_nGnRnE 0x00
#define MAIR_EL1_NORMAL_NOCACHE 0b01000100
#define MAIR_EL1_IDX_DEVICE_nGnRnE 0
#define MAIR__EL1_IDX_NORMAL_NOCACHE 1

/* PD bits */
#define PD_TABLE 0b11
#define PD_BLOCK 0b01
#define PD_ACCESS (1 << 10) /* for page fault, not control access (user or kernel) */

/* PAGE Entry bits */
#define PAGE_V (1 << 0)
#define PAGE_ENTRY (0b11 << 0)
#define PAGE_R (1 << 7)
#define PAGE_RW (0 << 7)
#define PAGE_ACCESS (1 << 6)
#define PAGE_NOACCESS (0 << 6)
#define PAGE_NEXT_TABLE_ADDR_MASK 0x0000fffffffff000

/* GIC bits */
#define GCI_CTRL_BASE 0x40000000
#define CORE0_TIMER_INTTERUPT (GCI_CTRL_BASE + 0x40)
#define CORE0_IRQ_SOURCE (GCI_CTRL_BASE + 0x60)

/* Time Quantum */
#define INIT_TIME_QUANTUM 2 // (timer interrupt cycle = 100ms, so 200ms = 2 * 100ms)

#endif // __ARMV8DEF_H__