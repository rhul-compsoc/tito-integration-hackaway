#pragma once

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
    TitoTicket(int ticketID,
               std::string ticketSlug,
               std::string ticketRelease,
               TitoCheckin checkin)
    {
        this->ticketID = ticketID;
        this->ticketSlug = ticketSlug;
        this->ticketRelease = ticketRelease;
        this->checkin = checkin;
    }
    int getTicketID() { return this->ticketID; }
    std::string getTicketSlug() { return this->ticketSlug; }
    std::string getTicketRelease() { return this->ticketRelease; }
    TitoCheckin getCheckin() { return this->checkin; }
private:
    int ticketID;
    std::string ticketSlug,
                ticketRelease;
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
