#include <curl/curl.h>
#include <iostream>
#include <ios>
#include <sstream>
#include <vector>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "tito.h"
#include "json.hpp"
#include "id_card_gen.h"

#define TITO_AUTH_JSON_KEY "authenticated"
#define TITO_ACCESS_TOKEN_JSON_KEY "access_token"

TitoApi::TitoApi(std::string token_str,
                 std::string accountSlug,
                 std::string eventSlug,
                 std::string checkinSlug)
{
    this->token = std::string(token_str);
    this->accountSlug = std::string(accountSlug);
    this->eventSlug = std::string(eventSlug);
    this->checkinSlug = std::string(checkinSlug);
    this->readIDCache();
    if (!this->checkAuthToken()) { // This gets the checkin slug
        throw TITO_ACCESS_TOKEN_ERROR;
    }
}

bool TitoApi::hasIDBeenGiven(TitoAttendee attendee)
{
    std::string name = attendee.getName();
    for (std::string idGiven : this->idsGiven) {
        if (name == idGiven) {
            return true;
        }
    }
    return false;
}

void TitoApi::addIDToCache(TitoAttendee attendee)
{
    std::string name = attendee.getName();
    this->idsGiven.push_back(name);

    FILE *f = fopen(ID_CACHE_FILE, "a");
    if (f == NULL) {
        std::cerr << "Error TitoApi::addIDToCache() : ID cache file cannot be "
                  "made."
                  << std::endl;
        throw TITO_ID_CACHE_ERROR;
    }
    fprintf(f, "%s\n", name.c_str());
    fclose(f);
}

void TitoApi::readIDCache()
{
    this->idsGiven = std::list<std::string>();
    if (access(ID_CACHE_FILE, F_OK) == -1) {
#ifdef DEBUG
        std::cerr << "Debug TitoApi::readIDCache() : Made cache file "
                  << std::endl;
#endif
        std::cerr << "Error TitoApi::readIDCache() : ID cache file does note "
                  "exist."
                  << std::endl;
        FILE *f = fopen(ID_CACHE_FILE, "w");
        if (f == NULL) {
            std::cerr << "Error TitoApi::readIDCache() : ID cache file cannot "
                      "be made."
                      << std::endl;
            throw TITO_ID_CACHE_ERROR;
        }
        fclose(f);
    } else {
#ifdef DEBUG
        std::cerr << "Debug TitoApi::readIDCache() : Read cache file "
                  << std::endl;
#endif

        std::fstream file;
        file.open(ID_CACHE_FILE, std::ios::in);
        if (!file.is_open()) {
            std::cerr << "Error TitoApi::readIDCache() : Cannot read the id "
                      "cache file."
                      << std::endl;
            throw TITO_ID_CACHE_ERROR;
        }

        std::string line;
        while (std::getline(file, line)) {
            this->idsGiven.push_back(line);
#ifdef DEBUG
            std::cerr << "Debug TitoApi::readIDCache() : Id for "
                      << line
                      << std::endl;
#endif
        }

        file.close();
    }
}

struct CurlResponse {
    char *ptr;
    size_t len;
};

static size_t write_callback(char *ptr_in,
                             size_t size,
                             size_t nmemb,
                             void *userdata)
{
    struct CurlResponse *response = (struct CurlResponse *) userdata;
    if (response->ptr == NULL) {
        response->ptr = (char *) malloc((size * nmemb) + 1);
        if (response->ptr == NULL) {
            std::cerr << "Error TitoApi::write_callback() : malloc failed."
                      << std::endl;
            return 0;
        }

        response->ptr[size * nmemb] = '\0';
        response->len = size * nmemb;
        memcpy(response->ptr, ptr_in, size * nmemb);
    } else {
        // We have to sellotape the chunks together
        response->ptr = (char *) realloc(response->ptr,
                                         response->len + (size * nmemb) + 1);
        if (response->ptr == NULL) {
            std::cerr << "Error TitoApi::write_callback() : realloc failed."
                      << std::endl;
            return 0;
        }

        response->ptr[response->len + (size * nmemb)] = '\0';
        memcpy(response->ptr + response->len, ptr_in, size * nmemb);
        response->len += size * nmemb;
    }

#ifdef DEBUG
    std::cerr << "Debug TitoApi::write_callback() : " << response->ptr << std::endl;
#endif

    return size * nmemb;
}

