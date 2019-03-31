#include "user.h"
#include <stdlib.h>
#include <string.h>

#define DEFAULT 0
#define window_border_clear(win) wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ')

static uint64_t max_user_line;

struct screen_operation user_s_op = {
    .init  = user_init,
    .loop  = user_loop,
    .close = user_close,
};

// constructor of user screen
struct screen user_screen (struct window_size size) {
    struct screen user_screen;
    user_screen.s_op = &user_s_op;

    user_screen.size.height  = size.height;
    user_screen.size.width   = size.width;
    user_screen.size.start.x = size.start.x;
    user_screen.size.start.y = size.start.y;

    return user_screen;
}

void user_init(struct screen* screen) {
    int height     = screen->size.height;
    int width      = screen->size.width;
    int index      = 0;

    struct point start = {
        .x = screen->size.start.x,
        .y = screen->size.start.y
    };

    max_user_line = height - 2; // because of the box's occupy line

    screen->buffer = (char**)malloc(sizeof(char*)*(max_user_line));
    for(index = 0; index < max_user_line; index++){
        screen->buffer[index]    = (char *)malloc(sizeof(char) * (width - 1));
        screen->buffer[index][0] = '\0';
    }

    screen->window = newwin(height, width, start.y, start.x);
    box(screen->window, DEFAULT, DEFAULT);
    wrefresh(screen->window);
}

// get message line which not empty
static inline uint64_t
check_msg_line(const char **data){
    uint64_t line_size;
    for (line_size = 0; line_size < MAX_MESSAGE_LINE; line_size++) {
        if ( data[line_size][0] == '\0') break;
    }
    return line_size + 1;
}

/*
 * Just add the user in buffer's last position.
 * If you use the share memory then this function may be useful.
 */
static inline void
push_the_user(char **screen_buffer, struct user* usr, uint64_t num_of_line) {
    strcpy(screen_buffer[num_of_line - 1], usr->name);
}

static inline void
print_the_screen(struct screen* screen) {
    int index = 0;
    for(; index < max_user_line; index++) {
        // sequence ==> y, x, data
        mvwprintw(screen->window, index + 1, 1, "%s", screen->buffer[index]);
    }
}

void user_loop(struct screen* screen, void* data) {
    struct user *usr = (struct user *)data;
    uint64_t usr_line = check_msg_line((const char **)(screen->buffer));
    wclear(screen->window);
    push_the_user(screen->buffer, usr, usr_line);
    print_the_screen(screen);

    box(screen->window, 0, 0);
    mvwprintw(screen->window, 0, 1, "login-user");
    wrefresh(screen->window);
}


void user_close(struct screen* screen) {
    uint64_t index = 0;
    window_border_clear(screen->window);
    wrefresh(screen->window);
    delwin(screen->window);
    for(index = 0; index < max_user_line; index++)
        free(screen->buffer[index]);
    free(screen->buffer);
}