#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>


#define MAX_USER_WIDTH 27
#define MAX_USER_SIZE 64
#define MAX_MESSAGE_LINE 3
#define MAX_MESSAGE_WIDTH 51

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
    char      name[MAX_USER_WIDTH];
};

struct message {
    struct user sender;
    time_t      timestamp;
    char        data[MAX_MESSAGE_LINE][MAX_MESSAGE_WIDTH];
};

struct screen_raw_input {
    struct user user;
    char        data;
};

struct screen_operation {
    void    (*init)  (struct screen* screen);
    void    (*loop)  (struct screen* screen, void* data);
    void    (*close) (struct screen* screen);
};

struct screen {
    WINDOW                  *window;
    struct window_size      size;
    std_input               input;
    struct screen_operation *s_op;
    char                    **buffer;
};

#endif