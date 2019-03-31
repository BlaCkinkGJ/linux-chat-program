#ifndef __MSG_RB_H__
#define __MSG_RB_H__

#include "screen.h"

#define MAX_RING_BUFFER_SIZE MAX_USER_SIZE
#define INVALID_MESSAGE_STRUCT 0

typedef int rb_index;

// ring buffer struct contains message struct data
struct msg_rb {
    struct message      buffer[MAX_RING_BUFFER_SIZE];
    rb_index            head;
    rb_index            tail;
    rb_index            count;    
    struct rb_operation *r_op;
};

struct rb_operation {
    void           (*init)     (struct msg_rb *rb);
    bool           (*is_empty) (struct msg_rb *rb);
    bool           (*is_full)  (struct msg_rb *rb);
    struct message (*get)      (struct msg_rb *rb);
    void           (*put)      (struct msg_rb *rb, struct message message);
    struct message (*peek)     (struct msg_rb *rb);
    void           (*flush)    (struct msg_rb *rb);
};

// constructor
struct msg_rb create_msg_rb();

// ring buffer index increment
static inline rb_index get_rb_inc_idx(const rb_index amount) {
    rb_index index = amount + 1;
    if (amount >= MAX_RING_BUFFER_SIZE)
        index = 0;
    return index;
}

void            msg_rb_init     (struct msg_rb *rb);
bool            msg_rb_is_empty (struct msg_rb *rb);
bool            msg_rb_is_full  (struct msg_rb *rb);
struct message  msg_rb_get      (struct msg_rb *rb);
void            msg_rb_put      (struct msg_rb *rb, struct message message);
struct message  msg_rb_peek     (struct msg_rb *rb);
void            msg_rb_flush    (struct msg_rb *rb);

#endif