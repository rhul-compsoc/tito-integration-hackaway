#pragma once
#include <list>
#include "tito_classes.h"

struct AttendeeSelection select_attendee(std::list<TitoAttendee> attendees,
                                         std::string message);
