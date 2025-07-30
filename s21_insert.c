#include "s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  if (src == S21_NULL || str == S21_NULL) {
    return S21_NULL;
  }

  char *res = S21_NULL;
  s21_size_t src_len = s21_strlen(src);
  s21_size_t str_len = s21_strlen(str);

  if (start_index <= src_len) {
    res = (char *)malloc((src_len + str_len + 1) * sizeof(char));
    if (res != S21_NULL) {
      s21_strncpy(res, src, start_index);
      s21_strcpy(res + start_index, str);
      s21_strcpy(res + start_index + str_len, src + start_index);
    }
  }

  return res;
}
