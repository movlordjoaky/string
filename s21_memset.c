#include "s21_string.h"

void *s21_memset(void *str, int c, s21_size_t n) {
  if (str != S21_NULL) {
    unsigned char *cstr = (unsigned char *)str;

    s21_size_t i = 0;

    while (i < n) {
      cstr[i] = (unsigned char)c;
      i++;
    }
  }
  return str;
}
