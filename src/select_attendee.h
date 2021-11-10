#pragma once
#include <list>
#include "tito_classes.h"
#include "tito.h"

struct AttendeeSelection select_attendee(TitoApi /*api*/,
                                         std::list<TitoAttendee> /*attendees*/,
                                         std::string /*message*/);
