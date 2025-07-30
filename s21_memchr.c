#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  unsigned char *s = (unsigned char *)str;
  int flag = 0;
  if (str != S21_NULL) {
    while (n) {
      if (*s == (unsigned char)c) {
        flag = 1;
        break;
      }

      n--;
      s++;
    }

    if (flag != 1) {
      s = S21_NULL;
    }
  }
  return s;
}
