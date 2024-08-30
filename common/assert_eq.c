#include "assert_eq.h"

void i_assert_eq(int a, int b, char *describe) {
    if (a == b) printf("[PASS] %s \n", describe);
    else {
        printf ("[Error] %s : Expected %d, but %d is Received\n", describe, b, a);
    }
}

void s_assert_eq(char *a, char *b, char *describe) {
    if (strcmp(a, b)) printf("[PASS] %s \n", describe);
    else {
        printf ("[ERROR] %s : Expected %s, but %s Received\n", describe, b , a);
    }
}

void assert_null(void *p, char *describe) {
    if (p == NULL) printf("[PASS] %s \n", describe);
    else printf("[ERROR] %s : Expected NULL, but %s Received\n", describe);
}