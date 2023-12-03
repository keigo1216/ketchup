#pragma once

#include "common.h"

#define PANIC(fmt, ...) \
    do { \
        printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        while(1) {} \
    } while(0)

void putchar(char ch);
void printf(char *fmt, ...);
void print_string(const char *str);