#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

class TitoCheckin {
public:
    TitoCheckin() {};
    TitoCheckin(
                bool deleted,
                struct tm checkinTime,
                struct tm deletedTime,
                struct tm lastUpdateTime)
    {
        this->deleted = deleted;
        this->checkinTime = checkinTime;
        this->deletedTime = deletedTime;
        this->lastUpdateTime = lastUpdateTime;
    }
    bool isCheckedin() { return this->checkedin; }
    bool isDeleted() { return this->deleted; }
    struct tm getCheckInTime() { return this->checkinTime; }
    struct tm getDeletedTime() { return this->deletedTime; }
    struct tm getLastUpdateTime() { return this->lastUpdateTime; }
private:
    bool checkedin, deleted;
    struct tm checkinTime, deletedTime, lastUpdateTime;
};

class TitoTicket {
public:
    TitoTicket(int ticketID,
               std::string ticketSlug,
               std::string ticketRelease)
    {
        this->ticketID = ticketID;
        this->ticketSlug = ticketSlug;
        this->ticketRelease = ticketRelease;
        this->checkedIn = false;
    }
    TitoTicket() {} // Do not use this constructor please
    int getTicketID() { return this->ticketID; }
    std::string getTicketSlug() { return this->ticketSlug; }
    std::string getTicketRelease() { return this->ticketRelease; }
    bool isCheckedin() { return this->checkedIn; }
    TitoCheckin getCheckin() { return this->checkin; }
    void setCheckin(TitoCheckin checkin) { this->checkin = checkin; }
private:
    int ticketID;
    std::string ticketSlug,
                ticketRelease;
    bool checkedIn;
    TitoCheckin checkin;
};

class TitoAttendee {
public:
    TitoAttendee(std::string name,
                 std::string email,
                 std::string phoneNumber,
                 TitoTicket ticket)
    {
        this->name = name;
        this->email = email;
        this->phoneNumber = phoneNumber;
        this->ticket = ticket;
    }
    TitoAttendee() {}; // Do not use this constructor please
    std::string getName() { return this->name; }
    std::string getEmail() { return this->email; }
    std::string getPhoneNumber() { return this->phoneNumber; }
    TitoTicket getTicket() { return this->ticket; }
    bool matches(std::string queryIn)
    {
        bool ret = false;
        std::string query;
        std::stringstream streamData(queryIn);
        while (std::getline(streamData, query, ' ')) {
            query = stripQueryStr(query);
            ret |= stripQueryStr(this->name).find(query) != std::string::npos;
            ret |= stripQueryStr(this->email).find(query) != std::string::npos;
            ret |= stripQueryStr(this->phoneNumber).find(query) != std::string::npos;
            ret |= stripQueryStr(this->ticket.getTicketRelease()).find(query) != std::string::npos;
        }
        return ret || queryIn == "";
    }
private:
    /**
     * Strips a string for searching, this removes all non alpha-numerical
     * characters and converts to lower case.
     *
     * @param std::string string to strip
     * @return the stripped string
     */
    std::string stripQueryStr(std::string str)
    {
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        std::string ret = "";
        for (char c : str) {
            if ((c >= '0' && c <'9') || (c >= 'a' || c <= 'z')) {
                ret += c;
            }
        }
        return ret;
    }
    // Don't tell Dave Cohen of my primitive obsession
    // Alexa git-blame-someone-else
    std::string name, email, phoneNumber;
    TitoTicket ticket;
};
