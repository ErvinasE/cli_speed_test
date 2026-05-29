#ifndef JSON_H
#define JSON_H

#include <stddef.h>

int parse_json();
int parse_location(char *json_data, char *Country, char *CountryCode, char *ContinentCode, size_t maxlen);

#endif // JSON_H
