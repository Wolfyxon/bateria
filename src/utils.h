#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <sys/stat.h>

float clock_sec();
void wait(float s);

bool dir_exists(char *path);

bool is_int(char *string);
bool is_whitespace(char c);

bool streq(const char *a, const char *b);
void strtrimbounds(const char *string, size_t *start, size_t *end);
void strslice(char *dest, const char *src, size_t start, size_t end);

bool parse_int(int *dest, const char *string);
