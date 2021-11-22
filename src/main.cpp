#include <stdlib.h>
#include <ncurses.h>
#include <string>
#include "ncurses_utils.h"
#include "select_attendee.h"
#include "error_screen.h"
#include "tito.h"
#include "id_card_gen.h"

static void warn_exit()
{
    attron(COLOUR_PAIR_YELLOW_AND_BLACK);
    print_centre(0, getmaxy(stdscr) - 2, "Press any key to exit...");
    refresh();
    attroff(COLOUR_PAIR_YELLOW_AND_BLACK);
    
    getch();
    
    endwin();
}

static void print_logo(int *y)
{
    attron(COLOUR_PAIR_ORANGE_AND_BLACK);
    *y += print_logo_centre(0, *y, 0);
    attroff(COLOUR_PAIR_ORANGE_AND_BLACK);
}

static bool updateAttendees(std::list<TitoAttendee> &list,
                            TitoApi &api)
{
    std::list<TitoAttendee> attendees;
    bool flag = true;
    while (flag) {
        try {
            clear();
            print_centre(0, getmaxy(stdscr) / 2, "Updating attendee cache...");
            refresh();
            attendees = api.getAttendees();
            flag = false;
        } catch (int e) {
            struct ErrorAction act;
            act = showErrorMessage("An error occurred updating the attendee cache",
                                e);
            
            if (act.action == ERROR_ACTION_IGNORE) {
                return false;
            }
        }
    }
    
    // Clear list
    while (list.size() > 0) {
        list.pop_back();
    }
    
    // Add the new attendees to the list
    for (TitoAttendee attendee : attendees) {
        list.push_back(attendee);
    }
    
    return true;
}

static void viewAttendees(std::list<TitoAttendee> &list,
                          TitoApi &api)
{
    updateAttendees(list, api);
    struct AttendeeSelection selection = select_attendee(api,
                                                         list,
                                                         "Showing all attendees.",
                                                         true);
}

static void printAttendeeIdCard(std::list<TitoAttendee> &list,
                                TitoApi &api)
{
    updateAttendees(list, api);
    struct AttendeeSelection selection = 
        select_attendee(api,
                        list,
                        "Select an attendee to print an id card for.",
                        true);
    
    if (selection.attendeeSelected) {
        std::string name = selection.attendee.getName();
        clear();
        int y = getmaxy(stdscr) / 2;
        y += print_centre(0,
                          y,
                          "Printing id card for " + name + "...");
        
        if (!selection.attendee.getTicket().getCheckin().isCheckedin()) {
            print_centre(0,
                         y,
                         "Do note: " + name + " is not checked in yet...");
        }
        refresh();
        
        IdCard idCard = IdCard(selection.attendee);
        idCard.print();
    }
}

static void checkinoutAttendee(std::list<TitoAttendee> &list,
                               TitoApi &api)
{
    updateAttendees(list, api);
    struct AttendeeSelection selection = 
        select_attendee(api,
                        list,
                        "Select an attendee to check in/out.",
                        true);
    
    bool checkedin = selection.attendee.getTicket().getCheckin().isCheckedin();
    std::string name = selection.attendee.getName();
    if (selection.attendeeSelected) {
        bool flag = true,
             success = false,
             print_id = !api.hasIDBeenGiven(selection.attendee);

        print_id &= !checkedin;
        while (flag) {
            try {
                clear();
                if (!checkedin) {
                    print_centre(0,
                                 getmaxy(stdscr) / 2,
                                 "Checking " + name + " in...");
                    refresh();
                    flag = api.checkinAttendee(selection.attendee);
                    success = true;
                } else {                    
                    print_centre(0,
                                 getmaxy(stdscr) / 2,
                                 "Checking " + name + " out...");
                    refresh();
                    flag = api.checkoutAttendee(selection.attendee);
                }
            } catch (int e) {                
                struct ErrorAction act;
                act = showErrorMessage("An error occurred during checking "
                                       + name + "in/out.",
                                       e);
                
                if (act.action == ERROR_ACTION_IGNORE) {
                    flag = false;
                }
            }
        }

        // if the operation was cancelled printing should not occur
        while (print_id && success) {
            try {
                clear();
                print_centre(0,
                             getmaxy(stdscr) / 2,
                             "Printing ticket for " + name + ".");
                refresh();

                IdCard idCard = IdCard(selection.attendee);
                idCard.print();
                print_id = false;
            } catch (int e) {
                struct ErrorAction act;
                act = showErrorMessage("An error occurred during printing.",
                                       e);

                if (act.action == ERROR_ACTION_IGNORE) {
                    print_id = false;
                }
            }
        }
    }
}

