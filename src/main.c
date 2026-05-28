#include "latency.c"
#include "json.c"
#include "speed_tests.c"
#include <unistd.h>
#include <locale.h>
#include <stdio.h>


extern char *optarg;

int automated_test()
{
    //1 Get location, find best server, download, upload

    char Country[100] = {0};
    char CountryCode[10] = {0};
    float best_ping;
    char *best_server;
    
    printf("Determining your location...\n");
    char* data = get_location();
    if (parse_location(data, Country, CountryCode, sizeof(Country)) != 0)
    {
        printf("Failure to determine location \n");
    }
    else
    {
        printf("Succesfully determined your location\n");
    }
    printf("Finding the best server for your location...\n");
    char* p_data = get_location();
    if (parse_location(p_data, Country, CountryCode, sizeof(Country)) == 0)
    {
        char *server_name = find_best_server(Country, CountryCode, &best_ping);
        best_server = server_name;

    }
    else
    {
        printf("Failure to determine location \n");
    }

    printf("Uploading 32mb of data to %s\n", best_server);
    double u_test = upload_test(best_server);
    if (u_test == 1.000000)
    {
        return 1;
    }
    printf("Downloading 32mb of data from %s\n", best_server);
    double d_test = download_test(best_server);
    if (d_test == 1.000000)
    {
        return 1;
    }
    printf("********************************************************************************************************\n");
    printf("Test Results:\n");
    printf("Your location: %s\n", Country);
    printf("Server: %s\n", best_server);
    printf("Upload speed is : %.1f Mbps\n",u_test);
    printf("Download speed is : %.1f Mbps\n",d_test);
    free(data);
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
    float best_ping;
    while ((opt = getopt(argc, argv, "u:d:lp")) != -1)
    {
        switch(opt)
        {
            case 'u':
                printf("Uploading 32mb of data to %s\n", optarg);
                double u_test = upload_test(optarg);
                if (u_test == 1.000000)
                {
                    break;
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
                if (d_test == 1.000000)
                {
                    break;
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
                if (parse_location(data, Country, CountryCode, sizeof(Country)) == 0)
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
                if (parse_location(p_data, Country, CountryCode, sizeof(Country)) == 0)
                {

                    char *server_name = find_best_server(Country, CountryCode, &best_ping);
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
            case '?':
                break;
        }
    }
    return 0;
}
