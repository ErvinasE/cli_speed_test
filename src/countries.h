#ifndef COUNTRIES_H
#define COUNTRIES_H

typedef struct {
    const char *name;
    const char *code;
    int ping_count;
} CountryData;

CountryData* get_continent_data(const char *continent_code, int *size);

#endif // COUNTRIES_H
