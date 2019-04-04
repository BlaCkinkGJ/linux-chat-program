#ifndef __POSTBOX_H__
#define __POSTBOX_H__

#include "screen.h"
#include <stdbool.h>
#include <string.h>

#define INVALID_USER_ID 0
// set the flag and copy the post data
// usage:  set_flag_and_copy_post(is_change, MAX_OUTPUT_LINE, output, post->output);
#define set_flag_and_copy_post_msg(is_change, max_size, dst, src) do{\
    int index = 0;\
    for(index = 0; index < max_size; index++) {\
        if(strcmp(dst[index], src[index]) != 0) {\
            is_change |= true;\
            strcpy(dst[index], src[index]);\
        }\
    }\
}while(0)

#define set_flag_and_copy_post_usr(is_change, max_size, dst, src) do{\
    int index = 0;\
    for(index = 0; index < max_size; index++) {\
        if(((src)[index].id != INVALID_USER_ID) && (strcmp(dst[index], (src)[index].name) != 0)) {\
            is_change |= true;\
            strcpy(dst[index], (src)[index].name);\
        } else if((src)[index].id == INVALID_USER_ID) {\
            strcpy(dst[index], "\0");\
        }\
    }\
}while(0)

#define copy_msg_data(max_size, dst, src) do{\
    int index = 0;\
    for(index = 0; index < max_size; index++)\
        strcpy(dst[index], src[index]);\
}while(0)

// this for share memory
struct postbox {
    uint64_t    user_counter;
    char        output [MAX_OUTPUT_LINE][MAX_OUTPUT_WIDTH];
    struct user user   [MAX_USER_LINE];
    bool        user_login[MAX_USER_LINE];
};

static inline void arrange_user(struct postbox* post) {
    int index, bubble;
    for(index = 0; index <= MAX_USER_LINE; index++) {
        for(bubble = 1; bubble < MAX_USER_LINE - index; bubble++) {
            if(post->user[bubble].id != INVALID_USER_ID && post->user[bubble - 1].id == INVALID_USER_ID) {
                memcpy(&(post->user[bubble - 1]), &(post->user[bubble]), sizeof(struct user));
                post->user[bubble].id = INVALID_USER_ID;
            } // end of if
        } // end of for
    } // end of for
}

static uint64_t current_user_id = 0;

static inline uint64_t get_user_id(struct postbox* post) {
    uint64_t index = 0;
    while(post->user_login[index] != false)  {
        index += 1;
        if(index == MAX_USER_LINE) break;
    }
    return index + 1; // because of the INVALID is 0
}

static inline int 
set_user_id(struct postbox* post, char *name) {
    uint64_t user_id = INVALID_USER_ID;
    uint64_t index = 0;
    user_id = get_user_id(post);
    for(index = 0; index < MAX_USER_LINE; index++) {
        if(post->user[index].id == INVALID_USER_ID) {
            post->user[index].id = user_id;
            strcpy(post->user[index].name, name);
            current_user_id = user_id;
            break;
        }
    }

    if (index == MAX_USER_LINE)
        return -1;

    post->user_login[current_user_id - 1] = true;
    return 0;
}

static inline int
remove_user_id(struct postbox* post) {
    int index = 0;
    for(index = 0; index < MAX_USER_LINE; index++) {
        if (post->user[index].id == current_user_id) {
            post->user[index].id = INVALID_USER_ID;
            post->user_login[current_user_id - 1] = false;
            break;
        }
    }
    if (index == MAX_USER_LINE)
        return -1;
    
    arrange_user(post);
    return 0;
}


#endif