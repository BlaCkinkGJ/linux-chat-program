#ifndef __POSTBOX_H__
#define __POSTBOX_H__

#include "msg_rb.h"
#include "screen.h"
#include <stdbool.h>

struct postbox {
    struct msg_rb msg_box[MAX_USER_SIZE];
    bool          is_user_box_used[MAX_USER_SIZE]; // index means user id
    struct user   user_box[MAX_USER_SIZE];
};

#endif