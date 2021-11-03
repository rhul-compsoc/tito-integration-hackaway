#include <stdlib.h>
#include "testTito.h"
#include "../src/tito.h"

#define TITO_TOKEN_NOT_FOUND 0x5001
#define TITO_TOKEN_ENV_VAR "TITO_TOKEN"

CPPUNIT_TEST_SUITE_REGISTRATION(TestTito);

TestTito::TestTito () : CppUnit::TestCase("tito.h tests")
{

}

QString TestTito::getToken() {    
    char* token;
    token = getenv(TITO_TOKEN_ENV_VAR);
    
    if (token != NULL) {
        return QString(token);
    }
    
    throw TITO_TOKEN_NOT_FOUND;
}

void TestTito::testConstructor()
{
    TitoApi *api = new TitoApi(QString());
    delete api;
}


