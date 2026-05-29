#include "location.h"
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <stdbool.h>
#include "countries.h"
#include "latency.h"

static size_t parse_data(char *buffer, size_t size, size_t nitems, void* userdata)
{
    size_t bytes = size * nitems;
    struct MemoryStruct *mem = (struct MemoryStruct *)userdata;
    char *ptr = realloc(mem->memory, mem->size + bytes + 1);
    if (!ptr)
    {
        printf("out of memory \n");
        return 0;
    }
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), buffer, bytes);
    mem->size += bytes;
    mem->memory[mem->size] = 0;

    //printf("Chunk size : %zu \n", bytes );
    return bytes;
}
char* get_location()
{
    struct MemoryStruct data;
    data.memory = malloc(1);
    if (data.memory == NULL)
    {
        fprintf(stderr, "EROR: Failed allocation \n");
        free(data.memory);
        return NULL;
    }
    data.size = 0;
    CURL *curl = curl_easy_init();
    if(!curl)
    {
        fprintf(stderr, "Init failure\n");
        return NULL;
    }
    CURLcode result;

    //CURENT IP : http://ip-api.com/json/?fields=57347

    curl_easy_setopt(curl, CURLOPT_URL, "http://ip-api.com/json/?fields=2146307");
    //1097731
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, parse_data);
    
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data);

    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    

    result = curl_easy_perform(curl);

    if (result != CURLE_OK)
    {
        fprintf(stderr, "Download failure %s\n", curl_easy_strerror(result));
    }

    //printf("%s\n", data.memory);

    curl_easy_cleanup(curl);
    //free(data.memory);
    return data.memory;
}
char *find_best_server(char *Country, char *CountryC, float *p_latency, char *Continent)
{
    FILE *fp = fopen("speedtest_server_list.json", "r");
    if (fp == NULL)
    {
        printf("Error: Can't open file \n");
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *buffer = malloc(size + 1);

    if (buffer == NULL)
    {
        free(buffer);
        return NULL;
    }
    int len = fread(buffer, 1, size, fp);
    fclose(fp);

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            printf("Error: %s\n", error_ptr);
        }
        free(buffer);
        cJSON_Delete(json);
        return NULL;
    }

    int count = cJSON_GetArraySize(json);
    float max_latency = 9999.f;
    char *best_server = NULL;
    bool found = false;
    for (int i = 0; i < count; i++)
    {
        cJSON *item = cJSON_GetArrayItem(json, i);
        cJSON *country = cJSON_GetObjectItemCaseSensitive(item, "country");

        if (strcmp(country->valuestring, Country) == 0 || strcmp(country->valuestring, CountryC) == 0) //TODO: ADD EDGE CASE, IF WE DONT FIND ANY WORKING SERVERS IN USER LOCATION
        {
            cJSON *provider = cJSON_GetObjectItemCaseSensitive(item, "provider");
            cJSON *host = cJSON_GetObjectItemCaseSensitive(item, "host");
            cJSON *id = cJSON_GetObjectItemCaseSensitive(item, "id");
            float latency = ping_server(host->valuestring);
            printf("Checking: %s server \n", host->valuestring);
            if (latency > 0)
            {
                //printf("Latency is : %f\n", latency);
                if (latency <= max_latency)
                {
                    best_server = host->valuestring;
                    found = true;
                    max_latency = latency;
                }
                else
                {
                    //printf("Testing");
                    continue;
                }
            }
            else
            {
                printf("Connection Error : Invalid Host\n");

            }

        }
    }

    if (found == false)
    {
        printf("Found 0 working servers inside %s, trying the %s continent...\n", Country, Continent);
        int continent_size = 0;
        CountryData *continent_data = get_continent_data(Continent, &continent_size);
        if (continent_data != NULL)
        {
            for (int k = 0; k < continent_size; k++) continent_data[k].ping_count = 0;

            for (int i = 0; i < count; i++)
            {
                cJSON *item = cJSON_GetArrayItem(json, i);
                cJSON *country_item = cJSON_GetObjectItemCaseSensitive(item, "country");
                if (!country_item || !country_item->valuestring) continue;

                for (int k = 0; k < continent_size; k++)
                {
                    if (strcmp(country_item->valuestring, continent_data[k].name) == 0 || strcmp(country_item->valuestring, continent_data[k].code) == 0)
                    {
                        if (continent_data[k].ping_count < 2)
                        {
                            cJSON *host = cJSON_GetObjectItemCaseSensitive(item, "host");
                            if (!host || !host->valuestring) continue;

                            printf("Checking %s %s server \n", continent_data[k].name, host->valuestring);
                            continent_data[k].ping_count++;
                            float latency = ping_server(host->valuestring);
                            if (latency > 0)
                            {
                                if (latency <= max_latency)
                                {
                                    best_server = host->valuestring;
                                    found = true;
                                    max_latency = latency;
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
    *p_latency = max_latency;
    return best_server;
}
