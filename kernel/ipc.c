#include "ipc.h"

static void send_message(struct process *dst, struct message *m, unsigned char flags) {

    /*
        | src process | dst process |
        |   current   |     dst     | 
    */
    struct process *current = current_proc; 

    /* cannot send message from myself */
    if (dst == current) {
        // Todo: return error
        return;
    }

    /*
        copy message from kernel (or user) space
        Copying from user space may be page fault
    */
    struct message copied_m;
    if (flags & IPC_KERNEL) {
        memcpy(&copied_m, (struct message *)m, sizeof(struct message));
    } else {
        // Todo: copy message from user space
        PANIC("not implemented yet");
    }
    
    /*
        send message algorithm
        if dst is not waiting for message
            put message into dst's message queue
            switch another process
            ------------------------------------
            (come bakc here)
            if dst process killed
                return error
            else 
                skip to else part below
        else
            put message into dst's message queue
            wake up dst
    */

    bool ready = (dst->state == PROC_UNUSED) 
                    && (dst->wait_for == IPC_ANY || dst->wait_for == current->pid);
    
    if (!ready) {
        /* Check wheather no-block message ?, if so, return error*/

        /*
            Check deadlock
            if both dst and src user try to send message to each other, deadlock occurs
            if so, return error
        */

       /*
            put message into dst's message queue and block this process and switch to another process
       */

        /*
            come back here (from other process switch this process)
            check dst process is killed or not, if so, return error
        */
    }

    /*
        set message to dst process message field
        make dst process runnable and switch to it
    */

    return;
}

static void recv_message(unsigned int src, struct message *m, unsigned char flags) {
    /*
        | src process | dst process |
        |     src     |   current   | 
    */
    struct process *current = current_proc;
    struct message copied_m;

    /*
        receive message algorithm
        set copied_m to following algorithm
        if get IPC notification
            set copied_m to
        else
            find src task from wait queue and restart it
            wait for message from src (switch to another process)
            ------------------------------------
            (come back here)
            set copied_m to current->m 
        
        copy copied_m to m (it is argument of this function)
    */

    return;
}