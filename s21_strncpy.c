
#include "s21_string.h"

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  return s21_memcpy((void *)dest, (const void *)src, n);
}
