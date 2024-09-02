#pragma once
#include "common.h"

struct message{
    int type; /*type of message*/
    int src;  /*source of process*/
    union {
        uint8_t data[0]; /*head pointer of message data*/
    };
};