#include "print.h"

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