int main(int argc, char **argv)
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
    setup_colours();
    
    // Setup cimg some more
    cimg::exception_mode(0);
    
    try {
        loadFont();
    } catch (CImgException e) {
        std::string msg = "Error loading font: " 
                        + std::string(e.what());
        print_centre(0, getmaxy(stdscr) / 2, msg);
        warn_exit();
    }
    
    // Initialise the attendee cache and the api, disallow for failure
    int y;
    TitoApi api;
    std::list<TitoAttendee> attendees;
    bool flag = true;
    while (flag) {
        y = 2;
        print_logo(&y);

        attron(COLOUR_PAIR_GREEN_AND_BLACK);
        y += 3;
        y += print_centre(0, y, "Loading Royal Hackaway TiTo integration...");
        attroff(COLOUR_PAIR_GREEN_AND_BLACK);
        refresh();

        try {
            api = TitoApi(getToken(), getAccountSlug(), getEventSlug(), getCheckinSlug());
            attendees = api.getAttendees();
            flag = false;
        } catch (int e) {
            struct ErrorAction act;
            act = showErrorMessage("An error occurred during initialisation",
                                   e);

            if (act.action == ERROR_ACTION_IGNORE) {
                print_centre(0, getmaxy(stdscr) / 2,
                             "Initialisation failed, see error log.");
                warn_exit();
                return EXIT_FAILURE;
            }
        }
    }
    
    y += print_centre(0, y, "Loaded ID cache, attendees and, checkins.");
    
    // Splash screen    
    bool running = true;
    while (running) {
        clear();        
        y = 2;
        print_logo(&y);
        y += 3;
        
        attron(COLOUR_PAIR_GREEN_AND_BLACK);
        y += print_centre(0, y, "Choose an Operation.");
        attroff(COLOUR_PAIR_GREEN_AND_BLACK);
        y += 1;
        
        // Print operations
        y += print_centre(0, y, "Press <C> to checkin/out an attendee.");
        y += print_centre(0, y, "Press <V> to view all attendees.");
        y += print_centre(0, y, "Press <P> to print an id card.");
        y += print_centre(0, y, "Press <ESCAPE> to exit.");
        
        int c = getch();
        try {
            switch (c) {
                case 'c':
                case 'C':
                    checkinoutAttendee(attendees, api);
                    break;
                case 'p':
                case 'P':
                    printAttendeeIdCard(attendees, api);
                    break;
                case 'v':
                case 'V':
                    viewAttendees(attendees, api);
                    break;
                // Exit
                case KEY_EXIT:
                case ESCAPE:
                    running = 0;
                    break;
            }
        } catch (int e) {
            clear();
            attron(COLOUR_PAIR_RED_AND_BLACK);
            y = getmaxy(stdscr) / 2;
            y -= 3;
            if (y < 0) y = 0;
            
            y += print_centre(0,
                              y,
                              "An unexpected and, uncaught error with code "
                              + std::to_string(e)
                              + " has occurred.");
            y += print_centre(0, y, "Error information:");
            y += print_centre(0, y, getTitoErrorMessage(e));
            y += 2;
            y += print_centre(0, y, "Press any key to return to the main menu.");
            attroff(COLOUR_PAIR_RED_AND_BLACK);
            refresh();            
            getch();
        } catch (CImgException e) {            
            clear();
            attron(COLOUR_PAIR_RED_AND_BLACK);
            y = getmaxy(stdscr) / 2;
            y -= 3;
            if (y < 0) y = 0;
            
            y += print_centre(0,
                              y,
                              "An unexpected and, uncaught error with cimg.eu "
                              "has occurred.");
            y += print_centre(0, y, "Error information:");
            y += print_centre(0, y, std::string(e.what()));
            y += 2;
            y += print_centre(0, y, "Press any key to return to the main menu.");
            attroff(COLOUR_PAIR_RED_AND_BLACK);
            refresh();            
            getch();
        }
        
        refresh();
    }

    endwin();
    return EXIT_SUCCESS;
}
