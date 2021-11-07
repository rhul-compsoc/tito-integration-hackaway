#pragma once
#include <cppunit/extensions/HelperMacros.h>

class TestTito : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(TestTito);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testCheckAuth);
    CPPUNIT_TEST(testWrongToken);
    CPPUNIT_TEST(testGetAttendees);
    CPPUNIT_TEST(testDateParser);
    CPPUNIT_TEST(testIDCache);
    CPPUNIT_TEST_SUITE_END();
public:
    TestTito(void);
    void testConstructor();
    void testCheckAuth();
    void testWrongToken();
    void testGetAttendees();
    void testDateParser(); // Tests the date format
    void testIDCache();
};
