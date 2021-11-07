#pragma once
#include <cppunit/extensions/HelperMacros.h>

class TestTito : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(TestTito);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testCorrectToken);
    CPPUNIT_TEST(testWrongToken);
    CPPUNIT_TEST(testDateParser);
    CPPUNIT_TEST_SUITE_END();
public:
    TestTito(void);
    void testConstructor();
    void testCorrectToken();
    void testWrongToken();
    void testDateParser(); // Tests the date format
};
