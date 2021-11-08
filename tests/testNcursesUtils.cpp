#include "testNcursesUtils.h"
#include "../src/ncurses_utils.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestNcursesUtils);

TestNcursesUtils::TestNcursesUtils() : CppUnit::TestCase("ncurses_utils.h tests")
{    
    initscr();
    
    // Get all input in real time
    keypad(stdscr, TRUE);
    cbreak();
    noecho();
    
    // Init colours
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal is shit and does not support colour.\n");
        exit(1);
    }
    
    start_color();
}

TestNcursesUtils::~TestNcursesUtils()
{
    endwin();    
}

void TestNcursesUtils::testPrintCentre() {
    print_centre(0, 0, "Test writing to the centre");
}

void TestNcursesUtils::testPrintLogo() {
    print_logo_centre(0, 0, 0);
    print_logo_centre(0, 0, 1);
}

void TestNcursesUtils::testSetupColours() {
    setup_colours();
}
