#pragma once
#include <ncurses.h>
#include <string>

#define COLOUR_WHITE_BLACK 0
#define COLOUR_PAIR_WHITE_AND_BLACK COLOR_PAIR(COLOUR_WHITE_BLACK)

#define COLOUR_GREEN_BLACK 1
#define COLOUR_PAIR_GREEN_AND_BLACK COLOR_PAIR(COLOUR_GREEN_BLACK)

#define COLOUR_YELLOW_AND_BLACK 2
#define COLOUR_PAIR_YELLOW_AND_BLACK COLOR_PAIR(COLOUR_YELLOW_AND_BLACK)

#define COLOUR_RED_AND_BLACK 3
#define COLOUR_PAIR_RED_AND_BLACK COLOR_PAIR(COLOUR_RED_AND_BLACK)

// 0xE06323
#define COLOUR_ORANGE_AND_BLACK 4
#define COLOUR_PAIR_ORANGE_AND_BLACK COLOR_PAIR(COLOUR_ORANGE_AND_BLACK)

void print_logo_centre(int /*x_offset*/, int /*y_offset*/, int /*force_small*/);
void print_centre(int /*x_offset*/, int /*y_offset*/, std::string /*str*/);
void setup_colours();
