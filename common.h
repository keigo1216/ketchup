#pragma once

#include <stdarg.h>
// #define va_list  __builtin_va_list
// #define va_start __builtin_va_start
// #define va_end   __builtin_va_end
// #define va_arg   __builtin_va_arg

void putchar(char c);
void print_string(const char *s);
void printf(char *fmt, ...);