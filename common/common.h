#pragma once

#define PL011_UART_BASE 0x3F201000
#define PL011_UART_DR   (PL011_UART_BASE + 0x00)
#define PL011_UART_FR   (PL011_UART_BASE + 0x18)

#define va_list  __builtin_va_list
#define va_start __builtin_va_start
#define va_end   __builtin_va_end
#define va_arg   __builtin_va_arg

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef uint64_t size_t;

void *memset(void *buf, char c, size_t n);