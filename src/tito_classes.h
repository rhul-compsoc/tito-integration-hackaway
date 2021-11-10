#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

class TitoCheckin {
public:
    TitoCheckin() ;
    TitoCheckin(std::string /*UUID*/,
                bool /*deleted*/,
                struct tm /*checkinTime*/,
                struct tm /*deletedTime*/,
                struct tm /*lastUpdateTime*/);
    std::string getUUID();
    bool isCheckedin();
    bool isDeleted();
    struct tm getCheckInTime();
    struct tm getDeletedTime();
    struct tm getLastUpdateTime();
private:
    std::string UUID;
    bool checkedin, deleted;
    struct tm checkinTime, deletedTime, lastUpdateTime;
};

class TitoTicket {
public:
    TitoTicket(int /*ticketID*/,
               std::string /*ticketSlug*/,
               std::string /*ticketRelease*/);
    TitoTicket();
    int getTicketID();
    std::string getTicketSlug();
    std::string getTicketRelease();
    TitoCheckin getCheckin();
    void setCheckin(TitoCheckin checkin);
private:
    int ticketID;
    std::string ticketSlug,
                ticketRelease;
    TitoCheckin checkin;
};

class TitoAttendee {
public:
    TitoAttendee(std::string /*name*/,
                 std::string /*email*/,
                 std::string /*phoneNumber*/,
                 TitoTicket /*ticket*/);
    TitoAttendee();
    std::string getName();
    std::string getEmail();
    std::string getPhoneNumber();
    TitoTicket getTicket();
    TitoTicket *getTicketRef();
    bool matches(std::string queryIn);
    friend bool operator <(TitoAttendee&, TitoAttendee&);
    /**
     * Shallow object equality (does not look at the tickets), this method is
     * for maintinaing the selection in the attendee selection screen.
     *
     * @param TitoAttendee the other attendee to compare to
     * @return whether the name, email and phone number are equal
     */
    bool operator==(TitoAttendee);
private:
    /**
     * Strips a string for searching, this removes all non alpha-numerical
     * characters and converts to lower case.
     *
     * @param std::string string to strip
     * @return the stripped string
     */
    std::string stripQueryStr(std::string str);
    // Don't tell Dave Cohen of my primitive obsession
    // Alexa git-blame-someone-else
    std::string name, email, phoneNumber;
    TitoTicket ticket;
};
