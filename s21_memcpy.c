#include "s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  if (dest != S21_NULL && src != S21_NULL) {
    unsigned char *cdest = (unsigned char *)dest;
    const unsigned char *csrc = (const unsigned char *)src;

    s21_size_t i = 0;

    while (i < n) {
      cdest[i] = csrc[i];
      i++;
    }
  }
  return dest;
}
