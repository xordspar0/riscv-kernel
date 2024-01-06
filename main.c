#include "printf.h"
#include "strings.h"

void main() {
	printf(STRING("hello\n"));

	printf(STRING("%d %d\n"), 0, 1);
	printf(STRING("%d %d\n"), 1, -1);
	printf(STRING("%d\n"), -1);
	printf(STRING("%d\n"), 0x7FFFFFFFFFFFFFFF);
	printf(STRING("%d\n"), -0x7FFFFFFFFFFFFFFF);

	char c = '%';
	println(string_init(&c, 1));

	printf(STRING("%d%%"), 99);
}
