/**************************
 * 어떻게 shared pointer 문제를 해결할 것인가?
 * 이번 방법에서 해결책은 배열 구조체를 사용하는 것이다.
 **************************/

#include "screen.h"
#include "output.h"
#include "input.h"
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]){
    struct window_size screen_input_size = {
        .height  = 5,
        .width   = 52,
        .start.y = (LINES - 5),
        .start.x = 0,
    };
    struct screen screen_input;

    struct window_size screen_output_size = {
        .height  = 19,
        .width   = 28,
        .start.y = 0,
        .start.x = 0,
    };
    struct screen screen_output;

    struct screen_raw_input input;

    strcpy((char*)(input.user.name), argv[1]);

    initscr();
    cbreak();

    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    screen_input  = input_screen(screen_input_size);
    screen_output = output_screen(screen_output_size);

    screen_input.s_op->init(&screen_input);
    screen_output.s_op->init(&screen_output);

    refresh();

    while((input.data = getch()) != KEY_F(1)){
        if((char)input.data == '\n'){
            struct point pos = {1, 1};
            time_t now;
            time(&now);
            struct message msg = {
                .sender    = input.user,
                .timestamp = now,
            };
            memcpy(msg.data, screen_input.buffer, sizeof(msg.data));
            set_cursor_pos(pos);
            screen_output.s_op->loop(&screen_output, (void *)&msg);
        } else {
            screen_input.s_op->loop(&screen_input, (void *)&input);
        }
    }

    endwin();
    screen_output.s_op->close(&screen_output);
    screen_input.s_op->close(&screen_input);
    return 0;
}