#include "output.h"
#include <stdlib.h>
#include <string.h>

#define DEFAULT 0
#define window_border_clear(win) wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ')
#define OUTPUT_BOX_NAME "output"

static uint64_t max_output_line;

struct screen_operation output_s_op = {
    .init  = output_init,
    .loop  = output_loop,
    .close = output_close,
};

// constructor of output screen
struct screen output_screen (struct window_size size) {
    struct screen output_screen;
    output_screen.s_op = &output_s_op;

    output_screen.size.height  = size.height;
    output_screen.size.width   = size.width;
    output_screen.size.start.x = size.start.x;
    output_screen.size.start.y = size.start.y;

    return output_screen;
}

void output_init(struct screen* screen) {
    int height     = screen->size.height;
    int width      = screen->size.width;
    int index      = 0;

    struct point start = {
        .x = screen->size.start.x,
        .y = screen->size.start.y
    };

    max_output_line = height - 2; // because of the box's occupy line

    screen->buffer = (char**)malloc(sizeof(char*)*(max_output_line));
    for(index = 0; index < max_output_line; index++){
        screen->buffer[index]    = (char *)malloc(sizeof(char) * (width - 1));
        screen->buffer[index][0] = '\0';
    }

    screen->window = newwin(height, width, start.y, start.x);
    box(screen->window, DEFAULT, DEFAULT);
    mvwprintw(screen->window, 0, 1, OUTPUT_BOX_NAME);
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

// move the message to top of screen
static inline void
__make_empty_lines__(char **screen_buffer) {
    uint64_t index = 1;
    for (;index < max_output_line; index++){
        char *prev = screen_buffer[index - 1];
        char *cur  = screen_buffer[index];
        strcpy(prev, cur);
    }
    // clear the last line
    strcpy(screen_buffer[index - 1], "\0");
}

/*
 * Why I made the this function?
 * Because output box have to make some place to put the message data
 */
static inline void
make_empty_lines(char **screen_buffer, uint64_t num_of_line) {
    uint64_t index = 0;
    for(;index < num_of_line; index++)
        __make_empty_lines__(screen_buffer);
}

static inline void
push_the_message(char **screen_buffer, struct message* msg, uint64_t num_of_line) {
    uint64_t index = 0;
    uint64_t screen_loc = 0;
    char** data = (char **)msg->data;
    for(;index < num_of_line; index++){
        screen_loc = max_output_line - (num_of_line - index + 1);
        if(index == 0){
            sprintf(screen_buffer[screen_loc+1],">> sender: %s", msg->sender.name);
        } else {
            strcpy(screen_buffer[screen_loc+1], data[index - 1]);
        }
    }
}

static inline void
print_the_screen(struct screen* screen) {
    int index = 0;
    for(; index < max_output_line; index++) {
        // sequence ==> y, x, data
        mvwprintw(screen->window, index + 1, 1, "%s", screen->buffer[index]);
    }
}

void output_loop(struct screen* screen, void* data) {
    struct message *msg = (struct message *)data;
    uint64_t msg_line = check_msg_line((const char **)(msg->data));
    wclear(screen->window);
    make_empty_lines(screen->buffer, msg_line);
    push_the_message(screen->buffer, msg, msg_line);
    print_the_screen(screen);

    box(screen->window, 0, 0);
    mvwprintw(screen->window, 0, 1, OUTPUT_BOX_NAME);
    wrefresh(screen->window);
}


void output_close(struct screen* screen) {
    uint64_t index = 0;
    window_border_clear(screen->window);
    wrefresh(screen->window);
    delwin(screen->window);
    for(index = 0; index < max_output_line; index++)
        free(screen->buffer[index]);
    free(screen->buffer);
}