#include <stdlib.h>
#include <ncurses.h>
#include <string>
#include "ncurses_utils.h"
#include "select_attendee.h"
#include "error_screen.h"
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

static void print_logo(int *y)
{    
    attron(COLOUR_PAIR_ORANGE_AND_BLACK);
    *y += print_logo_centre(0, *y, 0);
    attroff(COLOUR_PAIR_ORANGE_AND_BLACK);
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
    
    int y;
    
    TitoApi api;
    std::list<TitoAttendee> attendees;
    bool flag = true;
    while (flag) {
        y = 2;
        print_logo(&y);

        attron(COLOUR_PAIR_GREEN_AND_BLACK);
        y += 3;
        y += print_centre(0, y, "Loading Royal Hackaway TiTo integration...");
        attroff(COLOUR_PAIR_GREEN_AND_BLACK);
        refresh();

        try {
            api = TitoApi(getToken(), getAccountSlug(), getEventSlug(), getCheckinSlug());
            attendees = api.getAttendees();
            flag = false;
        } catch (int e) {
            struct ErrorAction act;
            act = showErrorMessage("An error occurred during initialisation",
                                   e);

            if (act.action == ERROR_ACTION_IGNORE) {
                print_centre(0, getmaxy(stdscr) / 2,
                             "Initialisation failed, see error log.");
                warn_exit();
                return EXIT_FAILURE;
            }
        }
    }

    
    y += print_centre(0, y, "Loaded ID cache, attendees and, checkins.");
    select_attendee(api, attendees, "Select an attendee.");
    
    warn_exit();
    
    return EXIT_SUCCESS;
}
