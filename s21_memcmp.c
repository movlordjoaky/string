#include "s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  s21_size_t flag = 0;
  if (str1 != S21_NULL && str2 != S21_NULL) {
    unsigned char *s1 = (unsigned char *)str1;
    unsigned char *s2 = (unsigned char *)str2;

    s21_size_t i = 0;
    while (i < n) {
      if (s1[i] != s2[i]) {
        flag = s1[i] - s2[i];
        i += n;
      }

      i++;
    }
  }

  return flag;
}