static void freeCurlResponse(struct CurlResponse *resp)
{
    if (resp->ptr) {
        free(resp->ptr);
        resp->ptr = NULL;
    }
}

std::string TitoApi::sendRequest(std::string url,
                                 std::string data,
                                 std::string request)
{
#ifdef DEBUG
    std::cerr << "Debug TitoApi::sendRequest() : The URL is " << url
              << " (" << request << ")"
              << std::endl;
#endif

    CURL *curl = curl_easy_init();
    if(curl) {
        CURLcode res;

        struct CurlResponse response = {NULL, 0};

        struct curl_slist *list = NULL;
        list = curl_slist_append(list, "Accept: application/json");
        list = curl_slist_append(list, "Content-Type: application/json");

        // Set timeouts
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 2L);

        // Set url, user-agent and, headers
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USE_SSL, 1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "rhul-tito-integration");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, request.c_str());

        // Set response write
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &response);

        res = curl_easy_perform(curl);

        bool getSuccess = res == CURLE_OK && response.ptr != NULL;

        std::string resp;
        if (getSuccess) {
            resp = std::string(response.ptr);
            if (resp == "5xx json") {
                getSuccess = false;
            }
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(list);
        freeCurlResponse(&response);

        if (!getSuccess) {
            if (res != CURLE_OK) {
                std::cerr << "Error TitoApi::sendRequest() : curl perform "
                          "failed after send."
                          << std::endl;
                fprintf(stderr,
                        "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));
            } else {
                std::cerr << "Error TitoApi::sendRequest() : no response was read."
                          << std::endl;
            }
            throw TITO_NET_ERROR;
        }

        return resp;
    } else {
        std::cerr << "Error TitoApi::sendRequest() : curl init failed."
                  << std::endl;
        throw TITO_NET_ERROR;
    }
}

std::string TitoApi::getRequest(std::string url)
{
#ifdef DEBUG
    std::cerr << "Debug TitoApi::getRequest() : The URL is " << url<< std::endl;
#endif

    CURL *curl = curl_easy_init();
    if(curl) {
        CURLcode res;

        struct CurlResponse response = {NULL, 0};

        std::string header = "Authorization: Token token=" + this->token;

        struct curl_slist *list = NULL;
        list = curl_slist_append(list, header.c_str());
        list = curl_slist_append(list, "Accept: application/json");

        // Set timeouts
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 2L);

        // Set url, user-agent and, headers
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USE_SSL, 1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "rhul-tito-integration");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

        // Set response write
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &response);

        res = curl_easy_perform(curl);

        bool getSuccess = res == CURLE_OK && response.ptr != NULL;

        std::string resp;
        if (getSuccess) {
            resp = std::string(response.ptr);
            if (resp == "5xx json") {
                getSuccess = false;
            }
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(list);
        freeCurlResponse(&response);

        if (!getSuccess) {
            if (res != CURLE_OK) {
                std::cerr << "Error TitoApi::getRequest() : curl perform failed."
                          << std::endl;
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));
            } else {
                std::cerr << "Error TitoApi::getRequest() : no response was read."
                          << std::endl;
            }
            throw TITO_NET_ERROR;
        }

        return resp;
    } else {
        std::cerr << "Error TitoApi::getRequest() : curl init failed."
                  << std::endl;
        throw TITO_NET_ERROR;
    }
}

std::list<TitoAttendee> TitoApi::getAttendees()
{
    return this->getAttendeesRecursive(1);
}

