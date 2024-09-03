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

process_t sys_process_create(uint64_t kernel_entry) {
    return process_create(kernel_entry);
}

void sys_vm_map(process_t pid, uint64_t vaddr, uint64_t paddr, uint64_t flags) {
    // Get process structure using pid
    struct process *proc = process_find(pid);

    // TODO : Check paddr is available

    vm_map(proc, vaddr, paddr, flags);
}

uint64_t handle_syscall(uint64_t sysno, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6) {
    switch (sysno) {
        case SYS_PUTCHAR:
            putchar(arg1);
            return 0; // TODO : return success code
        case SYS_MAP_PAGE:
            sys_vm_map((process_t)arg1, arg2, arg3, arg4);
            return 0; // TODO : return success code
        case SYS_CREATE_PROCESS:
            return sys_process_create(arg1);
        default:
            PANIC("unknown syscall: sysno=%d\n", sysno);
            return 0; // TODO : return success code
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
        init_process_struct(&idle_proc, -1, NULL);
        current_proc = &idle_proc;

        // paddr_t paddr0 = alloc_pages(2);
        // paddr_t paddr1 = alloc_pages(1);
        // printf("alloc_pages test: paddr0=%x\n", paddr0);
        // printf("alloc_pages test: paddr1=%x\n", paddr1);

        // proc_a = create_process((uint64_t) proc_a_entry);
        // proc_b = create_process((uint64_t) proc_b_entry);
        
        // test_list_macro();


        process_t id_a = process_create(USER_BASE);        
        process_t id_b =  process_create(USER_BASE);
        struct process *proc_a = process_find(id_a);
        struct process *proc_b = process_find(id_b);
        // map page
        for (usize64_t off = 0; off < (size_t)(_binary_shell_bin_end - _binary_shell_bin_start); off += PAGE_SIZE) {
            paddr_t page = alloc_pages(1);
            // printf("page = %x\n", page);
            memcpy((void *)page, _binary_shell_bin_start + off, PAGE_SIZE);
            vm_map(proc_a, USER_BASE + off, page, PAGE_RW | PAGE_ACCESS);
        }
        for (usize64_t off = 0; off < (size_t)(_binary_shell_bin_end - _binary_shell_bin_start); off += PAGE_SIZE) {
            paddr_t page = alloc_pages(1);
            // printf("page = %x\n", page);
            memcpy((void *)page, _binary_shell_bin_start + off, PAGE_SIZE);
            vm_map(proc_b, USER_BASE + off, page, PAGE_RW | PAGE_ACCESS);
        }

        yeild();

        // PANIC("booted!");
        // PANIC("unreachable");
        idel_process();
    } else {
        while(1) {}
    }
    while (1) {}
}