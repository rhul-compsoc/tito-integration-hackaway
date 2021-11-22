#pragma once
#include <list>
#include "tito_classes.h"
#include "tito.h"

/**
 * Sometimes the user wants to cancel the operation, as such the program should
 * support this. A datatype for the response is needed. It could be a c++ object
 * but a struct does the job you know.
 */ 
struct AttendeeSelection {
    bool attendeeSelected;
    TitoAttendee attendee;
};

/**
 * Selects an attendee from a screen.
 * 
 * @param TitoApi api used for checkin
 * @param std::list<TitoAttendee> the attendees list to display
 * @param std::string the message to show the users when they are selecting an attendee
 * @param bool whether or not confirmation is required before the attendee is selected
 */
struct AttendeeSelection select_attendee(TitoApi &/*api*/,
                                         std::list<TitoAttendee> /*attendees*/,
                                         std::string /*message*/,
                                         bool /*confirmationRequired*/);
