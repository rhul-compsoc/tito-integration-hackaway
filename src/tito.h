#pragma once
#include <string>
#include <list>
#include "tito_classes.h"

#define ID_CACHE_FILE "ids_given.txt"

#define TITO_TOKEN_NOT_FOUND 0x01001
#define TITO_ACCOUNT_SLUG_NOT_FOUND 0x01002
#define TITO_EVENT_SLUG_NOT_FOUND 0x01003
#define TITO_CHECKIN_SLUG_NOT_FOUND 0x01004
#define TITO_ID_CACHE_ERROR 0x01005

#define TITO_NET_ERROR 0x0001
#define TITO_AUTH_ERROR 0x0002
#define TITO_INTERNAL_ERROR 0x0003
#define TITO_ACCESS_TOKEN_ERROR 0x0004
#define TITO_CHECKINS_NOT_FOUND_ERROR 0x0005

#define TITO_TOKEN_ENV_VAR "TITO_TOKEN"
#define TITO_ACCOUNT_SLUG_ENV_VAR "TITO_ACCOUNT_SLUG"
#define TITO_EVENT_SLUG_ENV_VAR "TITO_EVENT_SLUG"
#define TITO_CHECKIN_SLUG_ENV_VAR "TITO_CHECKIN_SLUG"

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
            std::string /* eventSlug */,
            std::string /* checkinSlug */);
    TitoApi() {}; // Do not use this constructor please    
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
    /**
     * Checks a user into the system.
     * 
     * @param TitoAttendee the attendee to checkin to the system
     * @return whether the operation was a success
     * @throws TITO_NET_ERROR if there was a network error whilst connecting to
     * @throws TITO_AUTH_ERROR if authentication failed due to an invalid 
     * checkin slug
     * @throws TITO_INTERNAL_ERROR if there is an internal error
     */
    bool checkinAttendee(TitoAttendee);
    /**
     * Checks a user out of the system.
     *
     * @param TitoAttendee the attendee to check out to the system
     * @return whether the operation was a success
     * @throws TITO_NET_ERROR if there was a network error whilst connecting to
     * @throws TITO_AUTH_ERROR if authentication failed due to an invalid
     * checkin slug
     * @throws TITO_INTERNAL_ERROR if there is an internal error
     */
    bool checkoutAttendee(TitoAttendee);
    /**
     * Returns whether a TitoATttendee ha been give their ID card.
     * 
     * @param TitoAttendee the attendee to check
     * @return whether they have been given their ID card
     */ 
    bool hasIDBeenGiven(TitoAttendee);
    /**
     * Marks an attendee as having their ID card given out.
     * 
     * @param TitoAttendee the attendee to mark as having their ID card given out
     * @throws TITO_ID_CACHE_ERROR if they could not be marked as having their ID
     * cache given out
     */ 
    void addIDToCache(TitoAttendee);
private:
    /**
     * Makes a GET request to an endpoint with the tito API headers and returns 
     * the data.
     * 
     * @param url The URL to GET from.
     * @throws TITO_NET_ERROR When making the curl request fails.
     */ 
    std::string getRequest(std::string /* url */);
    /**
     * Makes a POST request to an endpoint with the tito API headers, the data 
     * that is passed and, returns the data received.
     * 
     * @param url The URL to get from.
     * @param data The data that should be sent with the POST request.
     * @param request_method The method to use i.e: POST.
     * @throws TITO_NET_ERROR When making the curl request fails.
     */ 
    std::string sendRequest(std::string /* url */,
                            std::string /* data */,
                            std::string /* request method */);
    std::string token,
                accountSlug,
                eventSlug,
                checkinSlug,
                accessToken; // This slug is obtained on authentication.
    std::list<std::string> idsGiven;
    void readIDCache();
};

std::string getToken();
std::string getAccountSlug();
std::string getEventSlug();
std::string getCheckinSlug();
std::string getTitoErrorMessage(int);
