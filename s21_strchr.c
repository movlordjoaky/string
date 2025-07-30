#include "s21_string.h"

char *s21_strchr(const char *str, int c) {
  return (char *)s21_memchr((const void *)str, c, s21_strlen(str) + 1);
}
