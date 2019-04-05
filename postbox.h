#ifndef __POSTBOX_H__
#define __POSTBOX_H__

#include "screen.h"
#include <stdbool.h>
#include <semaphore.h>
#include <string.h>

#define INVALID_USER_ID 0

#define COPY_MESSAGE    0
#define COPY_USER       1

// main structure for shared memory
struct postbox {
    uint64_t    user_counter;
    char        output [MAX_OUTPUT_LINE][MAX_OUTPUT_WIDTH];
    struct user user   [MAX_USER_LINE];
    bool        user_login[MAX_USER_LINE];
    sem_t       sem;
};

// set the flag and copy the post data
static inline void
set_flag_and_copy_post (int flag, bool *is_change, size_t max_size, 
        struct postbox *post, char **buffer) {
    int index = 0;
    for(index = 0; index < max_size; index++) {
        switch(flag) {
            case COPY_MESSAGE:
            if(strcmp(buffer[index], post->output[index]) != 0) {
                *is_change |= true;
                strcpy(buffer[index], post->output[index]);
            }
            break;
            case COPY_USER:
            if((post->user[index].id != INVALID_USER_ID) 
                && (strcmp(buffer[index], post->user[index].name) != 0)) {
                *is_change |= true;
                strcpy(buffer[index], post->user[index].name);
            } else if (post->user[index].id == INVALID_USER_ID) {
                strcpy(buffer[index], "\0");
            }
            break;
        }
    }
}

static inline void
copy_data (size_t max_size, struct postbox *post, char **buffer) {
    int index = 0;
    for(index = 0; index < max_size; index++) {
        strcpy(post->output[index], buffer[index]);
    }
}

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