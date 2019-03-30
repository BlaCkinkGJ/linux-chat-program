#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <ncurses.h>
#include <unistd.h>
#include "../util/linked_list.h"
#include <time.h>
#include <ncurses.h>


#define MAX_CHAR_SIZE 256
#define MAX_USER_SIZE 64

typedef int std_input;


struct point {
    uint64_t x;
    uint64_t y;
};

struct window_size {
    uint64_t height;
    uint64_t width;
    struct point start;
};

struct user {
    char      name[MAX_CHAR_SIZE];
};

struct message {
    struct user sender;
    time_t      timestamp;
    char        data[MAX_CHAR_SIZE];
};

struct screen_operation {
    void    (*init)  (struct screen* screen);
    void    (*loop)  (struct screen* screen);
    void    (*close) (struct screen* screen);
    void    (*add)   (struct screen* screen, void *data);
    void    (*del)   (struct screen* screen);
};

struct screen {
    WINDOW                  *window;
    struct window_size      size;
    std_input               input;
    struct linked_list      list; // this contains the data
    struct screen_operation *s_op;
};

#endif