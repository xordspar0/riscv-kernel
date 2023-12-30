#pragma once

#include <stdint.h>

struct string {
	uint8_t length;
	char data[255];
};

struct string string_init(const char *input, uint8_t strlen);

#define STRING(s) string_init(s, sizeof(s))
