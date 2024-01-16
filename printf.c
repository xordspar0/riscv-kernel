#include <stdarg.h>
#include <stdint.h>

#include "printf.h"
#include "uart.h"

int print(struct string s) {
  int i = 0;

  for (; i < s.length; i++) {
    uartputc_sync(s.data[i]);
  }

  return i;
}

void println(struct string s) {
  print(s);
  uartputc_sync('\n');
}

int printf(struct string format, ...) {
  int bytes_written = 0;
  va_list args;
  va_start(args);

	for (int i = 0; i < format.length; i++) {
    char c = format.data[i];
    if (c == '%') {
      i++;
      char specifier = format.data[i];

      switch (specifier) {
        case 'd': {
          long d = va_arg(args, long);
          bytes_written += print(int_to_string(d));
          break;
        }
        case '%':
          uartputc_sync('%');
          break;
      }

    } else {
      uartputc_sync(format.data[i]);
      bytes_written++;
    }
  }

  va_end(args);
  return bytes_written;
}

int print_hex_data(void * start, int length) {
	char chars[] = "0123456789ABCDEF";
	char white[] = " \n";
	int i = 0;

	for (; i < length; i++) {
		char value = ((char *)start)[i];
		char c0 = chars[(value >> 4) & 0xF];
		char c1 = chars[value & 0xF];
		char c2 = white[(i & 7) == 7];

		uartputc_sync(c0);
		uartputc_sync(c1);
		uartputc_sync(c2);
	}

	return i;
}
