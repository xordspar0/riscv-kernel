#include "printf.h"
#include "strings.h"

void main() {
	struct string s = STRING("hello\n");
	printf(&s);
}
