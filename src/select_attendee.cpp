#include <iostream>
#include <sstream>
#include <vector>
#include "ncurses_utils.h"
#include "view_attendee.h"
#include "select_attendee.h"
#include "error_screen.h"

#define BACKSPACE 127

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
                                    std::string searchMessage)
{
    // Print logo
    int y = SELECTION_Y_PADDING;
    attron(COLOUR_PAIR_ORANGE_AND_BLACK);
    y += print_logo_left(SELECTION_X_PADDING, y, 1);
    y += 2;
    attroff(COLOUR_PAIR_ORANGE_AND_BLACK);

    // Print the selection guide
    attron(COLOUR_PAIR_GREEN_AND_BLACK);
    int y_info = SELECTION_Y_PADDING + 1;

    int x = SELECTION_X_PADDING * 2 + 25;
    y_info += print_left(x, y_info,
                         message);
    y_info++;

    y_info += print_left(x, y_info,
                         "Type to search for specific attendees.");
    y_info += print_left(x, y_info,
                         "Press <F8> to refresh the attendee information.");
    y_info += print_left(x, y_info,
                         "Press <F9> to view attendee information.");
    y_info += print_left(x, y_info,
                         "Press <F10> to clear the search box.");
    y_info += print_left(x, y_info,
                         "Press <ENTER> to confirm selection.");
    y_info += print_left(x, y_info,
                         "Press <UP> and <DOWN> to change selection.");
    y_info += print_left(x, y_info,
                         "Press <ESCAPE> to cancel selection.");
    attroff(COLOUR_PAIR_GREEN_AND_BLACK);

    // Print the headings
    attron(COLOUR_PAIR_BLACK_AND_GREEN);

    // Replace the spaces with underscores so that they can be seen
    size_t index = 0;
    while (true) {
        /* Locate the substring to replace. */
        index = searchMessage.find(" ", index);
        if (index == std::string::npos) break;

        /* Make the replacement. */
        searchMessage.replace(index, 1, "_");

        /* Advance index forward so the next iteration doesn't pick it up as well. */
        index += 3;
    }

    // Pad the search field
    std::string search = "  SEARCH: " + searchMessage + "<";
    PAD_STR_TO_TABLE(search);
    y += print_left(SELECTION_X_PADDING, y, search);

    // Pad the headers field
    std::string headers = "  "  CHECKED_IN_HEADER  TABLE_PADDING
                        TICKET_TYPE_HEADER  TABLE_PADDING
                        NAME_HEADER  TABLE_PADDING  TABLE_PADDING TABLE_PADDING
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
    if (ticket.getCheckin().isCheckedin()) {
        ret += " [ x ] ";
    } else {
        ret += " [   ] ";
    }
    PAD_STR(ret,
            sizeof(CHECKED_IN_HEADER TABLE_PADDING) - sizeof(" [   ] "));

    // Tick type
    unsigned int maxTicketTypeLength = sizeof(TICKET_TYPE_HEADER TABLE_PADDING);
    std::string ticketType = ticket.getTicketRelease();
    while (ticketType.size() > maxTicketTypeLength) ticketType.pop_back();

    ret += ticketType;
    PAD_STR(ret,
            sizeof(TICKET_TYPE_HEADER TABLE_PADDING) - ticketType.size() - 1);

    // Name
    unsigned int maxNameLength = sizeof(NAME_HEADER TABLE_PADDING TABLE_PADDING
                               TABLE_PADDING) - 2;
    std::string ticketName = attendee.getName();
    while (ticketName.size() > maxNameLength) ticketName.pop_back();

    ret += ticketName;
    PAD_STR(ret,
            sizeof(NAME_HEADER TABLE_PADDING  TABLE_PADDING TABLE_PADDING)
                  - ticketName.size() - 1);

    // Email address
    unsigned int maxEmailLength = getmaxx(stdscr) - ret.size() - SELECTION_X_PADDING - 7;
    std::string ticketEmail = attendee.getEmail();
    while (ticketEmail.size() > maxEmailLength) ticketEmail.pop_back();

    ret += ticketEmail;
    PAD_STR(ret,
            sizeof(EMAIL_ADDRESS_HEADER TABLE_PADDING) - ticketEmail.size() - 1);

    return ret;
}

#define UPDATE_CACHE() \
for (errorFlag = true; errorFlag;) {\
    try {\
        clear();\
        print_centre(0,\
        getmaxy(stdscr) / 2,\
        "Updating attendee cache...");\
        refresh();\
        std::list<TitoAttendee> tmpattendees = api.getAttendees();\
        attendeesRaw = attendees = tmpattendees;\
        attendees.sort();\
        errorFlag = false;\
    } catch (int e) {\
        struct ErrorAction act;\
        act = showErrorMessage("An error updating the attendee cache.",\
        e);\
        \
        if (act.action == ERROR_ACTION_IGNORE) {\
            errorFlag = false;\
        }\
    }\
}

