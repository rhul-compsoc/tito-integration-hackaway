#pragma once
#include <cppunit/extensions/HelperMacros.h>

class TestIdCardGen : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(TestIdCardGen);
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();
public:
    TestIdCardGen(void);
    void test();
};

