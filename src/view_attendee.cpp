#include <ncurses.h>
#include <string>
#include "ncurses_utils.h"
#include "view_attendee.h"

void view_attendee(TitoAttendee attendee) {
    bool flag = true;
    while(flag) {
        // Print in the loop to make terminal zooming and resizing work
        clear();

        int y = 2;
        attron(COLOUR_PAIR_ORANGE_AND_BLACK);
        y += print_logo_centre(0, y, 0);
        attroff(COLOUR_PAIR_ORANGE_AND_BLACK);

        attron(COLOUR_PAIR_GREEN_AND_BLACK);
        std::string msg = "Showing information for " + attendee.getName() + ".";
        y += 3;
        y += print_centre(0, y, msg);
        y += 5;
        attroff(COLOUR_PAIR_GREEN_AND_BLACK);

        attron(COLOUR_PAIR_YELLOW_AND_BLACK);
        if (attendee.getPhoneNumber() != "") {
            y += print_centre(0, y, "Phone Number: " + attendee.getPhoneNumber());
        }
        y += print_centre(0, y, "Email Address: " + attendee.getEmail());

        TitoTicket ticket = attendee.getTicket();
        y += print_centre(0, y, "Ticket Type: " + ticket.getTicketRelease());
        y += print_centre(0, y, "Checked In: "
                         + std::string(ticket.isCheckedin() ? "Y" : "N"));
        if (ticket.isCheckedin()) {
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

        y = getmaxy(stdscr) - 5;
        y += print_centre(0, y, "Press <ENTER> to continue");

        refresh();

        int c = getch();
        switch (c) {
            case '\n':
            case KEY_ENTER:
                flag = false;
                break;
        }
    }

    clear();
    refresh();
}