#include <stdlib.h>
#include <ncurses.h>

int main(int argc, char **argv) {    
    initscr();
    
    addstr("Royal hackaway tito integration.");
    refresh();
    
    addstr("\npress any key to exit...");
    refresh();
    
    getch();
    
    endwin();
    
    return EXIT_SUCCESS;
}
