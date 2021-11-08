#pragma once
#include <cppunit/extensions/HelperMacros.h>

class TestNcursesUtils : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(TestNcursesUtils);
    CPPUNIT_TEST(testPrintLogo);
    CPPUNIT_TEST(testPrintCentre);
    CPPUNIT_TEST(testSetupColours);
    CPPUNIT_TEST_SUITE_END();
public:
    TestNcursesUtils(void);
    ~TestNcursesUtils();
    void testPrintLogo();
    void testPrintCentre();
    void testSetupColours();
};
