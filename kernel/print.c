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


void printf(char *fmt, ...) {
    va_list vargs;
    va_start(vargs, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case '\0':
                    putchar('%');
                    goto end;
                case '%':
                    putchar('%');
                    break;
                case 's': {
                    const char *s = va_arg(vargs, const char *);
                    while (*s) {
                        putchar(*s);
                        s++;
                    }
                    break;
                }
                case 'd': {
                    int value = va_arg(vargs, int);
                    if (value < 0) {
                        putchar('-');
                        value = -value;
                    }

                    int divisor = 1;
                    while (value / divisor > 9) 
                        divisor *= 10;
                    
                    while (divisor > 0) {
                        putchar('0' + value / divisor);
                        value %= divisor;
                        divisor /= 10;
                    }
                    break;
                }
                case 'x': {
                    int value = va_arg(vargs, int);
                    for (int i = 7; i >=0; i--) {
                        int nibble = (value >> (i * 4)) & 0xf;
                        putchar("0123456789abcdef"[nibble]);
                    }
                    break;
                }
            }
        } else {
            putchar(*fmt);
        }
        fmt++;
    }

end:
    va_end(vargs);
}