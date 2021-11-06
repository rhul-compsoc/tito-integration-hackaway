#pragma once
#include <string>
#include <list>
#include "tito_classes.hpp"

#define TITO_NET_ERROR 0x0001;
#define TITO_AUTH_ERROR 0x0002;
#define TITO_INTERNAL_ERROR 0x0003;

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
     * @param accountSlug The Tito account name (i.e: royal-hackaway)
     * @param eventSlug The Tito event name (i.e: v5)
     * The memory of the input string should be handled by the caller.
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
     * Gets all the registered attendees to the event.
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
    std::string token, accountSlug, eventSlug;
};
