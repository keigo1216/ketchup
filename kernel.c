typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef uint64_t size_t;

extern char __bss[], __bss_end[], __stack_top[];

void print_string(const char *str) {
    while (*str) {
        *(volatile char *)0x09000000 = *str++;  // 仮想UARTアドレス
    }
}

void *memset(void *buf, char c, size_t n) {
    uint8_t *p = (uint8_t *)buf;
    while(n--) {
        *p++ = c;
    }
    return buf;
}

void kernel_main() {
    memset(__bss, 0, (size_t)__bss_end - (size_t)__bss);
    print_string("Hello Kernel\n");
    while (1) {}
}

__attribute__((section(".text.boot")))
__attribute__((naked))
void boot(void) {
    __asm__ __volatile__(
        "ldr X8, =__stack_top\n"
        "mov sp, X8\n"
        "bl kernel_main\n"
        :
        :
        : "r0"
    );
}