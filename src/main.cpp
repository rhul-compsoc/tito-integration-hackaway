#include <stdlib.h>
#include <ncurses.h>
#include "ncurses_utils.h"

int main(int argc, char **argv)
{
    initscr();
    
    // Get all input in real time
    keypad(stdscr, TRUE);
    cbreak();
    noecho();
    
    // Init colours
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal is shit and does not support colour.\n");
        exit(1);
    }
    
    start_color();
    setup_colours();
    
    attron(COLOUR_PAIR_ORANGE_AND_BLACK);
    print_logo_centre(0, 0, 0);
    attroff(COLOUR_PAIR_ORANGE_AND_BLACK);
    
    attron(COLOUR_PAIR_GREEN_AND_BLACK);
    addstr("Royal hackaway tito integration.");
    attroff(COLOUR_PAIR_GREEN_AND_BLACK);
    refresh();
    
    addstr("\npress any key to exit...");
    refresh();
    
    getch();
    
    endwin();
    
    return EXIT_SUCCESS;
}