std::list<TitoAttendee> TitoApi::getAttendeesRecursive(int page)
{
    std::string url = "https://api.tito.io/v3/" + this->accountSlug
                      + "/" + this->eventSlug + "/tickets?page="
                      + std::to_string(page);
    std::string resp = getRequest(url);
    nlohmann::json rootJson = nlohmann::json::parse(resp);

    // Check for errors
    if (rootJson.contains("errors")) {
        std::cerr << "Error : TitoApi::getAttendees() : (probably) authentication "
                  << "or errors " << std::endl;
        throw TITO_AUTH_ERROR; // Probably an auth error lmao
    }
    if (!rootJson.contains("tickets")) {
        std::cerr << "Error : TitoApi::getAttendees() : internal errors, maybe "
                  << "the slugs are wrong "
                  << std::endl;
        throw TITO_INTERNAL_ERROR;
    }

    // Parse the registrations
    std::list<TitoAttendee *> tmp;
    nlohmann::json registrationJson = rootJson.at("tickets");
    for (nlohmann::json::iterator it = registrationJson.begin()
                                       ; it != registrationJson.end(); ++it) {
        nlohmann::json attendee = it.value();
        std::string name;
        attendee.at("name").get_to(name);

        std::string email, phoneNumber;
        if (!attendee.at("email").is_null()) {
            attendee.at("email").get_to(email);
        }

        if (!attendee.at("phone_number").is_null()) {
            attendee.at("phone_number").get_to(phoneNumber);
        }

        int ticketID;
        attendee.at("id").get_to(ticketID);
        std::string ticketSlug;
        attendee.at("slug").get_to(ticketSlug);
        std::string ticketRelease;
        attendee.at("release_title").get_to(ticketRelease);

        // The API is poor, so I have to ask for the check ins later
        TitoTicket ticket(ticketID, ticketSlug, ticketRelease);

#ifdef DEBUG
        std::cerr << "Debug TitoApi::getAttendees() : Found an attendee with name "
                  << name << std::endl;
#endif
        tmp.push_back(new TitoAttendee(name, email, phoneNumber, ticket));
    }

    // Get the checkins
    url = "https://checkin.tito.io/checkin_lists/"
          + this->checkinSlug + "/checkins";
    resp = getRequest(url);

    // Parse checkins
    rootJson = nlohmann::json::parse(resp);
    if (rootJson.contains("message")) {
        std::cerr << "Error TitoApi::getAttendees() : Unable to find checkins "
                  "on the TiTo server"
                  << std::endl;
        throw TITO_CHECKINS_NOT_FOUND_ERROR;
    }

    for (nlohmann::json::iterator it = rootJson.begin();
            it != rootJson.end(); ++it) {
        nlohmann::json checkinJson = it.value();

        int ticketID;
        checkinJson.at("ticket_id").get_to(ticketID);

        std::string createdAt, updatedAt, deletedAt;
        checkinJson.at("created_at").get_to(createdAt);
        checkinJson.at("updated_at").get_to(updatedAt);
        bool deleted = !checkinJson.at("deleted_at").is_null();

        std::string UUID;
        checkinJson.at("uuid").get_to(UUID);

        // Turn these strings to dates (struct tm)
        struct tm createdTime, updateTime, deletedTime;

        if (deleted) {
            checkinJson.at("deleted_at").get_to(deletedAt);
            strptime(deletedAt.c_str(), TITO_DATE_FORMAT, &deletedTime);
        }

        strptime(createdAt.c_str(), TITO_DATE_FORMAT, &createdTime);
        strptime(updatedAt.c_str(), TITO_DATE_FORMAT, &updateTime);

        TitoCheckin checkin(UUID, deleted, createdTime, deletedTime, updateTime);

        // Find the attendee to give the checkin
        bool flag = false;
        for (TitoAttendee *attendee : tmp) {
            TitoTicket *ticket = attendee->getTicketRef();
            if (ticket->getTicketID() == ticketID) {
                ticket->setCheckin(checkin);
                flag = true;
#ifdef DEBUG
                std::cerr << "Debug TitoApi:getAttendees() : Found ticket for "
                          "checkin "
                          << ticketID
                          << " (checked in at " << asctime(&createdTime)
                          << ") " << attendee->getName()
                          << std::endl;
#endif
                break;
            }
        }

        if (!flag) {
            std::cerr << "Error TitoApi:getAttendees() : Unable to find ticket "
                      "for checkin "
                      << ticketID
                      << " (checked in at " << asctime(&createdTime)
                      << ")"
                      << std::endl;
        }
    }

    std::list<TitoAttendee> output;
    for (TitoAttendee *attendee : tmp) {
        output.push_back(TitoAttendee(*attendee));
        delete attendee;
    }

    // If this is not the last page ask for more pages
    bool more = rootJson.contains("total_ticket_pages");
    if (more) {
        int pages;
        rootJson.at("total_ticket_pages").get_to(pages);

        more = page < pages;
    }
    
    if (more) {        
        std::list<TitoAttendee> more = this->getAttendeesRecursive(page + 1);
        for (TitoAttendee attendee : more) {
            output.push_back(attendee);
        }
    }

    return output;
}

