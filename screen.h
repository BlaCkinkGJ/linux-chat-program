#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>


#define MAX_MESSAGE_LINE 3
#define MAX_MESSAGE_WIDTH 51

#define MAX_OUTPUT_LINE 17 
#define MAX_OUTPUT_WIDTH 51

#define MAX_USER_LINE  22
#define MAX_USER_WIDTH 27

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
    std_input   data;
};

// For use overriding
struct screen_operation {
    void    (*init)  (struct screen* screen);
    void    (*loop)  (struct screen* screen, void* data);
    void    (*close) (struct screen* screen);
};

struct screen {
    WINDOW                  *window;
    struct window_size      size;
    struct screen_operation *s_op;
    char                    **buffer;
};

static inline void
clear_buffer(char **buffer, const int size){
    for(int i = 0; i < size; i++)
        buffer[i][0] = '\0';
}

static inline bool 
kbhit(void){
    int ch;
    bool ret;

    nodelay(stdscr, TRUE);

    ch = getch();
    if ( ch == ERR ) {
        ret = false;
    } else {
        ret = true;
        ungetch(ch);
    }

    nodelay(stdscr, FALSE);
    return ret;
}
#endif