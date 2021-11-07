#pragma once
#include <string>
#include <list>
#include "tito_classes.hpp"

#define TITO_TOKEN_NOT_FOUND 0x0101
#define TITO_ACCOUNT_SLUG_NOT_FOUND 0x0102
#define TITO_EVENT_SLUG_NOT_FOUND 0x0103

#define TITO_NET_ERROR 0x0001
#define TITO_AUTH_ERROR 0x0002
#define TITO_INTERNAL_ERROR 0x0003
#define TITO_ACCESS_TOKEN_ERROR 0x0004
#define TITO_CHECKINS_NOT_FOUND_ERROR 0x0005

#define TITO_TOKEN_ENV_VAR "TITO_TOKEN"
#define TITO_ACCOUNT_SLUG_ENV_VAR "TITO_ACCOUNT_SLUG"
#define TITO_EVENT_SLUG_ENV_VAR "TITO_EVENT_SLUG"

// 2018-06-18T09:35:39.000Z
// %Y  -%m-%dT%T
#define TITO_DATE_FORMAT "%Y-%m-%dT%T"

/**
 * The internal networking is purely GET requests over HTTPS, as such I chose to
 * use curl to make these requests because curl is well known to simply just 
 * work.
 */
class TitoApi {
public:
    /**
     * Inits the TiTo API token and then gets the check in slug using another
     * API call.
     * 
     * @param token The Tito authentication token. is copied in the constructor.
     * @param accountSlug The Tito account name (i.e: royal-hackaway)
     * @param eventSlug The Tito event name (i.e: v5)
     * @throws TITO_NET_ERROR if checkAuthToken() fails
     * @throws TITO_UNABLE_TO_GET_CHECKIN_SLUG if the checkin slug was not found
     */ 
    TitoApi(std::string /*token*/,
            std::string /* accountSlug */, 
            std::string /* eventSlug */);
    
    /**
     * Checks whether the authentication token passed in the constructor is
     * still valid.
     * 
     * @return whether the authentication token is correct.
     * @throws TITO_NET_ERROR if there was a network error whilst connecting to
     * the Tito system.
     */
    bool checkAuthToken();
    
    /**
     * Gets all the registered attendees to the event. It also gets all the
     * checkins for the tickets, this requires two api calls so it is twice as 
     * likely to error I guess.
     * 
     * @return all attendees to the event
     * @throws TITO_NET_ERROR if there was a network error whilst connecting to
     * @throws TITO_AUTH_ERROR if authentication failed
     * @throws TITO_INTERNAL_ERROR if there is an internal error
     * the Tito system
     */ 
    std::list<TitoAttendee> getAttendees();
private:
    /**
     * Makes a GET request to an endpoint with the tito API headers and returns 
     * the data.
     * 
     * @param url The URL to GET from.
     * @throws TITO_NET_ERROR When making the curl request fails.
     */ 
    std::string getRequest(std::string /* url */);
    std::string token,
                accountSlug,
                eventSlug,
                accessToken; // This slug is obtained on authentication.
};

std::string getToken();
std::string getAccountSlug();
std::string getEventSlug();
std::string getTitoErrorMessage(int);
