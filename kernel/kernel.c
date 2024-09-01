#include "kernel.h"
#include "timer.h"

extern char __bss[], __bss_end[], __free_ram[], __free_ram_end[];

struct process *proc_a;
struct process *proc_b;

void proc_a_entry(void) {
    printf("starting proc_a\n");
    while (1) {
        putchar('A');
        yeild();

        for (int i = 0; i < 1000000; i++) {
            __asm__ __volatile__("nop");
        }
    }
}

void proc_b_entry(void) {
    printf("starting proc_b\n");
    while (1) {
        putchar('B');
        yeild();

        for (int i = 0; i < 1000000; i++) {
            __asm__ __volatile__("nop");
        }
    }
}


void handle_syscall(int sysno, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6) {
    switch (sysno) {
        case SYS_PUTCHAR:
            putchar(arg1);
            break;
        default:
            PANIC("unknown syscall: sysno=%d\n", sysno);
            break;
    }
}

__noreturn static void idel_process(void) {
    for (;;) {
        yeild(); // switch to other task
        
        // TODO : unlock kernel 

        // TODO : enable interrupt
        // TODO : nop?

        // TODO : lock kernel
    }
}

void kernel_main() {
    unsigned int core_id = get_core_id();
    if (core_id == 0) {
        init_timer_handler();

        // while(1) {
        //     __asm__ __volatile__ ("wfi");
        // }

        // clear bss
        printf("current el = %d\n", get_current_el());
        memset(__bss, 0, __bss_end - __bss);

        // Test accsess to free ram using virtual address
        // printf("__kernel_page: %x\n", ((uint64_t *)(0xffff0000000a4000))[0]);
        // printf("__kernel_page: %x\n", ((uint64_t *)(0x00000000000a4000))[0]);

        // // アイドルプロセスの生成
        // idle_proc = create_process(NULL, 0);
        // idle_proc->pid = -1;
        // current_proc = idle_proc;

        // initialize idel process
        init_process_struct(&idle_proc, -1, NULL, 0);
        current_proc = &idle_proc;

        // paddr_t paddr0 = alloc_pages(2);
        // paddr_t paddr1 = alloc_pages(1);
        // printf("alloc_pages test: paddr0=%x\n", paddr0);
        // printf("alloc_pages test: paddr1=%x\n", paddr1);

        // proc_a = create_process((uint64_t) proc_a_entry);
        // proc_b = create_process((uint64_t) proc_b_entry);
        
        // test_list_macro();

        process_create(_binary_shell_bin_start, (size_t)(_binary_shell_bin_end - _binary_shell_bin_start));        
        process_create(_binary_shell_bin_start, (size_t)(_binary_shell_bin_end - _binary_shell_bin_start));
        yeild();

        // PANIC("booted!");
        // PANIC("unreachable");
        idel_process();
    } else {
        while(1) {}
    }
    while (1) {}
}