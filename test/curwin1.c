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
    int x = 1;
    int y = 1;
    char buffer[3][51];

    initscr(); // start curses mode
    cbreak(); // line buffering disabled
    
    keypad(stdscr, TRUE);

    win_size.height = 5;
    win_size.width = 52;
    win_size.start.y = (LINES - win_size.height) / 2; // calculating for a center placement
    win_size.start.x = (COLS - win_size.width) / 2;

    printw("Press F1 to exit");
    refresh();
    noecho();
    curs_set(0);

    my_window = create_new_window(win_size);

    buffer[0][0] = '\0';
    buffer[1][0] = '\0';
    buffer[2][0] = '\0';
    while((input = getch()) != KEY_F(1)) {
        refresh();
        box(my_window, 0, 0);
        if('A' <= (char)input <= 'z'){
            if ( x < win_size.width - 1){
                buffer[y-1][x-1] = (char)input;
                buffer[y-1][x] = (char)'\0';
                x++;
                //mvwprintw(my_window,y,x++, "%c", (char)input);
            }
            else{
                if ( y < win_size.height - 2) { 
                    x = 1;
                    y = y + 1;
                    buffer[y-1][x-1] = (char)input;
                    buffer[y-1][x] = (char)'\0';
                    //mvwprintw(my_window,y,x, "%c", (char)input);
                }
            }
            mvwprintw(my_window, 1, 1, "%s", buffer[0]);
            mvwprintw(my_window, 2, 1, "%s", buffer[1]);
            mvwprintw(my_window, 3, 1, "%s", buffer[2]);
        }
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