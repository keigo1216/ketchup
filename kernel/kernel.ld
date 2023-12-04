ENTRY(boot)

SECTIONS {
    . = 0x80000;  /* カーネルのロードアドレス */

   .text :{
        KEEP(*(.text.boot));
        *(.text .text.*);
    }

    .rodata : ALIGN(16) {
        *(.rodata .rodata.*);
    }

    .data : ALIGN(16) {
        *(.data .data.*);
    }

    .bss : ALIGN(16) {
        __bss = .;
        *(.bss .bss.* .sbss .sbss.*);
        __bss_end = .;
    }

    . = ALIGN(16);
    . += 128 * 1024; /* 128KB */
    __stack_top = .;

    . = ALIGN(16);
    . += 128 * 1024; /* 128KB */
    __stack_top_user = .;

    . = ALIGN(4096);
    __free_ram = .;
    . += 64 * 1024 * 1024;
    __free_ram_end = .;

    . = ALIGN(16);
    end = .;  /* カーネルイメージの終わりを示すシンボル */
}
