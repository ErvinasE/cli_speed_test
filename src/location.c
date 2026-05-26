#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>

struct MemoryStruct
{
    char *memory;
    size_t size;
};

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

    curl_easy_setopt(curl, CURLOPT_URL, "http://ip-api.com/json/?fields=1105945");    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, parse_data);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data);

    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    

    result = curl_easy_perform(curl);

    if (result != CURLE_OK)
    {
        fprintf(stderr, "Download falure %s\n", curl_easy_strerror(result));
    }

    //printf("%s\n", data.memory);

    curl_easy_cleanup(curl);
    //free(data.memory);
    return data.memory;
}