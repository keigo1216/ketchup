#pragma once

#include "asm.h"
#include "print.h"

void interrupt_trap(void);
void handle_trap(void);
void set_timer();