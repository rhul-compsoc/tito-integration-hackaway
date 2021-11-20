#include <ncurses.h>
#include <string>
#include "ncurses_utils.h"
#include "confirm_select.h"

bool confirm_attendee(TitoAttendee attendee) {
    bool flag = true,
         confirmed = false;
    while(flag) {
        // Print in the loop to make terminal zooming and resizing work
        clear();

        int y = 2;
        attron(COLOUR_PAIR_ORANGE_AND_BLACK);
        y += print_logo_centre(0, y, 0);
        attroff(COLOUR_PAIR_ORANGE_AND_BLACK);

        attron(COLOUR_PAIR_GREEN_AND_BLACK);
        std::string msg = "Confirm that you wish to select: " 
                        + attendee.getName() + ".";
        y += 3;
        y += print_centre(0, y, msg);
        y += 1;
        attroff(COLOUR_PAIR_GREEN_AND_BLACK);

        attron(COLOUR_PAIR_YELLOW_AND_BLACK);
        if (attendee.getPhoneNumber() != "") {
            y += print_centre(0, y, "Phone Number: " + attendee.getPhoneNumber());
        }
        y += print_centre(0, y, "Email Address: " + attendee.getEmail());

        TitoTicket ticket = attendee.getTicket();
        y += print_centre(0, y, "Ticket Type: " + ticket.getTicketRelease());
        y += print_centre(0, y, "Checked In: "
                          + std::string(ticket.getCheckin().isCheckedin() ? "Y" : "N"));
        if (ticket.getCheckin().isCheckedin()) {
            struct tm createTime = ticket.getCheckin().getCheckInTime();
            y += print_centre(0, y, "Checked In At: "
                              + std::string(asctime(&createTime)));
        }

        if (ticket.getCheckin().isDeleted()) {
            struct tm delTime = ticket.getCheckin().getDeletedTime();
            y += print_centre(0, y, "Checkin Deleted At: "
                              + std::string(asctime(&delTime)));
        }
        attroff(COLOUR_PAIR_YELLOW_AND_BLACK);

        y = getmaxy(stdscr) - 10;
        
        y += print_centre(0, y, "Press <ESCAPE> to cancel selection");
        y += print_centre(0, y, "Press <ENTER> to confirm selection");

        refresh();

        int c = getch();
        switch (c) {
            case '\n':
            case KEY_ENTER:
                flag = false;
                confirmed = true;
                break;
            case KEY_EXIT:
            case ESCAPE:
                flag = false;
                break;
        }
    }

    clear();
    refresh();
    
    return confirmed;
}
