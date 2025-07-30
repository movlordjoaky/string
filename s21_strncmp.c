#include "s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  return s21_memcmp((const void *)str1, (const void *)str2, n);
}