bool TitoApi::checkinAttendee(TitoAttendee attendee)
{
    if (attendee.getTicket().getCheckin().isCheckedin()) {
        std::cerr << "Error TitoApi::checkinAttendee() : The user "
                  << attendee.getName() << " has already checked in."
                  << std::endl;
        return false; // Illegal action
    }
    bool ret = false;

    std::string url = "https://checkin.tito.io/checkin_lists/"
                      + this->checkinSlug + "/checkins";
    std::string data = "{\"checkin\":{\"ticket_id\":"
                       + std::to_string(attendee.getTicket().getTicketID())
                       + "}}";
    std::string resp = sendRequest(url, data, "POST");
    nlohmann::json j = nlohmann::json::parse(resp);
    /*
     * The TiTo API is wank (if you pardon my french), so this is a good enough
     * error check. It returns a message if an error occurs.
     */
    if (!j.contains("ticket_id")) {
        std::cerr << "Error TitoApi::checkinAttendee() : The checkin slug is "
                  "incorrect or an internal TiTo error occurred."
                  << std::endl;
        throw TITO_INTERNAL_ERROR;
    }

    if (!this->hasIDBeenGiven(attendee)) {
        this->addIDToCache(attendee);
    }

    return ret;
}

bool TitoApi::checkoutAttendee(TitoAttendee attendee)
{
    if (!attendee.getTicket().getCheckin().isCheckedin()) {
        std::cerr << "Error TitoApi::checkoutAttendee() : The user "
                  << attendee.getName() << " has already checked out."
                  << std::endl;
        return false; // Illegal action
    }
    bool ret = false;

    std::string url = "https://checkin.tito.io/checkin_lists/"
                      + this->checkinSlug + "/checkins/"
                      + attendee.getTicket().getCheckin().getUUID();
    std::string data = "{\"checkin\":{\"ticket_id\":"
                       + std::to_string(attendee.getTicket().getTicketID())
                       + "}}";
    std::string resp = sendRequest(url, data, "DELETE");
    nlohmann::json j = nlohmann::json::parse(resp);
    /*
     * The TiTo API is wank (if you pardon my french), so this is a good enough
     * error check. It returns a message if an error occurs.
     */
    if (!j.contains("ticket_id")) {
        std::cerr << "Error TitoApi::checkoutAttendee() : The checkin slug is "
                  "incorrect or an internal TiTo error occurred."
                  << std::endl;
        throw TITO_INTERNAL_ERROR;
    }

    return ret;
}

