#include "screen.h"
#include "output.h"
#include "input.h"
#include "user.h"
#include "postbox.h"
#include "sys_V_semaphore.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

struct postbox *post;

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

void initpostbox(struct postbox *post);
void mainsetup();
void mainloop();
void mainend();

bool get_post(struct postbox *post, char **output, char **user);
void put_post(struct postbox *post, char **output, char **user);

int main(int argc, char *argv[]){
    key_t key = ftok(".", 201424494);
    int shm_id;

    shm_id = shmget(key, sizeof(struct postbox), IPC_CREAT | IPC_EXCL | 0666);
   

    if(shm_id < 0) {
        shm_id = shmget(key, sizeof(struct postbox), 0666);
        post = (struct postbox*)shmat(shm_id, 0, 0);
        post->user_counter += 1;
    } else {
        pid_t proc_id = fork();
        post = (struct postbox*)shmat(shm_id, 0, 0);
        initpostbox(post);
        if ( proc_id == 0 ) {
            while(post->user_counter != 0);
            shmctl(shm_id, IPC_RMID, 0);
            sem_destroy(&post->sem);
            exit(0);
        }
    }

    if(sem_init(&post->sem, key) == -1){
        if(sem_open(&post->sem, key) == -1){
            goto ERROR;
        }
    }

    mainsetup(argc, argv);
    sem_wait(&post->sem);
    if(set_user_id(post, input.user.name) == -1){
        goto ERROR;
    }
    screen_output.s_op->loop(&screen_output, NULL);
    screen_user.s_op->loop(&screen_user, NULL);
    sem_post(&post->sem);
    ///// MAIN RUNNING LOOP /////
    while(input.data != KEY_F(1)){
        static bool is_change = false;
        is_change = get_post(post, screen_output.buffer, screen_user.buffer);
        if(kbhit()) {
            input.data = getch();
            mainloop();
        }
        if (is_change == true){
            screen_output.s_op->loop(&screen_output, NULL);
            screen_user.s_op->loop(&screen_user, NULL);
        }
    }

    sem_wait(&post->sem);
    remove_user_id(post);
    mainend();

    post->user_counter -= 1;
    sem_post(&post->sem);

    return 0;
ERROR:
    mainend();
    fprintf(stderr, "Error Occurred!!");
    post->user_counter -= 1;
    return -1;
}

void initpostbox(struct postbox *post){
    int index = 0;
    post->user_counter = 1;
    for(index = 0; index < MAX_USER_LINE; index++) {
        post->user[index].id = INVALID_USER_ID;
        post->user_login[index] = false;
    }
}

static uint64_t prev_counter = 0;
// get data from the local
bool get_post(struct postbox *post, char **output, char **user) {
    bool is_change = false;
    if(prev_counter != post->user_counter){
        is_change |= true;
        prev_counter = post->user_counter;
    }
    set_flag_and_copy_post(COPY_MESSAGE, &is_change, MAX_OUTPUT_LINE, post, output);
    set_flag_and_copy_post(COPY_USER, &is_change, MAX_USER_LINE, post, user);
    return is_change;
}

// put data to post structure
void put_post(struct postbox *post, char **output, char **user) {
    copy_data(MAX_OUTPUT_LINE, post, output);
}

void mainsetup(int argc, char *argv[]) {
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


    if(argc > 1)
        strcpy((char*)(input.user.name), argv[1]);
    else
        gethostname((char*)(input.user.name),sizeof(char)*MAX_USER_WIDTH);
}

void mainloop() {
    // 'get_post' is function for next assignment
    if((char)input.data == '\n'){
        sem_wait(&post->sem);
        // get message from input box
        time_t now;
        struct message *msg = (struct message *)malloc(sizeof(struct message));
        get_post(post, screen_output.buffer, screen_user.buffer);
        time(&now);
        msg->sender = input.user;
        msg->timestamp = now;
        memcpy(msg->data, screen_input.buffer, sizeof(msg->data));
        // update the output screen
        screen_output.s_op->loop(&screen_output, (void *)msg);
        // clear the input screen
        screen_input.s_op->loop(&screen_input, (void *)&input);
        put_post(post, screen_output.buffer, screen_user.buffer);
        free(msg);
        sem_post(&post->sem);
    } else {
        // update the input screen
        screen_input.s_op->loop(&screen_input, (void *)&input);
    }
    // 'put_post' is function for next assignment
}

void mainend() {
    put_post(post, screen_output.buffer, screen_user.buffer);
    screen_output.s_op->close(&screen_output);
    screen_input.s_op->close(&screen_input);
    screen_user.s_op->close(&screen_user);
    endwin();
}

