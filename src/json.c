#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include "location.c"
int parse_json()
{
    FILE *fp = fopen("test_file.json", "r");
    if (fp == NULL)
    {
        printf("Error: Can't open file \n");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *buffer = malloc(size + 1);

    if (buffer == NULL)
    {
        free(buffer);
        return 1;
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
        return 1;
    }

    int count = cJSON_GetArraySize(json);
    for (int i = 0; i < count; i++)
    {
        cJSON *item = cJSON_GetArrayItem(json, i);
        cJSON *country = cJSON_GetObjectItemCaseSensitive(item, "country");
        cJSON *city = cJSON_GetObjectItemCaseSensitive(item, "city");
        cJSON *provider = cJSON_GetObjectItemCaseSensitive(item, "provider");
        cJSON *host = cJSON_GetObjectItemCaseSensitive(item, "host");
        cJSON *id = cJSON_GetObjectItemCaseSensitive(item, "id");

        printf("Country: %s\n", country->valuestring);
        printf("City: %s\n", city->valuestring);
        printf("Provider: %s\n", provider->valuestring);
        printf("Host: %s\n", host->valuestring);
        printf("id: %d\n", id->valueint);
        printf("\n");
    }
    //printf("THE COUNT: %d\n", count);
    cJSON_Delete(json);
    free(buffer);
    return 0;
}
int parse_location(char *json_data, char *Country, char *CountryCode, char *ContinentCode, size_t maxlen)
{
    cJSON *json = cJSON_Parse(json_data);
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            printf("Error: %s\n", error_ptr);
        }
        cJSON_Delete(json);
        return 1;
    }
    cJSON *status = cJSON_GetObjectItemCaseSensitive(json, "status");
    if (status && status->valuestring && strcmp(status->valuestring, "fail") == 0)
    {
        printf("Status: %s\n", status->valuestring);
        printf("API ERROR\n");
        cJSON_Delete(json);
        return 1;

    }

    cJSON *country = cJSON_GetObjectItemCaseSensitive(json, "country");
    //printf("Country: %s\n", country->valuestring);

    cJSON *countryCode = cJSON_GetObjectItemCaseSensitive(json, "countryCode");
    //printf("Country code: %s\n", countryCode->valuestring);
    cJSON *continentCode = cJSON_GetObjectItemCaseSensitive(json, "continentCode");

    if ((country && country->valuestring) && countryCode && countryCode->valuestring && continentCode && continentCode->valuestring)
    {
        strncpy(Country, country->valuestring, maxlen - 1);
        strncpy(CountryCode, countryCode->valuestring, 9);
        strncpy(ContinentCode, continentCode->valuestring, 9);
        CountryCode[9] = '\0';
        ContinentCode[9] = '\0';

        Country[maxlen-1] = '\0';
        //CountryCode[maxlen-1] = '\0';
        
    }
    cJSON_Delete(json);

    return 0;
}