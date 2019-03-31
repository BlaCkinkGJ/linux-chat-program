#include "input.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DEFAULT 0
#define window_border_clear(win) wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ')

static uint64_t      max_input_line;
static struct point  pos;

struct screen_operation input_s_op = {
    .init  = input_init,
    .loop  = input_loop,
    .close = input_close,
};

void set_cursor_pos(const struct point new_pos) {
    pos.x = new_pos.x;
    pos.y = new_pos.y;
}

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
    WINDOW *window = screen->window;
    int height     = screen->size.height;
    int width      = screen->size.width;
    int index      = 0;

    struct point start = {
        .x = screen->size.start.x,
        .y = screen->size.start.y
    };

    max_input_line = height - 2; // because of the box's occupy line

    screen->buffer = (char**)malloc(sizeof(char*)*(max_input_line));
    for(index = 0; index < max_input_line; index++){
        screen->buffer[index] = (char *)malloc(sizeof(char) * (width - 1));
        screen->buffer[index][0] = '\0';
    }

    window = newwin(height, width, start.y, start.x);
    box(window, DEFAULT, DEFAULT);
    wrefresh(window);
}

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

static inline void
push_char_to_buffer(struct screen* screen, const struct screen_raw_input *input) {
    const bool is_line_full = pos.x >= (screen->size.width - 1);
    const bool is_box_full = pos.y >= max_input_line;
    __push_char_to_buffer__(screen->buffer, input->data, is_line_full, is_box_full);
    sprintf(screen->buffer[0], ">> sender: %s", (char *)input->user.name);
}

static inline void
pop_char_from_buffer(struct screen* screen) {
    // 'pos.x - 1' has '\0' data. so, 'pos-2' is correct
    const bool is_avail_back_in_line = pos.x - 1 >= 1;
    const bool is_avail_back_in_box  = pos.y - 1 >= 1;
    screen->buffer[pos.y][pos.x - 2] = '\0';

    if (is_avail_back_in_line) {
        pos.x -= 1;
    } else {
        if (!is_avail_back_in_box) {
            pos.x = 1;
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
        mvwprintw(screen->window, index, 1, "%s", screen->buffer[index]);
}


void input_loop(struct screen* screen, void* data) {
    struct screen_raw_input *input = (struct screen_raw_input *)data;
    wclear(screen->window);

    if ( screen->input != KEY_BACKSPACE) {
        push_char_to_buffer(screen, input);
    } else {
        pop_char_from_buffer(screen);
    }

    print_the_screen(screen);

    box(screen->window, 0, 0);
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