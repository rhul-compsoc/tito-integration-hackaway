#pragma once
#include "tito.h"
#include "tito_classes.h"

/**
 * Shows the user information about an attendee in a full screen prompt.
 * The option to checkin/out a user is shown to make the attendee management
 * easier.
 *
 * @param TitoApi used for checkin
 * @param TitoAttendee the attendee to show information about
 * @return whether the attendee cache needs to be updated
 */
bool view_attendee(TitoApi &/*api*/, TitoAttendee /*attendee*/);
