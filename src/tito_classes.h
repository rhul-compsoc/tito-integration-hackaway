#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

class TitoCheckin {
public:
    TitoCheckin() ;
    TitoCheckin(
                bool /*deleted*/,
                struct tm /*checkinTime*/,
                struct tm /*deletedTime*/,
                struct tm /*lastUpdateTime*/);
    bool isCheckedin();
    bool isDeleted();
    struct tm getCheckInTime();
    struct tm getDeletedTime();
    struct tm getLastUpdateTime();
private:
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
    bool isCheckedin();
    TitoCheckin getCheckin();
    void setCheckin(TitoCheckin checkin);
private:
    int ticketID;
    std::string ticketSlug,
                ticketRelease;
    bool checkedIn;
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
    bool matches(std::string queryIn);
    /**
     * Used for sorting to put checkedin members at the top of the list.
     *
     * @param TitoAttendee other, not used
     * @return bool a < b is true if a is checkedin
     */
    bool operator<(TitoAttendee /*other*/);
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
