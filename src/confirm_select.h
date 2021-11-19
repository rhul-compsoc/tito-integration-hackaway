#pragma once
#include "tito.h"
#include "tito_classes.h"

/**
 * Asks the user to confirm the attendee that they selected.
 *
 * @param TitoAttendee the attendee to show information about
 * @return whether the attendee cache needs to be updated
 */
bool confirm_attendee(TitoAttendee /*attendee*/);
