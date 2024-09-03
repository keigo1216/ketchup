#include "ipc.h"

/*
    flags format (every fields has a 1 bit)
    | IPC_NOBLOCK | IPC_FROM | IPC_RECV | IPC_SEND |
    IPC_NOBLOCK[3] : 
    IPC_FROM[2]    : Message memmory from Kernel space => 0, from User space => 1 
    IPC_RECV[1]    : 
    IPC_SEND[0]    :
*/
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

    bool ready = (dst->state == PROC_BLOCKED) 
                    && (dst->wait_for == IPC_ANY || dst->wait_for == current->pid);
    
    if (!ready) {
        /* Check wheather no-block message ?, if so, return error*/
        if (flags & IPC_NOBLOCK) {
            // TODO : return error
            PANIC("NO BLOCK");
        }

        /*
            Check deadlock
            if both dst and src user try to send message to each other, deadlock occurs
            if so, return error
        */
       LIST_FOR_EACH(proc, &current->senders, struct process, waitqueue_next) {
            if (proc->pid == dst->pid) {
                // TODO : WARN and return error
                PANIC("DEAD LOCK");
            }
       }

       /*
            put message into dst's message queue and block this process and switch to another process
       */
        list_push_back(&dst->senders, &current->waitqueue_next);
        process_block(current);

        /*
            come back here (from other process switch this process)
            check dst process is killed or not, if so, return error
        */
        yeild();

        /*
            TODO : if destination process are terminate, return error
        */
    }

    /*
        set message to dst process message field
        make dst process runnable and switch to it
    */
    memcpy(&dst->m, &copied_m, sizeof(struct message));
    dst->m.src = (flags & IPC_KERNEL) ? FROM_KERNEL : current->pid;
    process_resume(dst);

    return;
}

static void recv_message(process_t src, struct message *m, unsigned char flags) {
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

    // TODO : implement notification

    // NOT notification
    // #######################
    LIST_FOR_EACH(sender, &current->senders, struct process, waitqueue_next) {
        if (src == IPC_ANY || src == sender->pid) {
            // TODO : print for debug
            list_remove(&sender->waitqueue_next);
            process_resume(sender);
            src = sender->pid;  // For IPC_ANY
            break;
        }
    }

    // switch to src process
    current->wait_for = src;
    process_block(current);
    yeild();

    // receive message
    current->wait_for = IPC_DENY;
    memcpy(&copied_m, &current->m, sizeof(struct message));
    // #######################

    if (flags & IPC_KERNEL) {
        memcpy((void *)m, &copied_m, sizeof(struct message));
    } else {
        PANIC("not implemented yet");
    }


    return;
}

void ipc(struct process *dst, process_t src, struct message *m, unsigned int flags) {
    
    if (flags & IPC_SEND) {
        send_message(dst, m, flags);
    }

    if (flags & IPC_RECV) {
        recv_message(src, m, flags);
    }

}