#pragma once
#include <string>
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
    ~TitoApi();
    
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
