#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <list>
#include "testTito.h"
#include "../src/tito.h"

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

