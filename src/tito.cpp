#include <curl/curl.h>
#include <iostream>
#include <string.h>
#include "tito.h"
#include "json.hpp"

#define TITO_AUTH_JSON_KEY "authenticated"

TitoApi::TitoApi(std::string token_str,
                 std::string accountSlug,
                 std::string eventSlug)
{
    this->token = std::string(token_str);
    this->accountSlug = std::string(accountSlug);
    this->eventSlug = std::string(eventSlug);
}

struct CurlResponse {
    char *ptr;
    size_t len;
};

static size_t write_callback(char *ptr,
                             size_t size,
                             size_t nmemb,
                             void *userdata)
{
    struct CurlResponse *response = (struct CurlResponse *) userdata;
    response->ptr = (char *) malloc((size * nmemb) + 1);
    if (response->ptr == NULL) {
        std::cerr << "Error TitoApi::write_callback() : malloc failed." << std::endl;
        return 0;
    }
    
    response->ptr[size * nmemb] = '\0';
    response->len = size * nmemb;
    memcpy(response->ptr, ptr, size * nmemb);
    
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

std::string TitoApi::getRequest(std::string url)
{
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
        }
        
        curl_easy_cleanup(curl);
        curl_slist_free_all(list);
        freeCurlResponse(&response);
        
        if (!getSuccess) {
            if (res != CURLE_OK) {
                std::cerr << "Error TitoApi::getRequest() : curl perform failed." << std::endl;
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));
            } else {
                std::cerr << "Error TitoApi::getRequest() : no response was read." << std::endl;
            }
            throw TITO_NET_ERROR;
        }
        
        return resp;
    } else {
        std::cerr << "Error TitoApi::getRequest() : curl init failed." << std::endl;
        throw TITO_NET_ERROR;
    }
}

std::list<TitoAttendee> TitoApi::getAttendees()
{
    std::string url = "https://api.tito.io/v3/" + this->accountSlug + "/" +
        this->eventSlug + "/registrations";
    std::string resp = getRequest(url);
    
    nlohmann::json rootJson = nlohmann::json::parse(resp);
    
    // Check for errors
    if (rootJson.contains("errors")) {
        std::cerr << "Error : TitoApi::getAttendees() : (probably) authentication "
                  << "or errors " << std::endl;
        throw TITO_AUTH_ERROR; // Probably an auth error lmao
    }
    if (!rootJson.contains("registrations")) {
        std::cerr << "Error : TitoApi::getAttendees() : interneal errors "
                  << std::endl;
        throw TITO_INTERNAL_ERROR;
    }
    
    // Start parsing
    std::list<TitoAttendee> out;
    nlohmann::json registrationJson = rootJson.at("registrations");
    for (nlohmann::json::iterator it = registrationJson.begin()
        ;it != registrationJson.end(); ++it) {
        nlohmann::json attendee = it.value();
        std::string name;
        std::string email;
        std::string phoneNumber;
        std::list<TitoAttendee> tickets;
        
        if (attendee.contains("tickets")) {
            nlohmann::json ticketsJson = attendee.at("tickets");
            for (nlohmann::json::iterator itt = registrationJson.begin()
                ;itt != registrationJson.end(); ++itt) {
                nlohmann::json ticketJson = itt.value();
            }
        }
        
        out.push_back(TitoAttendee(name, email, phoneNumber, tickets));
    }
    
    return out;
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
    
    return ret;
}
