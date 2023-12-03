#include "armv8def.h"
.section ".text.boot"
.global boot
boot:
    // Set exception_vector
    ldr   x5, =exception_vector
    msr   VBAR_EL3, x5
    msr   VBAR_EL2, x5
    msr   VBAR_EL1, x5

    // Set AArch64 on EL2
    mrs   x0, SCR_EL3
    mov   x1,  SCR_EL3_RW | SCR_EL3_NS
    orr   x0, x0, x1
    msr   SCR_EL3, x0

    // Set PC of start_el2
    ldr   x0, =kernel_main
    msr   elr_el3, x0

    mov   x0, PSR_A_BIT | PSR_I_BIT | PSR_F_BIT | PSR_MODE_EL2h
    msr   spsr_el3, x0

    ldr   x0, =__stack_top
    msr   SP_EL2, x0

    eret

start_el2:
    // Set AArch64 on EL1
    mov   x2, #HCR_EL2_RW
    msr   HCR_EL2, x2

    ldr   x0, =start_el1
    msr   ELR_EL2, x0

    mov   x0, PSR_A_BIT | PSR_I_BIT | PSR_F_BIT | PSR_MODE_EL1h
    msr   SPSR_EL2, x0

    ldr   x0, =__stack_top
    msr   SP_EL1, x0

    eret

start_el1:
    ldr x10, =__stack_top
    mov sp, x10

    mrs x0, daif       // 現在の割り込みマスクを取得
    bic x0, x0, #0xC0  // IビットとFビットをクリアして、IRQとFIQ割り込みを有効化
    msr daif, x0

    mrs   x0, CPACR_EL1         // CPACR_EL1レジスタの現在の値を読み込む
    orr   x0, x0, #(0b11 << 20) // CPACR_EL1の浮動小数点とSIMDのアクセスを有効にする
    msr   CPACR_EL1, x0         // CPACR_EL1レジスタに値を書き込む
    bl kernel_main