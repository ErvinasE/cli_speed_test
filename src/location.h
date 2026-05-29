#ifndef LOCATION_H
#define LOCATION_H

#include <stddef.h>

struct MemoryStruct
{
    char *memory;
    size_t size;
};

char* get_location();
char *find_best_server(char *Country, char *CountryC, float *p_latency, char *Continent);

#endif // LOCATION_H
