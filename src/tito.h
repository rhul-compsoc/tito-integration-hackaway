#pragma once
#include <string>
#include <list>
#include <time.h>

#define TITO_NET_ERROR 0x0001;
#define TITO_AUTH_ERROR 0x0002;

/**
 * The internal networking is purely GET requests over HTTPS, as such I chose to
 * use curl to make these requests because curl is well known to simply just 
 * work.
 */
class TitoApi {
public:
    /**
     * Init the Tito API with the token.
     * 
     * @param token The Tito authentication token. is copied in the constructor.
     * The memory of the input string should be handled by the caller.
     */ 
    TitoApi(std::string /*token*/);
    
    /**
     * @return whether the authentication token is correct.
     * @throws TITO_NET_ERROR if there was a network error whilst connecting to
     * the Tito system.
     */
    bool checkAuthToken();
private:
    /**
     * Makes a GET request to an endpoint with the tito API headers and returns 
     * the data.
     * 
     * @param url The URL to GET from.
     * @throws TITO_NET_ERROR When making the curl request fails.
     */ 
    std::string getRequest(std::string /* url */);
    std::string token;
};

class TitoCheckin {
public:
    TitoCheckin() {};
    TitoCheckin(bool checkedin,
                bool deleted,
                struct tm checkinTime,
                struct tm deletedTime,
                struct tm lastUpdateTime)
    {
        this->checkedin = checkedin;
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
    TitoTicket(std::string ticketName,
               std::string ticketRelease,
               int quanitity,
               TitoCheckin checkin)
    {
        this->ticketName = ticketName;
        this->ticketRelease = ticketRelease;
        this->quanitity = quanitity;
        this->checkin;
    }
    std::string getTicketName() { return this->ticketRelease; }
    std::string getTicketRelease() { return this->ticketName; }
    int getQuantity() { return this-> quanitity; }
    TitoCheckin getCheckin() { return this->checkin; }
private:
    std::string ticketName, ticketRelease;
    int quanitity;
    TitoCheckin checkin;
};

class TitoAttendee {
public:
    TitoAttendee(std::string name,
                 std::string email,
                 std::string phoneNumber,
                 std::list<TitoAttendee> tickets)
    {
        this->name = name;
        this->email = email;
        this->phoneNumber = phoneNumber;
        this->tickets = tickets;
    }
    std::string getName() { return this->name; }
    std::string getEmail() { return this->email; }
    std::string getPhoneNumber() { return this->phoneNumber; }
    std::list<TitoAttendee> getTickets() { return this->tickets; }
private:
    // Don't tell Dave Cohen of my primitive obsession
    // Alexa git-blame-someone-else
    std::string name, email, phoneNumber;
    std::list<TitoAttendee> tickets;
};
