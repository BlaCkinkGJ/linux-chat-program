#include "input.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DEFAULT 0
#define window_border_clear(win) wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ')
#define INPUT_BOX_NAME "input"

static uint64_t      max_input_line;
static struct point  pos;

struct screen_operation input_s_op = {
    .init  = input_init,
    .loop  = input_loop,
    .close = input_close,
};

// constructor of input screen
struct screen input_screen (struct window_size size) {
    struct screen input_screen;
    input_screen.s_op = &input_s_op;

    input_screen.size.height  = size.height;
    input_screen.size.width   = size.width;
    input_screen.size.start.x = size.start.x;
    input_screen.size.start.y = size.start.y;

    return input_screen;
}

void input_init(struct screen* screen) {
    int height     = screen->size.height;
    int width      = screen->size.width;
    int index      = 0;

    struct point start = {
        .x = screen->size.start.x,
        .y = screen->size.start.y
    };

    max_input_line = height - 2; // because of the box's occupy line

    pos.x = 1; pos.y = 0;

    screen->buffer = (char**)malloc(sizeof(char*)*(max_input_line));
    for(index = 0; index < max_input_line; index++){
        /*
         * Why doesn't 'width - 2'?
         * because of c string need '\0'. So, just use 'width - 1'
         */
        screen->buffer[index] = (char *)malloc(sizeof(char) * (width - 1));
        screen->buffer[index][0] = '\0';
    }

    screen->window = newwin(height, width, start.y, start.x);
    box(screen->window, DEFAULT, DEFAULT);
    mvwprintw(screen->window, 0, 1, INPUT_BOX_NAME);
    wrefresh(screen->window);
}

// push_char_to_buffer's wrapper function
static inline void
__push_char_to_buffer__(char **buffer, const char data, 
    const bool is_line_full, const bool is_box_full) {
    if(!is_line_full) {
        buffer[pos.y][pos.x - 1] = data;
        buffer[pos.y][pos.x] = '\0';
        pos.x += 1;
    } else {
        if (!is_box_full) {
            pos.x = 1;
            pos.y = pos.y + 1;
            buffer[pos.y][pos.x - 1] = data;
            buffer[pos.y][pos.x] = '\0';
        } // end of if
    } // end of if
}

// push the one character value to the input screen's buffer
static inline void
push_char_to_buffer(struct screen* screen, const struct screen_raw_input *input) {
    const bool is_line_full = pos.x >= (screen->size.width - 1);
    const bool is_box_full = pos.y >= (max_input_line - 1);
    __push_char_to_buffer__(screen->buffer, input->data, is_line_full, is_box_full);
}

// This for if I type the backspace then remove one character
static inline void
pop_char_from_buffer(struct screen* screen) {
    const bool is_avail_back_in_line = ((int)(pos.x) - 1 >= 1);
    const bool is_avail_back_in_box  = ((int)(pos.y) - 1 >= 0);

    // 'pos.x - 1' has '\0' data. so, 'pos-2' is correct
    screen->buffer[pos.y][pos.x - 2] = '\0';

    if (is_avail_back_in_line) {
        pos.x -= 1;
    } else { // This section move the previous line 
        if (!is_avail_back_in_box) { // box's first position
            pos.x = 1;
            pos.y = 0;
        } else {
            pos.x = 50;
            pos.y -= 1;
        } // end of if
    } // end of if
}

static inline void
print_the_screen(struct screen* screen) {
    int index = 0;
    for(index = 1; index <= max_input_line; index++)
        mvwprintw(screen->window, index, 1, "%s", screen->buffer[index - 1]);
}


void input_loop(struct screen* screen, void* data) {
    struct screen_raw_input *input = (struct screen_raw_input *)data;
    wclear(screen->window);

    if (input->data == KEY_BACKSPACE) {
        pop_char_from_buffer(screen);
    } else if ((char)(input->data) != '\n') {
        push_char_to_buffer(screen, input);
    } else {
        pos.x = 1;
        pos.y = 0;
        clear_buffer(screen->buffer, screen->size.height - 2);
    }

    print_the_screen(screen);

    box(screen->window, 0, 0);
    mvwprintw(screen->window, 0, 1, INPUT_BOX_NAME);
    wrefresh(screen->window);
}


void input_close(struct screen* screen) {
    uint64_t index = 0;
    window_border_clear(screen->window);
    wrefresh(screen->window);
    delwin(screen->window);
    for(index = 0; index < max_input_line; index++)
        free(screen->buffer[index]);
    free(screen->buffer);
}