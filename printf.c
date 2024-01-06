#include <stdint.h>

#include "printf.h"
#include "uart.h"

int printf(struct string *format, ...) {
  int i = 0;

	for (; i < format->length; i++) {
    uartputc_sync(format->data[i]);
  }

  return i;
}

void println(struct string *s) {
  printf(s);
  uartputc_sync('\n');
}