struct AttendeeSelection select_attendee(TitoApi api,
                                         std::list<TitoAttendee> attendeesRaw,
                                         std::string message)
{
    struct AttendeeSelection ret = {
        /*.attendeeSelected=*/0,
        /*.attendee=*/TitoAttendee()
    };

    // Init the search state
    std::list<TitoAttendee> attendees = std::list<TitoAttendee>(attendeesRaw);
    attendees.sort();
    std::string search = "";
    int currentlySelected = 0,
        scrollOffset = 0;

    bool running = true;
    while (running) {
        clear();
        int y, headersY;
        y = headersY = selection_screen_heading(message, search);
        bool errorFlag = true;
        y++;

        // Print attendees
        int i = 0;
        int yOfSelected = 0;
        TitoAttendee currentlySelectedAttendee;

        for (TitoAttendee attendee : attendees) {
            // Draw overflow indicator
            if (i == 0 && i - scrollOffset < 0) {
                print_left(2 + SELECTION_X_PADDING,
                           y - 1,
                           "^");
            }

            if (i - scrollOffset >= 0
                && y < getmaxy(stdscr) - SELECTION_Y_PADDING) {
                if (i == currentlySelected) {
                    currentlySelectedAttendee = attendee;
                    yOfSelected = y;
                    attron(COLOUR_PAIR_BLACK_AND_GREEN);
                }

                // Print the selected attendee and their tickets
                std::string attendeeRow = getAttendeeTableEntry(attendee,
                                                                attendee.getTicket());
                PAD_STR_TO_TABLE(attendeeRow);
                y += print_left(SELECTION_X_PADDING, y, attendeeRow);

                if (i == currentlySelected) {
                    attroff(COLOUR_PAIR_BLACK_AND_GREEN);
                }
            } else

            // Draw overflow indicator
            if (y == getmaxy(stdscr) - SELECTION_Y_PADDING) {
                print_left(2 + SELECTION_X_PADDING,
                           y,
                           "v");
                break;
            }

            i++;
        }

        // Get and parse input.
        refresh();
        int input = getch();
        std::string searchOld = std::string(search);
        switch (input) {
            // Navigation
            case KEY_UP:
                if (currentlySelected > 0) {
                    currentlySelected--;
                    if (currentlySelected - scrollOffset < 0) {
                        scrollOffset--;
                    }
                }
                break;
            case KEY_DOWN:
                if (currentlySelected < ((int) attendees.size()) - 1) {
                    currentlySelected++;
                    if (yOfSelected + 1 >= getmaxy(stdscr) - SELECTION_Y_PADDING) {
                        scrollOffset++;
                    }
                }
                break;
            // Make selection
            case '\n':
            case KEY_ENTER:
                running = 0;
                ret.attendeeSelected = true;
                ret.attendee = currentlySelectedAttendee;
                break;
            // Cancel
            case KEY_EXIT:
            case ESCAPE:
                running = 0;
                break;
            // Backspace
            case KEY_BACKSPACE:
            case KEY_DL:
            case KEY_DC:
            case BACKSPACE:
            case '\b':
                if (search.size() > 0) {
                    search.pop_back();
                }
                break;
            // Refresh cache
            case KEY_F(8):
                UPDATE_CACHE();
                break;
            // View attendee information
            case KEY_F(9):
                i = 0;
                for (TitoAttendee att : attendees) {
                    if (i == currentlySelected) {
                        view_attendee(api, att);

                        // Update cache
                        UPDATE_CACHE();
                        break;
                    }
                    i++;
                }
                break;
            // Clear search
            case KEY_F(10):
                search = "";
                break;
            // Add chars to search
            default:
                // if the input is text then add it to the search field
                if (input == ' ' || input == '@' || input == '.'
                    || input == '-' || input == '\''
                    || (input >= 'a' && input <= 'z')
                    || (input >= 'A' && input <= 'Z')
                    || (input >= '0' && input <= '9')) {
                    search.push_back(input);
                }
                break;
        }

        // Search if query was changed
        if (searchOld != search) {
            attendees = std::list<TitoAttendee>();
            scrollOffset = 0;
            currentlySelected = 0;
            int i = 0, j = 0;
            for (TitoAttendee attendee : attendeesRaw) {
                if (attendee.matches(search)) {
                    attendees.push_back(attendee);

                    // Fix the scrolling and maintain the selected
                    if (attendee == currentlySelectedAttendee) {
                        currentlySelected = j;
                        scrollOffset = j - getmaxy(stdscr)
                                     + headersY 
                                     + SELECTION_Y_PADDING
                                     + SELECTION_Y_PADDING;
                    }
                    j++;
                }
                i++;
            }
        }
    }

    clear();
    refresh();

    return ret;
}
