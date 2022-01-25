#include <ncurses.h>
#include <string>
#include "tito.h"
#include "ncurses_utils.h"
#include "error_screen.h"

struct ErrorAction showErrorMessage(std::string message,
                                    int errorCode)
{
    bool flag = true;
    struct ErrorAction ret = {ERROR_ACTION_RETRY};
    while(flag) {
        // Print in the loop to make terminal zooming and resizing work
        clear();

        int y = 2;
        attron(COLOUR_PAIR_ORANGE_AND_BLACK);
        y += print_logo_centre(0, y, 0);
        attroff(COLOUR_PAIR_ORANGE_AND_BLACK);

        attron(COLOUR_PAIR_GREEN_AND_BLACK);
        y += 3;
        y += print_centre(0, y, message);
        y += 3;
        attroff(COLOUR_PAIR_GREEN_AND_BLACK);

        attron(COLOUR_PAIR_RED_AND_BLACK);
        std::string msg = "An error with code " + std::to_string(errorCode) + " has occurred.";
        y += print_centre(0, y, msg);
        y += print_centre(0, y, "(" + getTitoErrorMessage(errorCode) + ")");
        attroff(COLOUR_PAIR_RED_AND_BLACK);

        y = getmaxy(stdscr) - 5;
        y += print_centre(0, y, "Press <R> to retry");
        y += print_centre(0, y, "Press <ENTER> to ignore");

        refresh();

        int c = getch();
        switch (c) {
        case '\n':
        case KEY_ENTER:
            ret.action = ERROR_ACTION_IGNORE;
            flag = false;
            break;
        default:
            if (c == 'R' || c == 'r') {
                ret.action = ERROR_ACTION_RETRY;
                flag = false;
            }
            break;
        }
    }

    clear();
    refresh();

    return ret;
}
