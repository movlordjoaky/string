#include "s21_string.h"

char *s21_strcpy(char *dest, const char *src) {
  if (dest == S21_NULL || src == S21_NULL) {
    return S21_NULL;
  }

  char *ptr = dest;
  while ((*ptr++ = *src++) != '\0') {
  }

  return dest;
}
