#pragma once
#include <qt/QtCore/qstring.h>
#include <cppunit/extensions/HelperMacros.h>

class TestTito : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(TestTito);
        CPPUNIT_TEST(testConstructor);
        CPPUNIT_TEST_SUITE_END();
    public:
        TestTito(void);
        void testConstructor();
    private:
        QString getToken();
};
