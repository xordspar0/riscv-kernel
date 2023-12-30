#include "mem.h"
#include "strings.h"

struct string string_init(const char *input, uint8_t length) {
	struct string s = {.length = length - 1, .data = {} };
	memmove(s.data, input, s.length);

	return s;
}
