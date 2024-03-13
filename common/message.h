#include "common.h"

typedef struct {
    int type; /*type of message*/
    int src;  /*source of process*/
    union {
        uint8_t data[0]; /*head pointer of message data*/
    };
} message;