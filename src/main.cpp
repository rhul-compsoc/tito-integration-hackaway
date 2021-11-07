#include <stdlib.h>
#include <ncurses.h>
#include <string>
#include "ncurses_utils.h"
#include "tito.h"

static void warn_exit()
{    
    attron(COLOUR_PAIR_YELLOW_AND_BLACK);
    print_centre(0, getmaxy(stdscr) - 2, "Press any key to exit...");
    refresh();
    attroff(COLOUR_PAIR_YELLOW_AND_BLACK);
    
    getch();
    
    endwin();
}

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
    
    int y = 0;
    
    attron(COLOUR_PAIR_ORANGE_AND_BLACK);
    y += print_logo_centre(0, y, 0);
    attroff(COLOUR_PAIR_ORANGE_AND_BLACK);
    
    attron(COLOUR_PAIR_GREEN_AND_BLACK);
    y += 3;
    y += print_centre(0, y, "Loading Royal Hackaway TiTo integration...");
    attroff(COLOUR_PAIR_GREEN_AND_BLACK);
    refresh();
    
    try {
        TitoApi api(getToken(), getAccountSlug(), getEventSlug());
        std::list<TitoAttendee> attendees = api.getAttendees();
    } catch (int e) {
        attron(COLOUR_PAIR_RED_AND_BLACK);
        std::string msg = "An error with code " + std::to_string(e) + " has occurred.";
        y += print_centre(0, y, msg);
        y += print_centre(0, y, "(" + getTitoErrorMessage(e) + ")");
        
        warn_exit();
        attroff(COLOUR_PAIR_RED_AND_BLACK);
        return EXIT_FAILURE;
    }
    
    y += print_centre(0, y, "Loaded attendees and checkins.");
    y += print_centre(0, y, "Loading id cache.");
    
    warn_exit();
    
    return EXIT_SUCCESS;
}
