#include "latency.h"
#include "json.h"
#include "location.h"
#include "speed_tests.h"
#include <unistd.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tests.h"

extern char *optarg;

int automated_test()
{
    //1 Get location, find best server, download, upload
    char Country[100] = {0};
    char CountryCode[10] = {0};
    char Continent[20] = {0};
    float best_ping;
    char *best_server = NULL;
    
    printf("Determining your location...\n");
    char* data = get_location();
    if (data == NULL || parse_location(data, Country, CountryCode, Continent, sizeof(Country)) != 0)
    {
        printf("Failure to determine location \n");
        if (data) free(data);
        return 1;
    }
    printf("Succesfully determined your location\n");

    printf("Finding the best server for your location...\n");
    char *server_name = find_best_server(Country, CountryCode, &best_ping, Continent);
    if (server_name == NULL)
    {
        printf("Error: no servers were found in that country or continent\n");
        free(data);
        return 1;
    }
    best_server = server_name;

    printf("Uploading 32mb of data to %s\n", best_server);
    double u_test = upload_test(best_server);

    printf("Downloading 32mb of data from %s\n", best_server);
    double d_test = download_test(best_server);

    printf("********************************************************************************************************\n");
    printf("Test Results:\n");
    printf("Your location: %s\n", Country);
    printf("Server: %s\n", best_server);
    if (u_test > 0) printf("Upload speed is : %.1f Mbps\n",u_test);
    else printf("Upload test failed: Connection timout\n");
    if (d_test > 0) printf("Download speed is : %.1f Mbps\n",d_test);
    else printf("Download test failed: Connection timeout\n");
    free(data);
    return 0;
}
int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        automated_test();
    }
    int opt;
    char Country[100] = {0};
    char CountryCode[10] = {0};
    char Continent[10] = {0};
    float best_ping;
    while ((opt = getopt(argc, argv, "u:d:lpt")) != -1)
    {
        switch(opt)
        {
            case 'u':
                printf("Uploading 32mb of data to %s\n", optarg);
                double u_test = upload_test(optarg);
                if (u_test <= 0)
                {
                    //printf("Upload test failed: Connection timout\n");
                    return 1;
                }
                else
                {
                    printf("*************************************************************\n");
                    printf("Test Results:\n");
                    printf("Upload speed is : %.1f Mbps\n",u_test);
                }
                break;
            case 'd':
                printf("Downloading 32mb of data from %s\n", optarg);
                double d_test = download_test(optarg);
                if (d_test <= 0)
                {
                    //printf("Download test failed: Connection timout\n");
                    return 1;
                }
                else
                {
                    printf("*************************************************************\n");
                    printf("Test Results:\n");
                    printf("Download speed is : %.1f Mbps\n",d_test);
                }
                break;
            case 'l':
                printf("Determining your location...\n");
                char* data = get_location();
                if (parse_location(data, Country, CountryCode, Continent, sizeof(Country)) == 0)
                {
                    printf("*************************************************************\n");
                    printf("Test Results:\n");
                    printf("Country: %s\n", Country);
                    //printf("Country Code: %s\n", CountryCode);
                }
                else
                {
                    printf("Failure to determine location \n");
                }
                free(data);
                break;
            case 'p':
                printf("Finding the best server for your location...\n");
                char* p_data = get_location();
                if (parse_location(p_data, Country, CountryCode, Continent, sizeof(Country)) == 0)
                {

                    char *server_name = find_best_server(Country, CountryCode, &best_ping, Continent);
                    printf("*************************************************************\n");
                    printf("Test Results:\n");
                    printf("Best server is: %s\n", server_name);
                }
                else
                {
                    printf("Failure to determine location \n");
                }
                //free(data);
                //printf("%s\n", Country);
                break;
            case 't':
                printf("Running non existant country test...\n");
                {
                    fake_country();
                }
                break;
            case '?':
                break;
        }
    }
    return 0;
}
