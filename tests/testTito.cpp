#include <stdlib.h>
#include "testTito.h"
#include "../src/tito.h"

#define TITO_TOKEN_NOT_FOUND 0x0101
#define TITO_ACCOUNT_SLUG_NOT_FOUND 0x0102
#define TITO_EVENT_SLUG_NOT_FOUND 0x0103

#define TITO_TOKEN_ENV_VAR "TITO_TOKEN"
#define TITO_ACCOUNT_SLUG_ENV_VAR "TITO_ACCOUNT_SLUG"
#define TITO_EVENT_SLUG_ENV_VAR "TITO_EVENT_SLUG"

CPPUNIT_TEST_SUITE_REGISTRATION(TestTito);

TestTito::TestTito() : CppUnit::TestCase("tito.h tests")
{
    
}

std::string TestTito::getToken()
{    
    char* token;
    token = getenv(TITO_TOKEN_ENV_VAR);
    
    if (token != NULL) {
        return std::string(token);
    }
    
    std::cerr << "Error TestTito::getToken() : No authentication token in environment variables." << std::endl;
    throw TITO_TOKEN_NOT_FOUND;
}

std::string TestTito::getAccountSlug()
{
    char* token;
    token = getenv(TITO_ACCOUNT_SLUG_ENV_VAR);
    
    if (token != NULL) {
        return std::string(token);
    }
    
    std::cerr << "Error TestTito::getAccountSlug() : No account slug in environment variables." << std::endl;
    throw TITO_ACCOUNT_SLUG_NOT_FOUND;
}

std::string TestTito::getEventSlug()
{
    char* token;
    token = getenv(TITO_EVENT_SLUG_ENV_VAR);
    
    if (token != NULL) {
        return std::string(token);
    }
    
    std::cerr << "Error TestTito::getEventSlug() : No event slug in environment variables." << std::endl;
    throw TITO_EVENT_SLUG_NOT_FOUND;
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
    TitoApi api = TitoApi("wrong token",
                          getAccountSlug(),
                          getEventSlug());
    CPPUNIT_ASSERT(!api.checkAuthToken());
}

