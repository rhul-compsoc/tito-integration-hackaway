#include <list>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include "testTito.h"
#include "../src/tito.h"
#include "../src/tito_classes.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestTito);

TestTito::TestTito() : CppUnit::TestCase("tito.h tests")
{
    
}

void TestTito::testConstructor()
{
    TitoApi api = TitoApi(getToken(),
                          getAccountSlug(),
                          getEventSlug(),
                          getCheckinSlug());
}

void TestTito::testCheckAuth()
{
    TitoApi api = TitoApi(getToken(),
                          getAccountSlug(),
                          getEventSlug(),
                          getCheckinSlug());
    CPPUNIT_ASSERT(api.checkAuthToken());
}

void TestTito::testGetAttendees()
{
    TitoApi api = TitoApi(getToken(),
                          getAccountSlug(),
                          getEventSlug(),
                          getCheckinSlug());
    std::list<TitoAttendee> attendees = api.getAttendees();
    CPPUNIT_ASSERT(attendees.size() > 0);
    
    // Do note that the test event must not have empty strings for these fields
    for (TitoAttendee attendee : attendees) {
        CPPUNIT_ASSERT(attendee.getName() != "");
        CPPUNIT_ASSERT(attendee.getEmail() != "");
        CPPUNIT_ASSERT(attendee.getPhoneNumber() != "");
    }
}


void TestTito::testWrongToken()
{    
    int e = TITO_ACCESS_TOKEN_ERROR + 1;
    try {
        TitoApi api = TitoApi("wrong token",
                              getAccountSlug(),
                              getEventSlug(),
                              getCheckinSlug());
    } catch (int err) {
        e = err;
    }
    CPPUNIT_ASSERT(e == TITO_ACCESS_TOKEN_ERROR);
}

void TestTito::testDateParser()
{
    struct tm t;
    strptime("2018-06-18T09:35:39.000Z", TITO_DATE_FORMAT, &t);
    
    CPPUNIT_ASSERT(t.tm_mday == 18);
    CPPUNIT_ASSERT(t.tm_mon == 6 - 1); // zero bound lmao
    CPPUNIT_ASSERT(t.tm_year == 2018 - 1900); // see tm_struct.h
    CPPUNIT_ASSERT(t.tm_hour == 9);
    CPPUNIT_ASSERT(t.tm_min == 35);
    CPPUNIT_ASSERT(t.tm_sec == 39);
}

void TestTito::testIDCache()
{
    // Delete the old file if it is present
    if (access(ID_CACHE_FILE, F_OK) != -1) {
        CPPUNIT_ASSERT(remove(ID_CACHE_FILE) == 0);
    }
    
    TitoApi *api = new TitoApi(getToken(),
                               getAccountSlug(),
                               getEventSlug(),
                               getCheckinSlug());
    
    CPPUNIT_ASSERT(access(ID_CACHE_FILE, F_OK) != -1);
    TitoAttendee attendee = TitoAttendee("name",
                                         "email",
                                         "phone number",
                                         TitoTicket());
    
    CPPUNIT_ASSERT(!api->hasIDBeenGiven(attendee));
    api->addIDToCache(attendee);
    CPPUNIT_ASSERT(api->hasIDBeenGiven(attendee));
    
    delete api;
        
    CPPUNIT_ASSERT(access(ID_CACHE_FILE, F_OK) != -1);    
    api = new TitoApi(getToken(),
                      getAccountSlug(),
                      getEventSlug(),
                      getCheckinSlug());
    
    CPPUNIT_ASSERT(api->hasIDBeenGiven(attendee));    
}

void TestTito::testSort()
{
    struct tm t;
    TitoCheckin checkin = TitoCheckin("UUID", true, t, t, t),
                checkedout = TitoCheckin("UUID", false, t, t, t);
    TitoTicket ticket = TitoTicket(1, "slug", "release"),
               ticket2 = TitoTicket(1, "slug", "release");

    ticket.setCheckin(checkin);
    ticket2.setCheckin(checkedout);

    TitoAttendee a = TitoAttendee("a", "a", "a", ticket),
                 b = TitoAttendee("a", "b", "a", ticket),
                 c = TitoAttendee("b", "a", "a", ticket),
                 d = TitoAttendee("a", "a", "a", ticket2);

    CPPUNIT_ASSERT(&a < &b);
    CPPUNIT_ASSERT(&a < &c);
    CPPUNIT_ASSERT(&a < &d);
}

