#include "printf.h"
#include "strings.h"

void main() {
	struct string s = STRING("hello\n");
	printf(&s);

	struct string n;

	n = int_to_string(0);
	println(&n);

	n = int_to_string(1);
	println(&n);

	n = int_to_string(-1);
	println(&n);

	n = int_to_string(0x7FFFFFFFFFFFFFFF);
	println(&n);

	n = int_to_string(-0x7FFFFFFFFFFFFFFF);
	println(&n);
}
