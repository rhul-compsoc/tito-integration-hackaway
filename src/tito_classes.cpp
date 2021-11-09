#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "tito_classes.h"

TitoCheckin::TitoCheckin() {};
TitoCheckin::TitoCheckin(bool deleted,
                         struct tm checkinTime,
                         struct tm deletedTime,
                         struct tm lastUpdateTime)
{
    this->deleted = deleted;
    this->checkinTime = checkinTime;
    this->deletedTime = deletedTime;
    this->lastUpdateTime = lastUpdateTime;
}
bool TitoCheckin::isCheckedin() { return this->checkedin; }
bool TitoCheckin::isDeleted() { return this->deleted; }
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
    this->checkedIn = false;
}
TitoTicket::TitoTicket() {} // Do not use this constructor please
int TitoTicket::getTicketID() { return this->ticketID; }
std::string TitoTicket::getTicketSlug() { return this->ticketSlug; }
std::string TitoTicket::getTicketRelease() { return this->ticketRelease; }
bool TitoTicket::isCheckedin() { return this->checkedIn; }
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
std::string TitoAttendee::getEmail() { return this->email; }
std::string TitoAttendee::getPhoneNumber() { return this->phoneNumber; }
TitoTicket TitoAttendee::getTicket() { return this->ticket; }
bool TitoAttendee::matches(std::string queryIn)
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
bool TitoAttendee::operator<(TitoAttendee /*other*/) {
    return this->ticket.isCheckedin();
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
