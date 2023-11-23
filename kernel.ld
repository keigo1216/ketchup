ENTRY(boot)

SECTIONS {
    . = 0x80000;  /* カーネルのロードアドレス */

   .text :{
        KEEP(*(.text.boot));
        *(.text .text.*);
    }

    .rodata : ALIGN(8) {
        *(.rodata .rodata.*);
    }

    .data : ALIGN(8) {
        *(.data .data.*);
    }

    .bss : ALIGN(8) {
        __bss = .;
        *(.bss .bss.* .sbss .sbss.*);
        __bss_end = .;
    }

    . = ALIGN(8);
    . += 1024 * 1024; /* 128KB */
    __stack_top = .;

    . = ALIGN(8);
    end = .;  /* カーネルイメージの終わりを示すシンボル */
}
