ENTRY(boot)

LMA_OFFSET = 0xffff000000000000;

MEMORY {
    boot_ram (rwx): ORIGIN = 0x0000000000080000, LENGTH = 0x0001000
    ram (rwx):      ORIGIN = 0xffff000000081000, LENGTH = 0x2fff000
    lma_ram (rwx):  ORIGIN = 0x0000000000081000, LENGTH = 0x2fff000
}

SECTIONS {
   .boot :{
        KEEP(*(.text.boot));
    } > boot_ram

    .text : ALIGN(8) {
        *(.text .text.*);
    } > ram AT > lma_ram

    .rodata : ALIGN(8) {
        *(.rodata .rodata.*);
    } > ram AT > lma_ram

    .data : ALIGN(8) {
        *(.data .data.*);
    } > ram AT > lma_ram

    .bss : ALIGN(8) {
        __bss = .;
        *(.bss .bss.* .sbss .sbss.*);
        __bss_end = .;

        . = ALIGN(8);
        . += 4 * 1024; /* 4KB */
        __stack_top = .;

        . = ALIGN(4096);
        __kernel_page = . - LMA_OFFSET;
        . += 4 * 1024 + 4 * 1024;

        . = ALIGN(4096);
        __free_ram = .;
        . += 64 * 1024;
        __free_ram_end = .;

        . = ALIGN(8);
        end = .;  /* カーネルイメージの終わりを示すシンボル */
    } > ram AT > lma_ram
}
