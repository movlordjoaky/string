#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  if (delim == S21_NULL) return S21_NULL;

  static char *next_tok = S21_NULL;
  char *tok = S21_NULL;

  if (str) {
    next_tok = str;
  }

  if (next_tok) {
    while (*next_tok && s21_strchr(delim, *next_tok)) {
      next_tok++;
    }

    if (*next_tok != '\0') {
      tok = next_tok;

      while (*next_tok && !s21_strchr(delim, *next_tok)) {
        next_tok++;
      }

      if (*next_tok) {
        *next_tok++ = '\0';
      } else {
        next_tok = S21_NULL;
      }
    }
  }

  return tok;
}
