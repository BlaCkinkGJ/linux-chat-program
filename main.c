#include "screen.h"
#include "output.h"
#include "input.h"
#include "user.h"
#include "postbox.h"
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

/////// RELATED IN NEXT ASSIGNMENT FUNCTIONS //////
void get_post(struct postbox* post, char **output, char **user);
void put_post(struct postbox* post, char **output, char **user);

int main(int argc, char *argv[]){

    ////// INITIALIZATION PART /////
    struct postbox post; // This for next assignment

    struct window_size screen_input_size = {
        .height  = 5,
        .width   = 52,
        .start.x = 0,
        .start.y = 19,
    };
    struct screen screen_input;

    struct window_size screen_output_size = {
        .height  = 19,
        .width   = 52,
        .start.x = 0,
        .start.y = 0,
    };
    struct screen screen_output;

    struct window_size screen_user_size = {
        .height  = 24,
        .width   = 28,
        .start.x = (80-28),
        .start.y = 0, // terminal width - user_size.width
    };
    struct screen screen_user;

    struct screen_raw_input input;

    if(argc > 1)
        strcpy((char*)(input.user.name), argv[1]);
    else
        strcpy((char*)(input.user.name), "201424494");

    initscr();
    cbreak();

    keypad(stdscr, TRUE);
    refresh();
    noecho();
    curs_set(0);

    screen_output = output_screen(screen_output_size);
    screen_input  = input_screen(screen_input_size);
    screen_user   = user_screen(screen_user_size);

    screen_user.s_op->init(&screen_user);
    screen_output.s_op->init(&screen_output);
    screen_input.s_op->init(&screen_input);

    screen_user.s_op->loop(&screen_user, (void *)(&input.user));

    // BELOW LINE ONLY USE THIS ASSIGNMENT
    put_post(&post, screen_output.buffer, screen_user.buffer);

    ///// MAIN RUNNING LOOP /////
    while((input.data = getch()) != KEY_F(1)){
        // 'get_post' is function for next assignment
        get_post(&post, screen_output.buffer, screen_user.buffer);
        if((char)input.data == '\n'){
            // get message from input box
            time_t now;
            struct message *msg = (struct message *)malloc(sizeof(struct message));
            time(&now);
            msg->sender = input.user;
            msg->timestamp = now;
            memcpy(msg->data, screen_input.buffer, sizeof(msg->data));
            // update the output screen
            screen_output.s_op->loop(&screen_output, (void *)msg);
            // clear the input screen
            screen_input.s_op->loop(&screen_input, (void *)&input);
            free(msg);
        } else {
            // update the input screen
            screen_input.s_op->loop(&screen_input, (void *)&input);
        }
        // 'put_post' is function for next assignment
        put_post(&post, screen_output.buffer, screen_user.buffer);
    }

    endwin();
    screen_output.s_op->close(&screen_output);
    screen_input.s_op->close(&screen_input);
    return 0;
}

// get data from the local
void get_post(struct postbox* post, char **output, char **user) {
    int index = 0;
    for(index = 0; index < MAX_OUTPUT_LINE; index++){
        strcpy(output[index], post->output[index]);
    }
    for(index = 0; index < MAX_USER_LINE; index++){
        strcpy(user[index], post->user[index]);
    }
}

// put data to post structure
void put_post(struct postbox* post, char **output, char **user) {
    int index = 0;
    for(index = 0; index < MAX_OUTPUT_LINE; index++){
        strcpy(post->output[index], output[index]);
    }
    for(index = 0; index < MAX_USER_LINE; index++){
        strcpy(post->user[index], user[index]);
    }
}
