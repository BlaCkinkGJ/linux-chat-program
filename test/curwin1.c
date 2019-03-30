#include <ncurses.h>

#define IS_VALID(input) \
    input == KEY_LEFT || \
    input == KEY_RIGHT || \
    input == KEY_DOWN || \
    input == KEY_UP

struct point {
    int x;
    int y;
};

struct rectangle {
    struct point start;
    uint64_t height;
    uint64_t width;
};

typedef int std_input;

WINDOW *create_new_window (struct rectangle win_size);
void destroy_window(WINDOW *local_window);

int main(int argc, char *argv[]) {
    WINDOW *my_window;
    struct rectangle win_size;
    std_input input;

    initscr(); // start curses mode
    cbreak(); // line buffering disabled
    
    keypad(stdscr, TRUE);

    win_size.height = 3;
    win_size.width = 10;
    win_size.start.y = (LINES - win_size.height) / 2; // calculating for a center placement
    win_size.start.x = (COLS - win_size.width) / 2;

    printw("Press F1 to exit");
    refresh();
    noecho();

    my_window = create_new_window(win_size);

    while((input = getch()) != KEY_F(1)) {
        clear();
        refresh();
        char buffer[256];
        if(IS_VALID(input)){
            destroy_window(my_window);
            switch (input) {
                case KEY_LEFT:
                win_size.start.x--;
                break;
                case KEY_RIGHT:
                win_size.start.x++;
                break;
                case KEY_DOWN:
                win_size.start.y++;
                break;
                case KEY_UP:
                win_size.start.y--;
                break;
            }
        }
        my_window = create_new_window(win_size);
        mvwprintw(my_window,1,1, "%c", (char)input);
        wrefresh(my_window);
        wclear(my_window);
    }

    endwin();
    return 0;
}

WINDOW *create_new_window(struct rectangle win_size) {
    WINDOW *local_win;

    local_win = newwin(win_size.height, win_size.width, win_size.start.y, win_size.start.x);
    box(local_win, 0, 0);

    wrefresh(local_win);

    return local_win;
}

void destroy_window(WINDOW *local_win) {
    //wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(local_win);
    delwin(local_win);
}