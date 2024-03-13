#include "message.h"
#include "process.h"
#include "common.h"

/*
    send message `m` to process `dst` with flags `flags`
*/
static void send_message(struct process *dst, struct message *m, unsigned char flags);

/*
    receive message `m` from process `src` with flags `flags`
*/
static void recv_message(unsigned int src, struct message *m, unsigned char flags);