void TestTito::testCheckInThenOut()
{
    TitoApi api = TitoApi(getToken(),
                          getAccountSlug(),
                          getEventSlug(),
                          getCheckinSlug());
    std::list<TitoAttendee> attendees = api.getAttendees();
    CPPUNIT_ASSERT(attendees.size() > 0);

    // Checkout of all the things that are checked in
    for (TitoAttendee attendee : attendees) {
        if (attendee.getTicket().getCheckin().isCheckedin()) {
            CPPUNIT_ASSERT(api.checkoutAttendee(attendee));
        }
    }

    // Check in all of the users then check them out
    for (TitoAttendee attendee : attendees) {
        if (attendee.getTicket().getCheckin().isCheckedin()) {
            CPPUNIT_ASSERT(api.checkinAttendee(attendee));
            CPPUNIT_ASSERT(!api.checkinAttendee(attendee));
            CPPUNIT_ASSERT(api.checkoutAttendee(attendee));
            CPPUNIT_ASSERT(!api.checkoutAttendee(attendee));
        }
    }
}

void TestTito::testTitoCheckin()
{
    CPPUNIT_ASSERT(!TitoCheckin().isCheckedin());
    
    struct tm time1, time2, time3, tmp;
    std::string UUID = "UUID";
    bool deleted = false;
    
    time_t t = time(NULL);
    time1 = *localtime(&t);
    
    sleep(1);
    t = time(NULL);
    time2 = *localtime(&t);
    
    sleep(1);
    t = time(NULL);
    time3 = *localtime(&t);
    
    TitoCheckin c = TitoCheckin(UUID,
                                deleted,
                                time1,
                                time2,
                                time3);
    CPPUNIT_ASSERT(c.getUUID() == UUID);
    CPPUNIT_ASSERT(c.isDeleted() == deleted);
    tmp = c.getCheckInTime();
    CPPUNIT_ASSERT(difftime(mktime(&tmp), mktime(&time1)) == 0);
    
    tmp = c.getDeletedTime();
    CPPUNIT_ASSERT(difftime(mktime(&tmp), mktime(&time2)) == 0);
    
    tmp = c.getLastUpdateTime();    
    CPPUNIT_ASSERT(difftime(mktime(&tmp), mktime(&time3)) == 0);
    
    
    c = TitoCheckin(UUID,
                    !deleted,
                    time1,
                    time2,
                    time3);
    CPPUNIT_ASSERT(c.isDeleted() == !deleted);
}

void TestTito::testTitoTicket()
{
    srand(time(NULL));
    int id = rand();
    std::string slug = "slug" + std::to_string(id),
                release = "release" + std::to_string(id);

    TitoCheckin checkin;
    TitoTicket ticket = TitoTicket(id,
                                   slug,
                                   release);
    ticket.setCheckin(checkin);
    CPPUNIT_ASSERT(id == ticket.getTicketID());
    CPPUNIT_ASSERT(slug == ticket.getTicketSlug());
    CPPUNIT_ASSERT(release == ticket.getTicketRelease());
}

void TestTito::testTitoAttendee()
{
    std::string name = "name",
                email = "email",
                phone = "phone";

    TitoAttendee attendee = TitoAttendee(name,
                                         email,
                                         phone,
                                         TitoTicket());

    CPPUNIT_ASSERT(attendee.getName() == name);
    CPPUNIT_ASSERT(attendee.getEmail() == email);
    CPPUNIT_ASSERT(attendee.getPhoneNumber() == phone);
}

void TestTito::testSearch()
{
    std::string name = "Dave Cohen",
                email = "davecohen@cohendave.davecohen",
                phone = "primitive obsession",
                slug = "slug",
                release = "ticket";
    int id = 69;
    TitoTicket ticket = TitoTicket(id, slug, release);
    TitoAttendee dave = TitoAttendee(name, email, phone, ticket);
    
    CPPUNIT_ASSERT(!dave.matches("Gutin"));
    CPPUNIT_ASSERT(!dave.matches("Gutin " + release));
    CPPUNIT_ASSERT(!dave.matches("Gutin " + name));
    CPPUNIT_ASSERT(!dave.matches("Gutin " + email));
    CPPUNIT_ASSERT(!dave.matches("Gutin " + phone));
    
    CPPUNIT_ASSERT(dave.matches(""));
    CPPUNIT_ASSERT(dave.matches(" "));
    CPPUNIT_ASSERT(dave.matches(name));
    CPPUNIT_ASSERT(dave.matches("Dave"));
    
    CPPUNIT_ASSERT(dave.matches(email));
    CPPUNIT_ASSERT(dave.matches(phone));
    CPPUNIT_ASSERT(dave.matches(release));
        
    CPPUNIT_ASSERT(dave.matches(name + " " + email + " " 
                  + phone + " " + release));
    
    std::string strs[] = {name, email, phone, release};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            CPPUNIT_ASSERT(dave.matches(strs[i] + " " + strs[j]));
        }
    }
}
