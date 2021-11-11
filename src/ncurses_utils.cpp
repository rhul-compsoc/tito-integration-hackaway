#include <iostream>
#include <sstream>
#include <vector>
#include "ncurses_utils.h"

// Generated on https://cloudapps.herokuapp.com/imagetoascii/
// to be of width 55, 29 height
#define LOGO_FIFTY_HEIGHT 29
std::string logo_fifty_five = 
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣶⣿⣿⣿⣿⣿⣿⣿⣶⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣴⣾⣿⡿⠟⠋⠉⠀⠀⠀⠉⠙⠻⢿⣿⣷⣦⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣶⣿⣿⠿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⠿⣿⣿⣶⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣴⣾⣿⡿⠟⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠻⢿⣿⣷⣦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⣠⣴⣿⣿⠿⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⢿⣿⣿⣦⣄⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⣴⣿⣿⠿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠿⣿⣿⣦⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⢀⣾⣿⡟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢻⣿⣷⡀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⣼⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣧⠀⠀⠀⠀\n"
"⠀⠀⠀⢠⣿⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣤⡄⠀⠀⠀⠀⠀⠀⣠⣤⡀⠀⠀⠀⠀⠀⠈⣿⣿⠀⠀⠀⠀\n"
"⠀⠀⠀⢸⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣶⣦⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⠇⠀⠀⠀⠀⠀⢰⣿⣿⠁⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀\n"
"⠀⠀⠀⢸⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⠟⠃⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⡟⠀⠀⠀⠀⠀⢠⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀\n"
"⠀⠀⠀⢸⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⣠⣶⣿⡿⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⡿⠀⠀⠀⠀⠀⢀⣾⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀\n"
"⠀⠀⠀⢸⣿⣿⠀⠀⠀⠀⠀⠀⢠⣾⣿⠿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⠁⠀⠀⠀⠀⠀⣼⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀\n"
"⠀⠀⠀⢸⣿⣿⠀⠀⠀⠀⠀⠀⠘⢿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⠃⠀⠀⠀⠀⠀⣸⣿⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀\n"
"⠀⠀⠀⢸⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠙⠿⣿⣷⣦⡀⠀⠀⠀⠀⠀⠀⢀⣾⣿⡏⠀⠀⠀⠀⠀⢠⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀\n"
"⠀⠀⠀⢸⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⢿⣿⣦⣄⠀⠀⠀⠀⣼⣿⡟⠀⠀⠀⠀⠀⢀⣿⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀\n"
"⠀⠀⠀⢸⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠿⠏⠀⠀⠀⣸⣿⡿⠁⠀⠀⠀⠀⠀⣾⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀\n"
"⠀⠀⠀⠘⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠛⠁⠀⠀⠀⠀⠀⠀⠛⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⢻⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⡟⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠈⢿⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⡿⠁⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠻⣿⣿⣶⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣶⣿⡿⠋⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠙⠻⢿⣿⣷⣦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣴⣾⣿⡿⠟⠋⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⢿⣿⣿⣦⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣴⣿⣿⠿⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠿⣿⣿⣷⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣾⣿⣿⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠻⢿⣿⣷⣦⣄⣀⠀⠀⠀⣀⣠⣴⣾⣿⡿⠟⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⠿⢿⣿⣿⣿⣿⣿⡿⠿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";

// ... to be of width 25, 13 height
#define LOGO_TWENTY_FIVE_HEIGHT 13
std::string logo_twenty_five =
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣤⣶⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⣀⣤⡾⠟⠉⠀⠀⠀⠉⠻⢷⣤⣀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⣠⣴⠾⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⠷⣦⣄⠀⠀⠀\n"
"⠀⠀⣼⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣧⠀⠀\n"
"⠀⢰⣿⠀⠀⠀⠀⠀⠀⣀⠀⠀⠀⠀⣴⠄⠀⠀⣴⠄⠀⠀⣿⡆⠀\n"
"⠀⢸⣿⠀⠀⠀⢀⣴⡾⠋⠀⠀⠀⣰⡟⠀⠀⣸⡟⠀⠀⠀⣿⡇⠀\n"
"⠀⢸⣿⠀⠀⠰⣿⣉⠀⠀⠀⠀⢠⡿⠁⠀⢠⡿⠁⠀⠀⠀⣿⡇⠀\n"
"⠀⢸⣿⠀⠀⠀⠈⠻⢷⣄⠀⢀⣾⠃⠀⢀⣿⠃⠀⠀⠀⠀⣿⡇⠀\n"
"⠀⠸⣿⠀⠀⠀⠀⠀⠀⠉⠀⠸⠋⠀⠀⠸⠃⠀⠀⠀⠀⠀⣿⠇⠀\n"
"⠀⠀⢻⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⡟⠀⠀\n"
"⠀⠀⠀⠉⠻⢷⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⡾⠟⠉⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠉⠛⠷⣦⣄⠀⠀⠀⣠⣴⠾⠛⠁⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠛⠿⠛⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";

int print_logo_left(int x_offset,
                    int y_offset,
                    int force_small)
{
    force_small |= getmaxy(stdscr) >= (LOGO_FIFTY_HEIGHT - 10);
    int lines;
    int width = getmaxx(stdscr);
    if (width >= 55 && !force_small) {
        lines = print_left(x_offset, y_offset, logo_fifty_five);
    } else {
        lines = print_left(x_offset, y_offset, logo_twenty_five);
    }
    return lines; 
}

int print_logo_centre(int x_offset,
                      int y_offset,
                      int force_small)
{
    force_small |= getmaxy(stdscr) >= (LOGO_FIFTY_HEIGHT - 10);
    int lines;
    int width = getmaxx(stdscr);
    if (width >= 55 && !force_small) {
        lines = print_centre(x_offset, y_offset, logo_fifty_five);
    } else {
        lines = print_centre(x_offset, y_offset, logo_twenty_five);
    }
    return lines;
}

int print_left(int x_offset,
               int y_offset,
               std::string str)
{
    int lines = 0;
    std::string line;
    std::stringstream streamData(str);
    while (std::getline(streamData, line, '\n')) {
        mvaddstr(y_offset + lines, x_offset, line.c_str());
        lines++;
    }
    addstr("\n");
    return lines;
}

int print_centre(int x_offset,
                 int y_offset,
                 std::string str)
{
    int width = 0;
    width = getmaxx(stdscr);
    
    int lines = 0;
    std::string line;
    std::stringstream streamData(str);
    while (std::getline(streamData, line, '\n')) {
        int offset = (width - line.size()) / 2;
        mvaddstr(y_offset + lines, x_offset + offset, line.c_str());
        lines++;
    }
    addstr("\n");
    return lines;
}

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
    
    init_pair(COLOUR_ORANGE_AND_BLACK,
              13, /* Find a better orange */
              COLOR_BLACK);
    
    init_pair(COLOUR_BLACK_AND_GREEN,
              COLOR_BLACK,
              COLOR_GREEN);
}
