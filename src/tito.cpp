#include <curl/curl.h>
#include <iostream>
#include <string.h>
#include "tito.h"
#include "json.hpp"

#define TITO_AUTH_KEY "authenticated"

TitoApi::TitoApi(std::string token_str)
{
    this->token = std::string(token_str);
}

struct CurlResponse {
    char *ptr;
    size_t len;
};

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
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

bool TitoApi::checkAuthToken()
{
    std::string resp = getRequest("https://api.tito.io/v3/hello");
    nlohmann::json j = nlohmann::json::parse(resp);
    
    bool ret;
    if (!j.contains(TITO_AUTH_KEY)) {
        std::cerr << "Error TitoApi::checkAuthToken() : " << TITO_AUTH_KEY 
                  << " is not in returned json." << std::endl;
        return false;
    }
    
    j.at(TITO_AUTH_KEY).get_to(ret);
    
    return ret;
}


