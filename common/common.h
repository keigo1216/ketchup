#pragma once

typedef int bool;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef uint64_t size_t;
typedef uint64_t paddr_t;
typedef uint64_t vaddr_t;
typedef uint64_t usize64_t;

#define true 1
#define false 0
#define NULL ((void *)0)
#define align_up(value, align)   __builtin_align_up(value, align)
#define is_aligned(value, align) __builtin_is_aligned(value, align)
#define offsetof(type, member)   __builtin_offsetof(type, member)

#define KERNEL_BASE_ADDR 0xffff000000000000

#define PL011_UART_BASE 0x3F201000 + KERNEL_BASE_ADDR
#define PL011_UART_DR   (PL011_UART_BASE + 0x00)
#define PL011_UART_FR   (PL011_UART_BASE + 0x18)

#define va_list  __builtin_va_list
#define va_start __builtin_va_start
#define va_end   __builtin_va_end
#define va_arg   __builtin_va_arg

#define PAGE_SIZE 4096

#define PROCS_MAX 8
#define PROC_UNUSED 0
#define PROC_RUNNABLE 1

#define SYS_PUTCHAR 1

void *memset(void *buf, char c, size_t n);
void *memcpy(void *dst, const void *src, size_t n);
char *strcpy(char *dst, const char *src);
int strcmp(const char *s1, const char *s2);
void printf(char *fmt, ...);