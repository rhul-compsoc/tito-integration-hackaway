#include "ncurses_colours.h"

void setup_colours()
{
    init_pair(COLOUR_WHITE_BLACK,
              COLOR_WHITE,
              COLOR_BLACK);
    
    init_pair(COLOUR_GREEN_BLACK,
              COLOR_GREEN,
              COLOR_BLACK);
    
    init_pair(COLOUR_YELLOW_AND_BLACK,
              COLOR_YELLOW,
              COLOR_BLACK);
    
    init_pair(COLOUR_RED_AND_BLACK,
              COLOR_RED,
              COLOR_BLACK);
}

