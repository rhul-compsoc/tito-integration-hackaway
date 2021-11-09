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

int print_logo_left(int x_offset,
                    int y_offset,
                    int force_small)
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

int print_logo_centre(int x_offset,
                      int y_offset,
                      int force_small)
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

int print_left(int x_offset,
               int y_offset,
               std::string str)
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

int print_centre(int x_offset,
                 int y_offset,
                 std::string str)
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

// A handy set of macros to pad my strings
#define PAD_STR(str, num) \
for (int i = 0, offset = num; i < offset; i++)\
    str += " ";
#define PAD_STR_TO_TABLE(str) \
PAD_STR(str, getmaxx(stdscr) - str.size() - 2 * SELECTION_X_PADDING)

#define TABLE_PADDING "     "
#define CHECKED_IN_HEADER "CHECKED IN"
#define TICKET_TYPE_HEADER "TICKET TYPE"
#define NAME_HEADER "FULL NAME"
#define EMAIL_ADDRESS_HEADER "EMAIL ADDRESS"

static int selection_screen_heading(std::string message,
                                    std::string *searchMessage)
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
                         "Press <F10> to clear the search.");
    y_info += print_left(x, y_info,
                         "Use <ENTER> to confirm selection.");
    y_info += print_left(x, y_info,
                         "Use <UP> and <DOWN> to change selection.");
    y_info += print_left(x, y_info,
                         "Use <ESCAPE> to cancel selection.");
    attroff(COLOUR_PAIR_GREEN_AND_BLACK);
    
    // Print the headings
    attron(COLOUR_PAIR_BLACK_AND_GREEN);
    // Pad the search field
    std::string search = "  SEARCH: " + *searchMessage;
    PAD_STR_TO_TABLE(search);
    y += print_left(SELECTION_X_PADDING, y, search);
    
    // Pad the headers field
    std::string headers = "  "  CHECKED_IN_HEADER  TABLE_PADDING 
                        TICKET_TYPE_HEADER  TABLE_PADDING 
                        NAME_HEADER  TABLE_PADDING  TABLE_PADDING TABLE_PADDING 
                        TABLE_PADDING
                        EMAIL_ADDRESS_HEADER;
    
    PAD_STR_TO_TABLE(headers);
    y += print_left(SELECTION_X_PADDING, y, headers);    
    attroff(COLOUR_PAIR_BLACK_AND_GREEN);
    
    return y;
}

/**
 * Gets a table row for a ticket. The internal padding of the row is guaranteed
 * to be correct.
 * 
 * @param TitoAttendee the attendee who the ticket belongs to
 * @param TitoTicket the ticket to get the row for
 */
static std::string getAttendeeTableEntry(TitoAttendee attendee,
                                         TitoTicket ticket)
{
    // Checked in status
    std::string ret = "  ";
    if (ticket.isCheckedin()) {
        ret += " [ x ] ";
    } else {
        ret += " [   ] ";
    }    
    PAD_STR(ret,
            sizeof(CHECKED_IN_HEADER TABLE_PADDING) - sizeof(" [   ] "));
    
    // Tick type
    unsigned int maxTicketTypeLength = sizeof(TICKET_TYPE_HEADER TABLE_PADDING) - 2;
    std::string ticketType = ticket.getTicketRelease();
    while (ticketType.size() > maxTicketTypeLength) ticketType.pop_back();
    
    ret += ticketType;
    PAD_STR(ret,
            sizeof(TICKET_TYPE_HEADER TABLE_PADDING) - ticketType.size());
    
    // Name
    unsigned int maxNameLength = sizeof(NAME_HEADER TABLE_PADDING TABLE_PADDING 
                               TABLE_PADDING) - 2;    
    std::string ticketName = attendee.getName();
    while (ticketName.size() > maxNameLength) ticketName.pop_back();
    
    ret += ticketName;
    PAD_STR(ret,
            sizeof(NAME_HEADER TABLE_PADDING  TABLE_PADDING TABLE_PADDING) 
            - ticketName.size());
    
    // Email address
    unsigned int maxEmailLength = getmaxx(stdscr) - ret.size() - 2 - SELECTION_X_PADDING;
    std::string ticketEmail = attendee.getEmail();
    while (ticketEmail.size() > maxEmailLength) ticketEmail.pop_back();
    
    ret += ticketEmail;
    PAD_STR(ret,
            sizeof(EMAIL_ADDRESS_HEADER TABLE_PADDING) - ticketEmail.size());
    
    return ret;
}

struct AttendeeSelection select_attendee(std::list<TitoAttendee> attendees,
                                         std::string message)
{
    struct AttendeeSelection ret = {
        /*.attendeeSelected=*/0,
        /*.attendee=*/TitoAttendee()
    };
    
    // Search state
    std::string search = "";
    int currentlySelected = 0,
        scrollOffset = 0;
    
    int running = 1;
    while (running) {
        clear();
        int y = selection_screen_heading(message, &search);
        y++;
        
        // Print attendees
        int i = 0;
        for (TitoAttendee attendee : attendees) {
            if (i - scrollOffset >= 0 
                && y < getmaxy(stdscr) - SELECTION_Y_PADDING) {
                if (i == currentlySelected) {
                    attron(COLOUR_PAIR_BLACK_AND_GREEN);
                }
                
                // Print the selected attendee and their tickets
                for (TitoTicket ticket : attendee.getTickets()) {
                    std::string attendeeRow = getAttendeeTableEntry(attendee, ticket);
                    PAD_STR_TO_TABLE(attendeeRow);                
                    y += print_left(SELECTION_X_PADDING, y, attendeeRow);
                }
                
                if (i == currentlySelected) {
                    attroff(COLOUR_PAIR_BLACK_AND_GREEN);
                }
            }
            
            i++;
        }
        
        // Get and parse input.        
        refresh();
        int input = getch();
        switch (input) {
            case KEY_EXIT:
            case ESCAPE:
                running = 0;
                break;
            case KEY_BACKSPACE:
            case KEY_DL:
            case KEY_DC:
            case BACKSPACE:
            case '\b':
                if (search.size() > 0) {
                    search.pop_back();
                }
                break;
            default:
                // if the input is text then add it to the search field
                if (input == ' ' || input == '@' || input == '.'
                    || (input >= 'a' && input <= 'z')
                    || (input >= 'A' && input <= 'Z')
                    || (input >= '0' && input <= '9')) {
                    search += input;
                }                
                continue;
        }
    }
    
    return ret;
}

