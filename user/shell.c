#include "user.h"

void proc_a(void) {
    printf("Hello proc_a\n");
    while(1) {

    }
}

void proc_b(void) {
    printf("Hello World\n");
    while(1) {

    }
}

void main(void) {
    // printf("Hello world\n");
    process_create((uint64_t)&proc_b);
    // process_create();
    // printf("Hello World");x
    while(1) {

    }
}