#include <stdlib.h>

void fake_country()
{
    char fake_country[100] = "ZXJKJLKJ";
    char fake_code[10] = "XX";
    char Continent[10] = "XX";
    float best_ping;
    
    char *server = find_best_server(fake_country, fake_code, &best_ping, Continent);
    if (server == NULL) {
        printf("Correctly handled: No servers found for %s\n", fake_country);
    }
    printf("Server: %s\n", server);
}
