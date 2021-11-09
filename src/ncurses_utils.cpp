#include <iostream>
#include <sstream>
#include <vector>
#include "ncurses_utils.h"

// Generated on https://cloudapps.herokuapp.com/imagetoascii/
// to be of width 55, 29 height
std::string logo_fifty_five = 
"                     =7ZgB@@@BgS7=                     \n"
"                 .)SB@@QKc/)\\chg@@BU\\'                 \n"
"              !yg@@BS).         '?ZB@@gw~              \n"
"          -/MB@@$c=                 :c0@@Bqv_          \n"
"      `~2Q@@BK*`                       `rhB@@Qs+       \n"
"    rA@@@Nl:                               \"7D@@@H?    \n"
"  *B@@Zr`                                     `^K@@B{  \n"
" V@@S'                                           `X@@M \n"
"7@@c                                               l@@u\n"
"B@@                             vo>       \\o^       B@B\n"
"@@B               rfl          7@@s      {@@o       g@@\n"
"@@B            ,sB@@h         ;@@&      !@@8        g@@\n"
"@@B         `?D@@8/`         \"B@#'     -B@B'        g@@\n"
"@@B       .VB@Bo\"           `B@@,     `8@@:         g@@\n"
"@@B       S@@R`             d@@*      H@@)          g@@\n"
"@@B       .VB@BX:          s@@J      w@@2           g@@\n"
"@@B         `*D@@gv`      ?@@U      r@@N            g@@\n"
"@@B            ,VB@@K    !@@g`     =@@Q`            g@@\n"
"@@B               rX7   `B@B-     `B@@_             g@@\n"
"B@@`                    `cV:       lV:             `B@Q\n"
"c@@u                                               w@@}\n"
" U@@h`                                           `h@@U \n"
"  |B@@S?.                                     `*A@@#?  \n"
"    ^ZB@@&J!                               =u&@@BS^    \n"
"       =u8@@BA\\'                       .)SB@@8y!       \n"
"          `?ZB@@g2~                 !Jg@@BK).          \n"
"              :cR@@@Hx_         _/A@@@$u=              \n"
"                 `rf#@@BSc/)/cZ#@@Bfr`                 \n"
"                     ,xK8B@@@B8Ki:                     \n";

// ... to be of width 25, 13 height
std::string logo_twenty_five =
"       `<FqKfKqo^`       \n"
"    ,}WqL_     _iqW},    \n"
" :f6y!             !J6f= \n"
"cQ,                   _gu\n"
"@?      =_    {y  `H,  r@\n"
"@r   .cRJ'   rB_  9K   <@\n"
"@r  ,@a     \"@!  f8    ^@\n"
"@r   'u0u' `Bi  vB.    ^@\n"
"@?      =, }z  `D,     r@\n"
"uQ,                   ,Qc\n"
" =f6y!             !J6h= \n"
"    _{Wq{_     _iqW},    \n"
"        !tWKhKWj~        \n";

int print_logo_left(int x_offset, int y_offset, int force_small)
{    
    int lines;
    int width = getmaxx(stdscr);
    if (width >= 55 && !force_small) {
        lines = print_left(x_offset, y_offset, logo_fifty_five);
    } else {
        lines = print_left(x_offset, y_offset, logo_twenty_five);
    }
    return lines; 
}

int print_logo_centre(int x_offset, int y_offset, int force_small)
{
    int lines;
    int width = getmaxx(stdscr);
    if (width >= 55 && !force_small) {
        lines = print_centre(x_offset, y_offset, logo_fifty_five);
    } else {
        lines = print_centre(x_offset, y_offset, logo_twenty_five);
    }
    return lines;
}

int print_left(int x_offset, int y_offset, std::string str)
{
    int lines = 0;
    std::string line;
    std::stringstream streamData(str);
    while (std::getline(streamData, line, '\n')) {
        mvaddstr(y_offset + lines, x_offset, line.c_str());
        lines++;
    }
    addstr("\n");
    return lines;
}

int print_centre(int x_offset, int y_offset, std::string str)
{
    int width = 0;
    width = getmaxx(stdscr);
    
    int lines = 0;
    std::string line;
    std::stringstream streamData(str);
    while (std::getline(streamData, line, '\n')) {
        int offset = (width - line.size()) / 2;
        mvaddstr(y_offset + lines, x_offset + offset, line.c_str());
        lines++;
    }
    addstr("\n");
    return lines;
}


void setup_colours()
{
    init_pair(COLOUR_WHITE_BLACK,
              COLOR_WHITE,
              COLOR_BLACK);
    
    init_pair(COLOUR_GREEN_BLACK,
              COLOR_GREEN,
              COLOR_BLACK);
    
    init_pair(COLOUR_YELLOW_AND_BLACK,
              COLOR_YELLOW,
              COLOR_BLACK);
    
    init_pair(COLOUR_RED_AND_BLACK,
              COLOR_RED,
              COLOR_BLACK);
    
    init_pair(COLOUR_ORANGE_AND_BLACK,
              13, /* Find a better orange */
              COLOR_BLACK);
    
    init_pair(COLOUR_BLACK_AND_GREEN,
              COLOR_BLACK,
              COLOR_GREEN);
}

#define SELECTION_X_PADDING 5
#define SELECTION_Y_PADDING 2

static void selection_screen_heading(std::string message)
{
    // Print logo
    int y = SELECTION_Y_PADDING;
    attron(COLOUR_PAIR_ORANGE_AND_BLACK);
    y += print_logo_left(SELECTION_X_PADDING, y, 1);
    y += 2;
    attroff(COLOUR_PAIR_ORANGE_AND_BLACK);
    
    // Print the selection guide
    attron(COLOUR_PAIR_GREEN_AND_BLACK);    
    int y_info = SELECTION_Y_PADDING + 3;
    int x = SELECTION_X_PADDING * 2 + 25;
    y_info += print_left(x, y_info,
                         message);
    y_info += print_left(x, y_info,
                         "Type to search for specific attendees.");
    y_info += print_left(x, y_info,
                         "Use <ENTER> to confirm selection.");
    y_info += print_left(x, y_info,
                         "Use <UP> and <DOWN> to change selection.");
    y_info += print_left(x, y_info,
                         "Use <ESCAPE> to cancel selection.");
    attroff(COLOUR_PAIR_GREEN_AND_BLACK);
    
    // Print the headings
    attron(COLOUR_PAIR_BLACK_AND_GREEN);
    
    std::string headers = "  CHECKED IN     TICKET TYPE     NAME     EMAIL ADDRESS";
    int paddingNeeded = getmaxx(stdscr) - headers.size() - 2 * SELECTION_X_PADDING;
    for (int i = 0; i < paddingNeeded; i++)
        headers += " "; // Pad the headers column
    
    print_left(SELECTION_X_PADDING, y, headers);
    attroff(COLOUR_PAIR_BLACK_AND_GREEN);
    refresh();
}

struct AttendeeSelection select_attendee(std::list<TitoAttendee> attendees,
                                         std::string message)
{
    clear();
    refresh();
    struct AttendeeSelection ret = {
        /*.attendeeSelected=*/0,
        /*.attendee=*/TitoAttendee()
    };
    
    int running = 1;
    while (running) {
        selection_screen_heading(message);
        
        // Get and parse input.
        int input = getch();
        switch (input) {
            case KEY_EXIT:
            case 27: // This is what it is on my machine
                running = 0;
            default:
                continue;
        }
    }
    
    return ret;
}

