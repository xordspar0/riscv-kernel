#include <stdint.h>
#include <stddef.h>

#include "mem.h"

void *memset(void *dest, int ch, size_t count) {
	uint8_t *d_bytes = dest;

	for (size_t i = 0; i < count; i++) {
		d_bytes[i] = ch;
	}

	return dest;
}

void *memmove(void *dest, const void *src, size_t count) {
	uint8_t *d_bytes = dest;
	const uint8_t *s_bytes = src;
	
	for (size_t i = 0; i < count; i++) {
		d_bytes[i] = s_bytes[i];
	}

	return dest;
}

void *memcpy(void *dest, const void *src, size_t count) {
	return memmove(dest, src, count);
}
