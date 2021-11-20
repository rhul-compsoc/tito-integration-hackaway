#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "tito_classes.h"

TitoCheckin::TitoCheckin()
{
    this->deleted = false;
    this->checkedin = false;
};
TitoCheckin::TitoCheckin(std::string UUID,
                         bool deleted,
                         struct tm checkinTime,
                         struct tm deletedTime,
                         struct tm lastUpdateTime)
{
    this->UUID = UUID;
    this->checkedin = !deleted;
    this->deleted = deleted;
    this->checkinTime = checkinTime;
    this->deletedTime = deletedTime;
    this->lastUpdateTime = lastUpdateTime;
}
std::string TitoCheckin::getUUID() { return this->UUID; }
bool TitoCheckin::isCheckedin() { return this->checkedin; }
bool TitoCheckin::isDeleted() { return this->deleted && this->deletedTime.tm_year != 0; }
struct tm TitoCheckin::getCheckInTime() { return this->checkinTime; }
struct tm TitoCheckin::getDeletedTime() { return this->deletedTime; }
struct tm TitoCheckin::getLastUpdateTime() { return this->lastUpdateTime; }

TitoTicket::TitoTicket(int ticketID,
                       std::string ticketSlug,
                       std::string ticketRelease)
{
    this->ticketID = ticketID;
    this->ticketSlug = ticketSlug;
    this->ticketRelease = ticketRelease;
}
TitoTicket::TitoTicket() {} // Do not use this constructor please
int TitoTicket::getTicketID() { return this->ticketID; }
std::string TitoTicket::getTicketSlug() { return this->ticketSlug; }
std::string TitoTicket::getTicketRelease() { return this->ticketRelease; }
TitoCheckin TitoTicket::getCheckin() { return this->checkin; }
void TitoTicket::setCheckin(TitoCheckin checkin) { this->checkin = checkin; }
TitoAttendee::TitoAttendee(std::string name,
                           std::string email,
                           std::string phoneNumber,
                           TitoTicket ticket)
{
    this->name = name;
    this->email = email;
    this->phoneNumber = phoneNumber;
    this->ticket = ticket;
}
TitoAttendee::TitoAttendee() {}; // Do not use this constructor please
std::string TitoAttendee::getName() { return this->name; }
std::string TitoAttendee::getForename(){
    size_t index = this->name.find(" ");
    if (index == std::string::npos) return this->getName();
    return this->name.substr(0, index);
}
std::string TitoAttendee::getSurname()
{
    size_t index = this->name.find(" ");
    if (index == std::string::npos) return "";
    return this->name.substr(index + 1, this->name.size() - 1);
}
std::string TitoAttendee::getEmail() { return this->email; }
std::string TitoAttendee::getPhoneNumber() { return this->phoneNumber; }
TitoTicket TitoAttendee::getTicket() { return this->ticket; }
TitoTicket *TitoAttendee::getTicketRef() { return &this->ticket; }

bool TitoAttendee::matches(std::string queryIn)
{
    bool ret = true;
    std::string query;
    std::stringstream streamData(queryIn);
    while (std::getline(streamData, query, ' ')) {
        query = stripQueryStr(query);
        bool subPattern = false;
        
        subPattern |= stripQueryStr(this->name).find(query) != std::string::npos;
        subPattern |= stripQueryStr(this->email).find(query) != std::string::npos;
        subPattern |= stripQueryStr(this->phoneNumber).find(query) != std::string::npos;
        subPattern |= stripQueryStr(this->ticket.getTicketRelease()).find(query) 
            != std::string::npos;
        subPattern |= query == stripQueryStr(this->ticket.getTicketSlug());
            
        ret &= subPattern;
    }
    return ret;
}

bool operator <(TitoAttendee &b, TitoAttendee &a)
{
    // Checkedin
    if (a.getTicket().getCheckin().isCheckedin()
        && !b.getTicket().getCheckin().isCheckedin()) {
        return true;
    }

    if (a.getName() < b.getName()) {
        return true;
    }

    if (a.getEmail() < b.getEmail()) {
        return true;
    }

    if (a.getPhoneNumber() < b.getPhoneNumber()) {
        return true;
    }

    if (a.getTicket().getTicketRelease() < b.getTicket().getTicketRelease()) {
        return true;
    }

    return a.getTicket().getTicketID() < b.getTicket().getTicketID();
}

bool TitoAttendee::operator==(TitoAttendee other)
{
    return this->name == other.name
        && this->email == other.email
        && this->phoneNumber == other.phoneNumber;
}

std::string TitoAttendee::stripQueryStr(std::string str)
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
