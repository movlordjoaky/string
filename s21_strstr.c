#include "s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  if (haystack == S21_NULL || needle == S21_NULL) return S21_NULL;

  char *res = S21_NULL;

  for (s21_size_t i = 0; res == S21_NULL && haystack[i] != '\0'; i++) {
    s21_size_t j = 0;
    while (haystack[i + j] == needle[j] && needle[j] != '\0') j++;

    if (needle[j] == '\0') res = (char *)&haystack[i];
  }

  return res;
}
