#pragma once
#include <ncurses.h>
#include <string>
#include <list>
#include "tito_classes.h"

#define COLOUR_WHITE_BLACK 0
#define COLOUR_PAIR_WHITE_AND_BLACK COLOR_PAIR(COLOUR_WHITE_BLACK)

#define COLOUR_GREEN_BLACK 1
#define COLOUR_PAIR_GREEN_AND_BLACK COLOR_PAIR(COLOUR_GREEN_BLACK)

#define COLOUR_YELLOW_AND_BLACK 2
#define COLOUR_PAIR_YELLOW_AND_BLACK COLOR_PAIR(COLOUR_YELLOW_AND_BLACK)

#define COLOUR_RED_AND_BLACK 3
#define COLOUR_PAIR_RED_AND_BLACK COLOR_PAIR(COLOUR_RED_AND_BLACK)

// 0xE06323
#define COLOUR_ORANGE_AND_BLACK 4
#define COLOUR_PAIR_ORANGE_AND_BLACK COLOR_PAIR(COLOUR_ORANGE_AND_BLACK)

#define COLOUR_BLACK_AND_GREEN 5
#define COLOUR_PAIR_BLACK_AND_GREEN COLOR_PAIR(COLOUR_BLACK_AND_GREEN)

/**
 * Sometimes the user wants to cancel the operation, as such the program should
 * support this. A datatype for the response is needed. It could be a c++ object
 * but a struct does the job you know.
 */ 
struct AttendeeSelection {
    bool attendeeSelected;
    TitoAttendee attendee;
};

int print_logo_left(int /*x_offset*/, int /*y_offset*/, int /*force_small*/);
int print_logo_centre(int /*x_offset*/, int /*y_offset*/, int /*force_small*/);
int print_left(int /*x_offset*/, int /*y_offset*/, std::string /*str*/);
int print_centre(int /*x_offset*/, int /*y_offset*/, std::string /*str*/);
void setup_colours();
/**
 * Opens the select screen for a tito attendee where users can search for an 
 * attendee from a list then select it. This is to be used for the manual check
 * in process and for querying users.
 * 
 * The list of attendees will have a search feature for unique identifiers for
 * the attendees (name, email and, phone number) as well as showing if they are
 * checked in and their ticket type.
 * 
 * In this menu the user can press <v> to open a more verbose information page
 * for the user and they can also press <esc> to cancel the operation. The 
 * <enter> button will be used for selection of the attendee.
 * 
 * @param std::list<TitoAttendee> the list of attendees to choose from
 * @param std::string A string message to show to the user so that they know
 * what they are selecting
 * @return struct AttendeeSelection a copy of the select attendee if one was
 * selected, and a boolean flag to see if a selection was made.
 */
struct AttendeeSelection select_attendee(std::list<TitoAttendee>,
                                         std::string /*message*/);
/**
 * Prints an attendee column on the screen, this is used for the
 * select_attendees method and is defined here because I felt like it.
 * 
 * @param int y_offset how far from the top of the page the attendee should be
 * printed.
 * @param TitoAttendee the attendee to print the information about
 * @see select_attendee for the print format
 */ 
void print_attendee(int /*y_offset*/ TitoAttendee);
