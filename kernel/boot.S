#include "armv8def.h"

.section ".text.boot"
.global boot
boot:
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

    bl kernel_main