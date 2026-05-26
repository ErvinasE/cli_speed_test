#include <stdio.h>
#include <unistd.h>
#include "json_test.c"
#include <locale.h>
//#include "location.c"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    int opt;
    while ((opt = getopt(argc, argv, "udsl")) != -1)
    {
        switch(opt)
        {
            case 'u':
                printf("Uploading 5 mb of data...\n");
                break;
            case 'd':
                printf("Downloading 5mb of data...\n");
                break;
            case 's':
                printf("Please specify the server ip and id\n");
                parse_json();
                break;
            case 'l':
                printf("Determining your location...\n");
                char* data = get_location();
                parse_location(data);
                free(data);
                break;
            case '?':
                break;
        }
    }
    return 0;
}