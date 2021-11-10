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
    TitoCheckin checkin = TitoCheckin(true, t, t, t);
    TitoTicket ticket = TitoTicket(1, "slug", "release");

    CPPUNIT_ASSERT(TitoAttendee("a", "a", "a", ticket) <
                   TitoAttendee("a", "b", "a", ticket));
    CPPUNIT_ASSERT(TitoAttendee("a", "a", "a", ticket) <
                   TitoAttendee("b", "a", "a", ticket));
}
