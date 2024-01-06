#include "mem.h"
#include "strings.h"

#include "printf.h"

struct string string_init(const char *input, uint8_t length) {
	struct string s = {.length = length, .data = {} };
	memmove(s.data, input, s.length);

	return s;
}

struct string int_to_string(int64_t num)
{
	int i = 0;
	int j = 0;
	char digits[20];
	struct string s = {0, {}};

	if(num < 0) {
		num = -num;
		j++;
		s.data[0] = '-';
	}

	do {
		digits[i] = num % 10 + '0';
		num /= 10;
		i++;
	} while(num);

	i--;

	for(; i >= 0; i--)
	{
		s.data[j] = digits[i];
		j++;
	}

	s.length = j;

	return s;
}
