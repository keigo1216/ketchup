#include "armv8def.h"

#define BOOT_PGD_ATTR PD_TABLE
#define BOOT_PUD_ATTR (PD_ACCESS | (MAIR_EL1_IDX_DEVICE_nGnRnE << 2) | PD_BLOCK)

.section ".text.boot"
.global boot
boot:
    ldr  x0, =0x30d00800 // Lttile Endian, MMU disabled, I/D-cache disabled
    msr  sctlr_el1, x0

    // Non-Secure
    // set EL2 to AArch64
    mrs     x0, SCR_EL3
    mov     x1,  SCR_EL3_RW | SCR_EL3_NS
    orr     x0, x0, x1
    msr     SCR_EL3, x0

    // set EL1 to AArch64
    mov     x2, #HCR_EL2_RW
    msr     HCR_EL2, x2

    // set stack pointer
    ldr     x4, =__stack_top
    mov     sp, x4

    mrs  x1, mpidr_el1
    and  x1, x1, #3
    cbz x1, EL3_TO_EL1
    bl halt

halt:
    bl halt

EL3_TO_EL1:
    // ELR_EL3
    ldr     x0, =el1_mode
    msr    ELR_EL3, x0            // set up the return data
    // SPSR_EL3
    mov     x0, PSR_A_BIT | PSR_I_BIT | PSR_F_BIT | PSR_MODE_EL1h
    msr     SPSR_EL3, x0

    mov     x24, sp
    msr     SP_EL1, x24
    eret

el1_mode:
    ldr x10, =__stack_top
    mov sp, x10
    
    ldr   x0, =__stack_top
    msr   SP_EL0, x0

    mrs   x0, CPACR_EL1         // CPACR_EL1レジスタの現在の値を読み込む
    orr   x0, x0, #(0b11 << 20) // CPACR_EL1の浮動小数点とSIMDのアクセスを有効にする
    msr   CPACR_EL1, x0         // CPACR_EL1レジスタに値を書き込む

    ldr x0, = TCR_ELL1_REGION_48bit | TCR_EL1_PAGESIZE_4KB
    msr tcr_el1, x0

    ldr x0, =( \
        (MAIR_EL1_DEVICE_nGnRnE << (MAIR_EL1_IDX_DEVICE_nGnRnE * 8)) | \
        (MAIR_EL1_NORMAL_NOCACHE << (MAIR__EL1_IDX_NORMAL_NOCACHE * 8)) \
        )
    msr mair_el1, x0


    ldr x0, =__kernel_page // PGD's page frame at 0x0
    mov x1, 0x1000 // PUD's page frame at 0x1000
    add x1, x1, x0

    ldr x2, = BOOT_PGD_ATTR
    orr x2, x1, x2 // combine the physical address of next level page with attribute.
    str x2, [x0]

    ldr x2, = BOOT_PUD_ATTR
    mov x3, 0x00000000
    orr x3, x2, x3
    str x3, [x1] // 1st 1GB mapped by the 1st entry of PUD
    mov x3, 0x40000000
    orr x3, x2, x3
    str x3, [x1, 8] // 2nd 1GB mapped by the 2nd entry of PUD

    msr ttbr0_el1, x0
    msr ttbr1_el1, x0

    mrs x2, sctlr_el1
    orr x2 , x2, 1
    msr sctlr_el1, x2 // enable MMU, cache remains disabled

    bl kernel_main