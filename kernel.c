#include "common.h"

#define PL011_UART_BASE 0x3F201000
#define PL011_UART_DR   (PL011_UART_BASE + 0x00)
#define PL011_UART_FR   (PL011_UART_BASE + 0x18)

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef uint64_t size_t;

extern char __bss[], __bss_end[], __stack_top[];

static inline uint64_t get_core_id(void) {
    uint64_t core_id;
    __asm__ __volatile__ (
        "mrs %0, mpidr_el1"
        : "=r" (core_id)
    );
    return core_id & 0x3;
}

void putchar(char ch) {
    // *(volatile char *)0x3F201000 = ch;  // 仮想UARTアドレス
    // UARTのレジスタのアドレス
    __volatile__ unsigned int *uart_flags = (unsigned int *)(PL011_UART_FR);
    __volatile__ unsigned int *uart_data = (unsigned int *)(PL011_UART_DR);

    // 送信可能かどうかをチェック
    while (*uart_flags & (1 << 5)) {}

    // 送信
    *uart_data = ch;
}

void print_string(const char *str) {
    while (*str) {
        putchar(*str++);
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
    unsigned int core_id = get_core_id();
    if (core_id == 0) {
        printf("Hello, world!%d\n", 123);
    } else {
        while(1) {}
    }
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