#include "utils.h"

float clock_sec() {
    return (float)clock() / (float)CLOCKS_PER_SEC;
}

void wait(float s) {
    float start = clock_sec();
    float end = start + s;

    while(clock_sec() < end);
}

bool dir_exists(char *path) {
    struct stat stats;
    stat(path, &stats);

    return S_ISDIR(stats.st_mode);
}

bool is_int(char *string) {
    size_t len = strlen(string);

    bool has_minus = false;
    bool got_digit = false;

    if(len == 0) {
        return false;
    }

    for(size_t i = 0; i < len; i++) {
        char c = string[i];

        if(c == '-') {
            if(has_minus || got_digit) {
                return false;
            }

            has_minus = true;
            continue;
        }

        if(!isdigit(c)) {
            return false;
        }
        
        got_digit = true;
    }

    return true;
}

bool is_whitespace(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

bool streq(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

void strtrimbounds(const char *string, size_t *start, size_t *end) {
    size_t len = strlen(string);

    if(len == 0) {
        return;
    }

    bool got_start = false;

    for(size_t i = 0; i < len; i++) {
        char c = string[i];
        
        if(!is_whitespace(c)) {
            if(!got_start) {
                *start = i;
                got_start = true;
            }
            
            *end = i + 1;
        }
    }
}

void strslice(char *dest, const char *src, size_t start, size_t end) {
    if(strlen(src) < start) {
        fprintf(stderr, "error: strlice(): start greater than length of src\n");
        dest[0] = '\0';
        return;
    }

    strncpy(dest, src + start, (end - start));
    dest[end - start] = '\0';
}

bool parse_int(int *dest, const char *string) {
    size_t start, end = 0;
    strtrimbounds(string, &start, &end);
    
    char trimmed[(end - start) + 1];
    strslice(trimmed, string, start, end);
    
    if(!is_int(trimmed)) {
        return false;
    }

    *dest = atoi(trimmed);
    return true;
}
