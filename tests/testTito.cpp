#include <stdlib.h>
#include "testTito.h"
#include "../src/tito.h"

#define TITO_TOKEN_NOT_FOUND 0x0101
#define TITO_TOKEN_ENV_VAR "TITO_TOKEN"

CPPUNIT_TEST_SUITE_REGISTRATION(TestTito);

TestTito::TestTito() : CppUnit::TestCase("tito.h tests")
{
    
}

char *TestTito::getToken() {    
    char* token;
    token = getenv(TITO_TOKEN_ENV_VAR);
    
    if (token != NULL) {
        return token;
    }
    
    std::cerr << "Error TestTito::getToken() : No authentication token in environment variables." << std::endl;
    throw TITO_TOKEN_NOT_FOUND;
}

void TestTito::testConstructor()
{
    TitoApi api = TitoApi(getToken());
}

void TestTito::testCorrectToken()
{
    TitoApi api = TitoApi(getToken());
    CPPUNIT_ASSERT(api.checkAuthToken());
}

void TestTito::testWrongToken()
{    
    TitoApi api = TitoApi("wrong token");
    CPPUNIT_ASSERT(!api.checkAuthToken());
}

