.balign 2048
.global exception_vector
exception_vector:
    b exception_common
    .balign 128
    b interrupt_common
    .balign 128
    b interrupt_common
    .balign 128
    b exception_common
    .balign 128

    b exception_common
    .balign 128
    b interrupt_common
    .balign 128
    b interrupt_common
    .balign 128
    b exception_common
    .balign 128

    b exception_common
    .balign 128
    b interrupt_common
    .balign 128
    b interrupt_common
    .balign 128
    b exception_common
    .balign 128

    b exception_common
    .balign 128
    b interrupt_common
    .balign 128
    b interrupt_common
    .balign 128
    b exception_common
    .balign 128

exception_common:
    stp  x1,  x2,  [sp, #-16]!
    stp  x3,  x4,  [sp, #-16]!
    stp  x5,  x6,  [sp, #-16]!
    stp  x7,  x8,  [sp, #-16]!
    stp  x9,  x10, [sp, #-16]!
    stp  x11, x12, [sp, #-16]!
    stp  x13, x14, [sp, #-16]!
    stp  x15, x16, [sp, #-16]!
    stp  x17, x18, [sp, #-16]!
    stp  x19, x20, [sp, #-16]!
    stp  x21, x22, [sp, #-16]!
    stp  x23, x24, [sp, #-16]!
    stp  x25, x26, [sp, #-16]!
    stp  x27, x28, [sp, #-16]!
    stp  x29, x30, [sp, #-16]!

    b handle_trap
    
    ldp  x29, x30, [sp], #16
    ldp  x27, x28, [sp], #16
    ldp  x25, x26, [sp], #16
    ldp  x23, x24, [sp], #16
    ldp  x21, x22, [sp], #16
    ldp  x19, x20, [sp], #16
    ldp  x17, x18, [sp], #16
    ldp  x15, x16, [sp], #16
    ldp  x13, x14, [sp], #16
    ldp  x11, x12, [sp], #16
    ldp  x9,  x10, [sp], #16
    ldp  x7,  x8,  [sp], #16
    ldp  x5,  x6,  [sp], #16
    ldp  x3,  x4,  [sp], #16
    ldp  x1,  x2,  [sp], #16
    eret

interrupt_common:  
    eret