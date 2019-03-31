#include "msg_rb.h"

void msg_rb_init(struct msg_rb *rb){
    rb->head  = 0;
    rb->count = 0;
    rb->tail  = 0;
}

bool msg_rb_is_empty(struct msg_rb *rb){
    return (rb->count == 0);
}

bool msg_rb_is_full(struct msg_rb *rb){
    return (rb->count >= MAX_RING_BUFFER_SIZE);
}

struct message msg_rb_get(struct msg_rb *rb){
    struct message msg = {
        .timestamp = INVALID_MESSAGE_STRUCT
    };
    if (rb->count > 0) {
        msg = rb->buffer[rb->tail];
        rb->tail = get_rb_inc_idx(rb->tail);
        rb->count--;
    }
    return msg;
}

void msg_rb_put(struct msg_rb *rb, struct message message){
    if (rb->count >= MAX_RING_BUFFER_SIZE)
        return;
    rb->buffer[rb->head] = message;
    rb->head = get_rb_inc_idx(rb->head);
    rb->count++;
}

// only look the ring buffer contents 
struct message msg_rb_peek(struct msg_rb *rb){
    struct message invalid_msg = {
        .timestamp = INVALID_MESSAGE_STRUCT
    };
    return (rb->count ? invalid_msg : rb->buffer[rb->tail]);
}

void msg_rb_flush(struct msg_rb *rb){
    rb->count = 0;
    rb->head = 0;
    rb->tail = 0;
}

struct rb_operation msg_rb_op = {
    .init     = msg_rb_init,
    .is_empty = msg_rb_is_empty,
    .is_full  = msg_rb_is_full,
    .get      = msg_rb_get,
    .put      = msg_rb_put,
    .peek     = msg_rb_peek,
    .flush    = msg_rb_flush 
};

// constructor
struct msg_rb create_msg_rb(){
    struct msg_rb rb = {
        .count = 0,
        .head = 0,
        .tail = 0,
        .r_op = &msg_rb_op
    };
    return rb;
}