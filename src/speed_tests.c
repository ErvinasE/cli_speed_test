#include <curl/curl.h>
#include <stdlib.h>


static size_t discard_data_callback (void *ptr, size_t size, size_t nmemb, void *userdata)
{
    return size * nmemb;
}
double download_test(const char *host)
{
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        return 1;
    }

    char url[256];
    snprintf(url, sizeof(url), "http://%s/speedtest/download?size=320000000", host);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_data_callback);

    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 15L);

    CURLcode result = curl_easy_perform(curl);

    curl_off_t speed = 0;
    if (result == CURLE_OK)
    {
        curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD_T, &speed);
        //printf("SPEED : %lldl\n", (long long)speed);

    long http_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    //printf("HTTP status: %ld\n", http_code);

    }
    else
    {
        fprintf(stderr, "Download failed: %s\n", curl_easy_strerror(result));
        curl_easy_cleanup(curl);
        return 1;
    }
    curl_easy_cleanup(curl);

    return (double)(speed * 8.0) / 1000000.0;
}
double upload_test(const char *host)
{
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        return 1;
    }

    char url[256];
    char *buffer;
    size_t upload_size = 32000000;
    buffer = malloc(upload_size);
    memset(buffer, 0, upload_size);
    snprintf(url, sizeof(url), "http://%s/speedtest/upload?size=32000000", host);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_data_callback);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buffer);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)upload_size);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 15L);

    CURLcode result = curl_easy_perform(curl);

    curl_off_t speed = 0;
    if (result == CURLE_OK)
    {
        curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD_T, &speed);
        //printf("SPEED : %lldl\n", (long long)speed);

    // long http_code;
    // curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    //printf("HTTP status: %ld\n", http_code);

    }
    else
    {
        fprintf(stderr, "Upload failed: %s\n", curl_easy_strerror(result));
        curl_easy_cleanup(curl);
        return 1;
    }
    free(buffer);
    curl_easy_cleanup(curl);

    return (double)(speed * 8.0) / 1000000.0;
}




