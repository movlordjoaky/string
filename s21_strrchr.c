#include "s21_string.h"

char *s21_strrchr(const char *str, int c) {
  char *res = S21_NULL;

  for (int i = s21_strlen(str); res == S21_NULL && i >= 0; i--)
    if (str[i] == c) {
      res = (char *)&str[i];
    }

  return res;
}
