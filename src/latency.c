#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
float ping_server(const char *host)
{
    char url[128];
    snprintf(url, sizeof(url), "http://%s", host);
    CURL *curl = curl_easy_init();

    if(!curl)
    {
        fprintf(stderr, "Init failure\n");
        return -1;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);

    curl_easy_perform(curl);

    CURLcode result;
    
    double ping_time;
    curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &ping_time);

    curl_easy_cleanup(curl);
    
    if (ping_time * 1000 <= 0)
    {
        //printf("Connection error: Invalid host\n");
        return -1;
    }
    return ping_time * 1000;
}