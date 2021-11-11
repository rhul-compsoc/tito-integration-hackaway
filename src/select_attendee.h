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

struct AttendeeSelection select_attendee(TitoApi /*api*/,
                                         std::list<TitoAttendee> /*attendees*/,
                                         std::string /*message*/);