bool TitoApi::checkAuthToken()
{
    std::string resp = getRequest("https://api.tito.io/v3/hello");
    nlohmann::json j = nlohmann::json::parse(resp);

    bool ret;
    if (!j.contains(TITO_AUTH_JSON_KEY)) {
        std::cerr << "Error TitoApi::checkAuthToken() : " << TITO_AUTH_JSON_KEY
                  << " is not in returned json." << std::endl;
        return false;
    }

    j.at(TITO_AUTH_JSON_KEY).get_to(ret);
    if (ret) {
        std::string accessTokenRaw;
        j.at(TITO_ACCESS_TOKEN_JSON_KEY).get_to(accessTokenRaw);
        // Remove the * from the access token
        this->accessToken = std::string(accessTokenRaw.c_str() + 1);

#ifdef DEBUG
        std::cerr << "Debug TitoApi::checkAuthToken() : Access token is "
                  << this->accessToken << std::endl;
#endif
    }

    return ret;
}

std::string getToken()
{
    char* token;
    token = getenv(TITO_TOKEN_ENV_VAR);

    if (token != NULL) {
        return std::string(token);
    }

    std::cerr << "Error TestTito::getToken() : No authentication token in "
              "environment variables."
              << std::endl;
    throw TITO_TOKEN_NOT_FOUND;
}

std::string getAccountSlug()
{
    char* token;
    token = getenv(TITO_ACCOUNT_SLUG_ENV_VAR);

    if (token != NULL) {
        return std::string(token);
    }

    std::cerr << "Error TestTito::getAccountSlug() : No account slug in "
              "environment variables."
              << std::endl;
    throw TITO_ACCOUNT_SLUG_NOT_FOUND;
}

std::string getEventSlug()
{
    char* token;
    token = getenv(TITO_EVENT_SLUG_ENV_VAR);

    if (token != NULL) {
        return std::string(token);
    }

    std::cerr << "Error TestTito::getEventSlug() : No event slug in environment "
              "variables."
              << std::endl;
    throw TITO_EVENT_SLUG_NOT_FOUND;
}

std::string getCheckinSlug()
{
    char* token;
    token = getenv(TITO_CHECKIN_SLUG_ENV_VAR);

    if (token != NULL) {
        return std::string(token);
    }

    std::cerr << "Error TestTito::getCheckinSlug() : No event slug in environment variables."
              << std::endl;
    throw TITO_CHECKIN_SLUG_NOT_FOUND;
}

std::string getTitoErrorMessage(int e)
{
    switch(e) {
    // Environment variable errors
    case TITO_TOKEN_NOT_FOUND:
        return TITO_TOKEN_ENV_VAR
               " is not defined in the environment variables.";
    case TITO_ACCOUNT_SLUG_NOT_FOUND:
        return TITO_ACCOUNT_SLUG_ENV_VAR
               " is not defined in the environment variables.";
    case TITO_EVENT_SLUG_NOT_FOUND:
        return TITO_EVENT_SLUG_ENV_VAR
               " is not defined in the environment variables.";
    case TITO_CHECKIN_SLUG_NOT_FOUND:
        return TITO_CHECKIN_SLUG_ENV_VAR
               " is not defined in the environment variables.";
    case TITO_ID_CACHE_ERROR:
        return "An error whilst reading or writing " ID_CACHE_FILE " has occurred.";
    // TiTo API Errors
    case TITO_NET_ERROR:
        return "A network error occurred when contacting the TiTo API.";
    case TITO_AUTH_ERROR:
        return "An authentication error occurred when contacting the TiTo API.";
    case TITO_INTERNAL_ERROR:
        return "A internal error occurred when contacting the TiTo API. (See stderr for details)";
    case TITO_ACCESS_TOKEN_ERROR:
        return "The check-in slug could not be found as there was an authentication error.";
    case TITO_CHECKINS_NOT_FOUND_ERROR:
        return "Check-ins could not be found, TiTo returned an error message.";
    case ID_CARD_READ_ERROR:
        return "The ID card template could not be read. See stderr for more details";
    default:
        return "An unknown error has occurred.";
    }
}
