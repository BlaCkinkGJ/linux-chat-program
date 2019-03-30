#include "output.h"
#include <stdlib.h>
#include <string.h>

#define DEFAULT 0
#define window_border_clear(win) wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ')

struct screen_operation output_s_op = {
    .init  = output_init,
    .loop  = output_loop,
    .close = output_close,
    .add   = output_list_add,
    .del   = output_list_del
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
    WINDOW *window = screen->window;
    int height     = screen->size.height;
    int width      = screen->size.width;
    struct point start = {
        .x = screen->size.start.x,
        .y = screen->size.start.y
    };

    window = newwin(height, width, start.y, start.x);
    box(window, DEFAULT, DEFAULT);
    wrefresh(window);
}

void output_loop(struct screen* screen) {
    int start = 0;
    char *display = (char *)malloc(sizeof(char)*MAX_CHAR_SIZE*MAX_OUTPUT_LINE);
    
    if(screen->list.size > MAX_OUTPUT_LINE)
        start = screen->list.size - MAX_OUTPUT_LINE - 1;

    // 밑에서부터 위로 올라가면서 쌓이고, 쌓은 후에 빼면 됩니다. 
    for(;start < screen->list.size; start++) {
        wprintw(screen->window, "%s\n", );
    }
    free(display);
}

void output_close(struct screen* screen) {
    window_border_clear(screen->window);
    wrefresh(screen->window);
    delwin(screen->window);

    // delete the data of linked list
    while (screen->list.size > 0) {
        struct list_node* node  = screen->list.l_op->get_first(&screen->list);
        struct message* message = (struct message *)node->data;
        free(message->sender.name);
        free(message->data);
        screen->list.l_op->remove_first(&screen->list);
    }
}

void output_list_add(struct screen* screen, void *data){
    struct message *msg = (struct message *)data;
    screen->list.l_op->add_last(&(screen->list), msg);
}

void output_list_del(struct screen* screen){
    screen->list.l_op->remove_first(&(screen->list));
}