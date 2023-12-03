#ifndef __ALLOC_H__
#define __ALLOC_H__

#include "common.h"
#include "print.h"

paddr_t alloc_pages(uint32_t n);

extern char __free_ram[], __free_ram_end[];
#endif // __ALLOC_H__