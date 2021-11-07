#include <stdlib.h>
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
                          getEventSlug());
}

void TestTito::testCorrectToken()
{
    TitoApi api = TitoApi(getToken(),
                          getAccountSlug(),
                          getEventSlug());
    CPPUNIT_ASSERT(api.checkAuthToken());
}

void TestTito::testWrongToken()
{    
    int e = TITO_ACCESS_TOKEN_ERROR + 1;
    try {
        TitoApi api = TitoApi("wrong token",
                              getAccountSlug(),
                              getEventSlug());
    } catch (int err) {
        e = err;
    }
    CPPUNIT_ASSERT(e == TITO_ACCESS_TOKEN_ERROR);
}

