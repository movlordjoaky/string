#include <check.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_string.h"

// memchr

START_TEST(test_memchr_equal) {
  char str1[4] = "abc";
  void *mem1, *mem2;
  mem1 = memchr(str1, 'b', 3);
  mem2 = s21_memchr(str1, 'b', 3);
  ck_assert_ptr_eq(mem1, mem2);
}
END_TEST

START_TEST(test_memchr_notequal) {
  char str1[4] = "abc";
  void *mem1, *mem2;
  mem1 = memchr(str1, 'b', 3);
  mem2 = s21_memchr(str1, 'c', 3);
  ck_assert_ptr_ne(mem1, mem2);
}
END_TEST

START_TEST(test_memchr_null) {
  char str1[4] = "abc";
  void *mem1, *mem2;
  mem1 = memchr(str1, 'd', 3);
  mem2 = s21_memchr(str1, 'd', 3);
  ck_assert_ptr_null(mem1);
  ck_assert_ptr_null(mem2);
}
END_TEST

// memcmp

START_TEST(test_memcmp_equal) {
  char str1[4] = "abc", str2[100] = "abc";
  int cmp1 = memcmp(str1, str2, 2);
  int cmp2 = s21_memcmp(str1, str2, 2);
  ck_assert_int_eq(cmp1, cmp2);
}
END_TEST

START_TEST(test_memcmp_gr) {
  char str1[4] = "abd", str2[4] = "abc";
  int cmp1 = memcmp(str1, str2, 3);
  int cmp2 = s21_memcmp(str1, str2, 3);
  ck_assert_int_eq(cmp1, cmp2);
}
END_TEST

// memcpy

START_TEST(test_memcpy) {
  char src[4] = "abc", dest1[4] = "", dest2[4] = "";
  void *cp1 = memcpy(dest1, src, 2);
  void *cp2 = s21_memcpy(dest2, src, 2);
  ck_assert_str_eq(cp1, cp2);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_memcpy_overflow) {
  char src[4] = "abc", dest1[4] = "", dest2[4] = "";
  void *cp1 = memcpy(dest1, src, 3);
  void *cp2 = s21_memcpy(dest2, src, 3);
  ck_assert_str_eq(cp1, cp2);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

// memset

START_TEST(test_memset) {
  char dest1[4] = "", dest2[4] = "";
  void *cp1 = memset(dest1, 'a', 3);
  void *cp2 = s21_memset(dest2, 'a', 3);
  ck_assert_str_eq(cp1, cp2);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

// strncat

START_TEST(test_strncat) {
  char dest1[7] = "abc", dest2[7] = "abc", src[4] = "def";
  void *cp1 = strncat(dest1, src, 3);
  void *cp2 = s21_strncat(dest2, src, 3);
  ck_assert_str_eq(cp1, cp2);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

// strchr

START_TEST(test_strchr_found) {
  char str1[7] = "abc";
  char *srch1 = strchr(str1, 'c');
  char *srch2 = s21_strchr(str1, 'c');
  ck_assert_int_eq(*srch1, *srch2);
}
END_TEST

START_TEST(test_strchr_not_found) {
  char str1[7] = "abc";
  char *srch1 = strchr(str1, 'd');
  char *srch2 = s21_strchr(str1, 'd');
  ck_assert_ptr_null(srch1);
  ck_assert_ptr_null(srch2);
}
END_TEST

// strncmp

START_TEST(test_strncmp_eq) {
  char str1[4] = "abd", str2[4] = "abc";
  int cmp1 = strncmp(str1, str2, 2);
  int cmp2 = s21_strncmp(str1, str2, 2);
  ck_assert_int_eq(cmp1, cmp2);
}
END_TEST

// strncpy

START_TEST(test_strncpy) {
  char src[4] = "abc", dest1[4] = "", dest2[4] = "";
  char *cp1 = strncpy(dest1, src, 2);
  char *cp2 = s21_strncpy(dest2, src, 2);
  ck_assert_str_eq(cp1, cp2);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

// strcspn

START_TEST(test_strcspn) {
  char str1[5] = "abcd", str2[4] = "cd";
  size_t sz1 = strcspn(str1, str2);
  size_t sz2 = s21_strcspn(str1, str2);
  ck_assert_int_eq(sz1, sz2);
}
END_TEST

// strerror

START_TEST(test_strerror) {
  for (int i = 0; i < 100; i++) {
    char *er1 = strerror(i);
    char *er2 = s21_strerror(i);
    ck_assert_str_eq(er1, er2);
  }
}
END_TEST

// strlen

START_TEST(test_strlen) {
  char str1[4] = "abc";
  size_t sz1 = strlen(str1);
  size_t sz2 = s21_strlen(str1);
  ck_assert_int_eq(sz1, sz2);
}
END_TEST

// strpbrk

START_TEST(test_strpbrk_found) {
  char str1[4] = "abc", str2[3] = "db";
  char *srch1 = strpbrk(str1, str2);
  char *srch2 = s21_strpbrk(str1, str2);
  ck_assert_int_eq(*srch1, *srch2);
}
END_TEST

START_TEST(test_strpbrk_not_found) {
  char str1[4] = "abc", str2[4] = "def";
  char *srch1 = strpbrk(str1, str2);
  char *srch2 = s21_strpbrk(str1, str2);
  ck_assert_ptr_null(srch1);
  ck_assert_ptr_null(srch2);
}
END_TEST

// strrchr

START_TEST(test_strrchr_found) {
  char str1[7] = "abc";
  char *srch1 = strrchr(str1, 'c');
  char *srch2 = s21_strrchr(str1, 'c');
  ck_assert_int_eq(*srch1, *srch2);
}
END_TEST

START_TEST(test_strrchr_not_found) {
  char str1[7] = "abc";
  char *srch1 = strrchr(str1, 'd');
  char *srch2 = s21_strrchr(str1, 'd');
  ck_assert_ptr_null(srch1);
  ck_assert_ptr_null(srch2);
}
END_TEST

// strstr

START_TEST(test_strstr_found) {
  char str1[7] = "abcdef", str2[4] = "de";
  char *srch1 = strstr(str1, str2);
  char *srch2 = s21_strstr(str1, str2);
  ck_assert_int_eq(*srch1, *srch2);
}
END_TEST

START_TEST(test_strstr_not_found) {
  char str1[7] = "abcdef", str2[4] = "gh";
  char *srch1 = strstr(str1, str2);
  char *srch2 = s21_strstr(str1, str2);
  ck_assert_ptr_null(srch1);
  ck_assert_ptr_null(srch2);
}
END_TEST

// strtok

START_TEST(test_strtok_middle_1) {
  char str1[18] = "word1,word2 word3", str2[18], delim[3] = ", ";
  strcpy(str2, str1);
  char *tok1 = strtok(str1, delim);
  char *tok2 = s21_strtok(str2, delim);
  ck_assert_str_eq(tok1, tok2);
}
END_TEST

START_TEST(test_strtok_middle_2) {
  char str1[18] = "word1,word2 word3", str2[18], delim[3] = ", ";
  strcpy(str2, str1);
  char *tok1 = strtok(str1, delim);
  char *tok2 = s21_strtok(str2, delim);
  ck_assert_str_eq(tok1, tok2);
}
END_TEST

START_TEST(test_strtok_end) {
  char str1[12] = "word2", str2[12], delim[3] = ", ";
  strcpy(str2, str1);
  char *tok1 = strtok(str1, delim);
  char *tok2 = s21_strtok(str2, delim);
  ck_assert_str_eq(tok1, tok2);
}
END_TEST

// sprintf (integer)
START_TEST(test_sprintf_int) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  int i = 12345;
  sprintf(sprintf_buf, "%d", i);
  s21_sprintf(s21_sprintf_buf, "%d", i);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (integer_n)
START_TEST(test_sprintf_int_n) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  int i = 12345;
  int n;
  sprintf(sprintf_buf, "%d %n", i, &n);
  s21_sprintf(s21_sprintf_buf, "%d %n", i, &n);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (integer +)
START_TEST(test_sprintf_int_plus) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  int i = 12345;
  sprintf(sprintf_buf, "%+d", i);
  s21_sprintf(s21_sprintf_buf, "%+d", i);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (float)
START_TEST(test_sprintf_float) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  float i = 12345.54321;
  sprintf(sprintf_buf, "%f", i);
  s21_sprintf(s21_sprintf_buf, "%f", i);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (float *)
START_TEST(test_sprintf_float_star) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  float i = 12345.54321;
  sprintf(sprintf_buf, "%.*f", 1, i);
  s21_sprintf(s21_sprintf_buf, "%.*f", 1, i);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (float exp)
START_TEST(test_sprintf_float_exp) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  float i = 12345.54321;
  sprintf(sprintf_buf, "%e", i);
  s21_sprintf(s21_sprintf_buf, "%e", i);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (float -)
START_TEST(test_sprintf_float_minus) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  float i = 12345.5;
  sprintf(sprintf_buf, "%-.1f", i);
  s21_sprintf(s21_sprintf_buf, "%-.1f", i);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (float space)
START_TEST(test_sprintf_float_space) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  float i = 12345;
  sprintf(sprintf_buf, "% .0f", i);
  s21_sprintf(s21_sprintf_buf, "% .0f", i);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (float zero)
START_TEST(test_sprintf_float_zero) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  float i = 12345;
  sprintf(sprintf_buf, "%05.1f", i);
  s21_sprintf(s21_sprintf_buf, "%05.1f", i);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (hex)
START_TEST(test_sprintf_hex) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  int i = 21;
  sprintf(sprintf_buf, "%x", i);
  s21_sprintf(s21_sprintf_buf, "%x", i);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (hex_square)
START_TEST(test_sprintf_hex_square) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  int i = 21;
  sprintf(sprintf_buf, "%#x", i);
  s21_sprintf(s21_sprintf_buf, "%#x", i);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (octal)
START_TEST(test_sprintf_octal) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  int i = 21;
  sprintf(sprintf_buf, "%o", i);
  s21_sprintf(s21_sprintf_buf, "%o", i);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (unsigned int)
START_TEST(test_sprintf_unsigned_int) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  unsigned int i = 21;
  sprintf(sprintf_buf, "%u", i);
  s21_sprintf(s21_sprintf_buf, "%u", i);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (unsigned int_h)
START_TEST(test_sprintf_unsigned_int_h) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  unsigned int i = 21;
  sprintf(sprintf_buf, "%hu", i);
  s21_sprintf(s21_sprintf_buf, "%hu", i);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (unsigned long long)
START_TEST(test_sprintf_unsigned_long_long) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  unsigned long long i = 21;
  sprintf(sprintf_buf, "%lld", i);
  s21_sprintf(s21_sprintf_buf, "%lld", i);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (long)
START_TEST(test_sprintf_long) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  long i = 21;
  sprintf(sprintf_buf, "%ld", i);
  s21_sprintf(s21_sprintf_buf, "%ld", i);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (float)
START_TEST(test_sprintf_float_2) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  double d = 21.21;
  sprintf(sprintf_buf, "%.2f", d);
  s21_sprintf(s21_sprintf_buf, "%.2f", d);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (negative float)
START_TEST(test_sprintf_negative_float) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  double d = -21.21;
  sprintf(sprintf_buf, "%.2f", d);
  s21_sprintf(s21_sprintf_buf, "%.2f", d);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (char)
START_TEST(test_sprintf_char) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  char c = 's';
  sprintf(sprintf_buf, "%c", c);
  s21_sprintf(s21_sprintf_buf, "%c", c);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (string)
START_TEST(test_sprintf_string) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  char str[] = "abc";
  sprintf(sprintf_buf, "%s", str);
  s21_sprintf(s21_sprintf_buf, "%s", str);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (percent)
START_TEST(test_sprintf_percent) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  char str[] = "abc%%";
  sprintf(sprintf_buf, "%s", str);
  s21_sprintf(s21_sprintf_buf, "%s", str);
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sprintf (percent)
START_TEST(test_sprintf_percent_2) {
  char s21_sprintf_buf[100];
  char sprintf_buf[100];
  sprintf(sprintf_buf, "%%");
  s21_sprintf(s21_sprintf_buf, "%%");
  ck_assert_str_eq(sprintf_buf, s21_sprintf_buf);
}
END_TEST

// sscanf (int)
START_TEST(test_sscanf_int) {
  int s21_i;
  int i;
  sscanf("21", "%d", &i);
  s21_sscanf("21", "%d", &s21_i);
  ck_assert_int_eq(i, s21_i);
}
END_TEST

// sscanf (int skip)
START_TEST(test_sscanf_int_skip) {
  int s21_i;
  int i;
  sscanf("20 21", "%*d %d", &i);
  s21_sscanf("20 21", "%*d %d", &s21_i);
  ck_assert_int_eq(i, s21_i);
}
END_TEST

// sscanf (int negative)
START_TEST(test_sscanf_int_negative) {
  int s21_i;
  int i;
  sscanf("-21", "%d", &i);
  s21_sscanf("-21", "%d", &s21_i);
  ck_assert_int_eq(i, s21_i);
}
END_TEST

// sscanf (int positive)
START_TEST(test_sscanf_int_positive) {
  int s21_i;
  int i;
  sscanf("+21", "%d", &i);
  s21_sscanf("+21", "%d", &s21_i);
  ck_assert_int_eq(i, s21_i);
}
END_TEST

// sscanf (int_h)
START_TEST(test_sscanf_int_h) {
  short int s21_i;
  short int i;
  sscanf("21", "%hi", &i);
  s21_sscanf("21", "%hi", &s21_i);
  ck_assert_int_eq(i, s21_i);
}
END_TEST

// sscanf (int_n)
START_TEST(test_sscanf_int_n) {
  int s21_i;
  int i;
  int n;
  int s21_n;
  sscanf("21", "%d%n", &i, &n);
  s21_sscanf("21", "%d%n", &s21_i, &s21_n);
  ck_assert_int_eq(n, s21_n);
}
END_TEST

// sscanf (long_int)
START_TEST(test_sscanf_long_int) {
  long int s21_i;
  long int i;
  sscanf("21", "%li", &i);
  s21_sscanf("21", "%li", &s21_i);
  ck_assert_int_eq(i, s21_i);
}
END_TEST

// sscanf (long_long)
START_TEST(test_sscanf_long_long) {
  long long s21_i;
  long long i;
  sscanf("21", "%lld", &i);
  s21_sscanf("21", "%lld", &s21_i);
  ck_assert_int_eq(i, s21_i);
}
END_TEST

// sscanf (long_long)
START_TEST(test_sscanf_unsigned_long_long) {
  unsigned long long s21_i;
  unsigned long long i;
  sscanf("21", "%llu", &i);
  s21_sscanf("21", "%llu", &s21_i);
  ck_assert_int_eq(i, s21_i);
}
END_TEST

// sscanf (unsigned_short)
START_TEST(test_sscanf_unsigned_short) {
  unsigned short s21_i;
  unsigned short i;
  sscanf("21", "%hu", &i);
  s21_sscanf("21", "%hu", &s21_i);
  ck_assert_int_eq(i, s21_i);
}
END_TEST

// sscanf (unsigned long)
START_TEST(test_sscanf_unsigned_long) {
  unsigned int s21_i;
  unsigned int i;
  sscanf("21", "%u", &i);
  s21_sscanf("21", "%u", &s21_i);
  ck_assert_int_eq(i, s21_i);
}
END_TEST

// sscanf (int float)
START_TEST(test_sscanf_int_float) {
  int s21_i;
  int i;
  float d;
  float s21_d;
  sscanf("21 21.21", "%d %f", &i, &d);
  s21_sscanf("21 21.21", "%d %f", &s21_i, &s21_d);
  ck_assert_int_eq(i, s21_i);
  ck_assert_float_eq(d, s21_d);
}
END_TEST

// sscanf (float)
START_TEST(test_sscanf_float) {
  float s21_i;
  float i;
  sscanf("21.21", "%f", &i);
  s21_sscanf("21.21", "%f", &s21_i);
  ck_assert_float_eq(i, s21_i);
}
END_TEST

// sscanf (double)
START_TEST(test_sscanf_double) {
  double s21_i;
  double i;
  sscanf("21.21", "%lf", &i);
  s21_sscanf("21.21", "%lf", &s21_i);
  ck_assert_double_eq(i, s21_i);
}
END_TEST

// sscanf (L)
START_TEST(test_sscanf_L) {
  long double s21_i;
  long double i;
  sscanf("21.21", "%Lf", &i);
  s21_sscanf("21.21", "%Lf", &s21_i);
  ck_assert_double_eq(i, s21_i);
}
END_TEST

// sscanf (string)
START_TEST(test_sscanf_string) {
  char s21_i[100];
  char i[100];
  sscanf("21.21", "%s", i);
  s21_sscanf("21.21", "%s", s21_i);
  ck_assert_str_eq(i, s21_i);
}
END_TEST

// sscanf (string_2)
START_TEST(test_sscanf_string_2) {
  char s21_i[100];
  char i[100];
  sscanf("21", "%2s", i);
  s21_sscanf("21", "%2s", s21_i);
  ck_assert_str_eq(i, s21_i);
}
END_TEST

// sscanf (string_spaces)
START_TEST(test_sscanf_string_spaces) {
  char s21_i[100];
  char i[100];
  sscanf("abc     ", "%s", i);
  s21_sscanf("abc     ", "%s", s21_i);
  ck_assert_str_eq(i, s21_i);
}
END_TEST

// sscanf (string_percent)
START_TEST(test_sscanf_string_percent) {
  char s21_i[100];
  char i[100];
  sscanf("%%%%%%", "%s", i);
  s21_sscanf("%%%%%%", "%s", s21_i);
  ck_assert_str_eq(i, s21_i);
}
END_TEST

// sscanf (string_minus)
START_TEST(test_sscanf_string_minus) {
  int s21_d;
  int d;
  char s21_i[100];
  char i[100];
  sscanf("21-abc", "%d-%s", &d, i);
  s21_sscanf("21-abc", "%d-%s", &s21_d, s21_i);
  ck_assert_int_eq(d, s21_d);
  ck_assert_str_eq(i, s21_i);
}
END_TEST

// sscanf (string_4)
START_TEST(test_sscanf_string_4) {
  char *buff = "Name is Tom and age is 38";
  char name[10];
  char s21_name[10];
  int age;
  int s21_age;

  sscanf(buff, "Name is %s and age is %d", name, &age);
  s21_sscanf(buff, "Name is %s and age is %d", s21_name, &s21_age);
  ck_assert_int_eq(age, s21_age);
  ck_assert_str_eq(name, s21_name);
}
END_TEST

// sscanf (string_tag)
START_TEST(test_sscanf_string_tag) {
  char str[] = "tag:R123:P1234";
  char r_value[5];
  char p_value[6];
  char s21_r_value[5];
  char s21_p_value[6];
  sscanf(str, "tag:%[^:]:%s", r_value, p_value);
  s21_sscanf(str, "tag:%[^:]:%s", s21_r_value, s21_p_value);
  ck_assert_str_eq(r_value, s21_r_value);
  ck_assert_str_eq(p_value, s21_p_value);
}
END_TEST

// sscanf (char)
START_TEST(test_sscanf_char) {
  char s21_i;
  char i;
  sscanf("s", "%c", &i);
  s21_sscanf("s", "%c", &s21_i);
  ck_assert_int_eq(i, s21_i);
}
END_TEST

// sscanf (unsigned_char)
START_TEST(test_sscanf_unsigned_char) {
  unsigned char s21_i;
  unsigned char i;
  sscanf("s", "%hhu", &i);
  s21_sscanf("s", "%hhu", &s21_i);
  ck_assert_int_eq(i, s21_i);
}
END_TEST

// sscanf (unsigned long_2)
START_TEST(test_sscanf_unsigned_long_2) {
  unsigned long s21_i;
  unsigned long i;
  sscanf("  21", "  %lu", &i);
  s21_sscanf(" 21", "  %lu", &s21_i);
  ck_assert_int_eq(i, s21_i);
}
END_TEST

// sscanf (hex)
START_TEST(test_sscanf_hex) {
  unsigned int s21_i;
  unsigned int i;
  sscanf("0x17", "%x", &i);
  s21_sscanf("0x17", "%x", &s21_i);
  ck_assert_int_eq(i, s21_i);
}
END_TEST

// sscanf (hex_2)
START_TEST(test_sscanf_hex_2) {
  unsigned int s21_i;
  unsigned int i;
  sscanf("0X17", "%x", &i);
  s21_sscanf("0X17", "%x", &s21_i);
  ck_assert_int_eq(i, s21_i);
}
END_TEST

// sscanf (hex_3)
START_TEST(test_sscanf_hex_3) {
  unsigned int s21_i;
  unsigned int i;
  sscanf("0xFF", "  %d", &i);
  s21_sscanf("0xFF", "  %d", &s21_i);
  ck_assert_int_eq(i, s21_i);
}
END_TEST

// sscanf (octal)
START_TEST(test_sscanf_octal) {
  unsigned int s21_i;
  unsigned int i;
  sscanf("0754", "%o", &i);
  s21_sscanf("0754", "%o", &s21_i);
  ck_assert_int_eq(i, s21_i);
}
END_TEST

// sscanf (ptr)
START_TEST(test_sscanf_ptr) {
  char *input = " 0x1a";
  const char *format = "%p";
  void *my_pointer = NULL;
  void *pointer = NULL;

  s21_sscanf(input, format, &my_pointer);
  sscanf(input, format, &pointer);

  ck_assert_ptr_eq(my_pointer, pointer);
}
END_TEST

// sscanf (exp)
START_TEST(test_sscanf_exp) {
  char *input = " 1.234e+03";
  const char *format = "%e";
  double my_ddouble = 0;
  double ddouble = 0;

  s21_sscanf(input, format, &my_ddouble);
  sscanf(input, format, &ddouble);

  ck_assert_double_eq_tol(my_ddouble, ddouble, 6);
}
END_TEST

// sscanf (wrong)
START_TEST(test_sscanf_wrong) {
  char *input = " 1.234e+03";
  const char *format = "%z";
  double my_ddouble = 0;
  double ddouble = 0;

  s21_sscanf(input, format, &my_ddouble);
  sscanf(input, format, &ddouble);

  ck_assert_double_eq_tol(my_ddouble, ddouble, 6);
}
END_TEST

// sscanf (wrong_2)
START_TEST(test_sscanf_wrong_2) {
  char *input = " 1.234e+03";
  const char *format = "%z%";
  double my_ddouble = 0;
  double ddouble = 0;

  s21_sscanf(input, format, &my_ddouble);
  sscanf(input, format, &ddouble);

  ck_assert_double_eq_tol(my_ddouble, ddouble, 6);
}
END_TEST

// sscanf (wrong_3)
START_TEST(test_sscanf_wrong_3) {
  char *input = " 1.234e+03";
  const char *format = "%%";
  double my_ddouble = 0;
  double ddouble = 0;

  s21_sscanf(input, format, &my_ddouble);
  sscanf(input, format, &ddouble);

  ck_assert_double_eq_tol(my_ddouble, ddouble, 6);
}
END_TEST

// to_upper

START_TEST(test_to_upper) {
  char str1[12] = "aB1- ^";
  char *str2 = "AB1- ^";
  char *str3 = s21_to_upper(str1);
  ck_assert_str_eq(str2, str3);
}
END_TEST

START_TEST(test_to_upper_null) {
  char *str1 = NULL;
  char *str2 = s21_to_upper(str1);
  ck_assert_ptr_null(str2);
}
END_TEST

// to_lower

START_TEST(test_to_lower) {
  char str1[12] = "aB1- ^";
  char *str2 = "ab1- ^";
  char *str3 = s21_to_lower(str1);
  ck_assert_str_eq(str2, str3);
}
END_TEST

START_TEST(test_to_lower_null) {
  char *str1 = NULL;
  char *str2 = s21_to_lower(str1);
  ck_assert_ptr_null(str2);
}
END_TEST

// insert

START_TEST(test_insert) {
  char str1[12] = "abc", str2[12] = "def";
  char str3[12] = "abdefc";
  char *str4 = s21_insert(str1, str2, 2);
  ck_assert_str_eq(str3, str4);
}
END_TEST

START_TEST(test_insert_null) {
  char *str1 = NULL, *str2 = NULL;
  char *str3 = s21_insert(str1, str2, 2);
  ck_assert_ptr_null(str3);
}
END_TEST

// trim

START_TEST(test_trim) {
  char str1[12] = " \na- b+c-\n", str2[12] = " \n-";
  char str3[12] = "a- b+c";
  char *str4 = s21_trim(str1, str2);
  ck_assert_str_eq(str3, str4);
}
END_TEST

START_TEST(test_trim_null) {
  char *str1 = NULL, *str2 = NULL;
  char *str3 = s21_trim(str1, str2);
  ck_assert_ptr_null(str3);
}
END_TEST

Suite *string_suite(void) {
  Suite *suite = suite_create("String");
  TCase *tcase = tcase_create("string_functions");

  tcase_add_test(tcase, test_memchr_equal);
  tcase_add_test(tcase, test_memchr_notequal);
  tcase_add_test(tcase, test_memchr_null);

  tcase_add_test(tcase, test_memcmp_equal);
  tcase_add_test(tcase, test_memcmp_gr);

  tcase_add_test(tcase, test_memcpy);
  tcase_add_test(tcase, test_memcpy_overflow);

  tcase_add_test(tcase, test_memset);

  tcase_add_test(tcase, test_strncat);

  tcase_add_test(tcase, test_strchr_found);
  tcase_add_test(tcase, test_strchr_not_found);

  tcase_add_test(tcase, test_strncmp_eq);

  tcase_add_test(tcase, test_strncpy);

  tcase_add_test(tcase, test_strcspn);

  tcase_add_test(tcase, test_strerror);

  tcase_add_test(tcase, test_strlen);

  tcase_add_test(tcase, test_strpbrk_found);
  tcase_add_test(tcase, test_strpbrk_not_found);

  tcase_add_test(tcase, test_strrchr_found);
  tcase_add_test(tcase, test_strrchr_not_found);

  tcase_add_test(tcase, test_strstr_found);
  tcase_add_test(tcase, test_strstr_not_found);

  tcase_add_test(tcase, test_strtok_middle_1);
  tcase_add_test(tcase, test_strtok_middle_2);
  tcase_add_test(tcase, test_strtok_end);

  tcase_add_test(tcase, test_sprintf_int);
  tcase_add_test(tcase, test_sprintf_int_n);
  tcase_add_test(tcase, test_sprintf_unsigned_int);
  tcase_add_test(tcase, test_sprintf_unsigned_int_h);
  tcase_add_test(tcase, test_sprintf_long);
  tcase_add_test(tcase, test_sprintf_unsigned_long_long);
  tcase_add_test(tcase, test_sprintf_int_plus);
  tcase_add_test(tcase, test_sprintf_hex);
  tcase_add_test(tcase, test_sprintf_hex_square);
  tcase_add_test(tcase, test_sprintf_octal);
  tcase_add_test(tcase, test_sprintf_unsigned_int);
  tcase_add_test(tcase, test_sprintf_float);
  tcase_add_test(tcase, test_sprintf_float_2);
  tcase_add_test(tcase, test_sprintf_float_star);
  tcase_add_test(tcase, test_sprintf_float_minus);
  tcase_add_test(tcase, test_sprintf_float_space);
  tcase_add_test(tcase, test_sprintf_float_exp);
  tcase_add_test(tcase, test_sprintf_float_zero);
  tcase_add_test(tcase, test_sprintf_negative_float);
  tcase_add_test(tcase, test_sprintf_char);
  tcase_add_test(tcase, test_sprintf_string);
  tcase_add_test(tcase, test_sprintf_percent);
  tcase_add_test(tcase, test_sprintf_percent_2);

  tcase_add_test(tcase, test_sscanf_int);
  tcase_add_test(tcase, test_sscanf_int_skip);
  tcase_add_test(tcase, test_sscanf_int_negative);
  tcase_add_test(tcase, test_sscanf_int_positive);
  tcase_add_test(tcase, test_sscanf_int_h);
  tcase_add_test(tcase, test_sscanf_int_n);
  tcase_add_test(tcase, test_sscanf_long_int);
  tcase_add_test(tcase, test_sscanf_long_long);
  tcase_add_test(tcase, test_sscanf_unsigned_long_long);
  tcase_add_test(tcase, test_sscanf_unsigned_short);
  tcase_add_test(tcase, test_sscanf_unsigned_long);
  tcase_add_test(tcase, test_sscanf_unsigned_long_2);
  tcase_add_test(tcase, test_sscanf_int_float);
  tcase_add_test(tcase, test_sscanf_float);
  tcase_add_test(tcase, test_sscanf_double);
  tcase_add_test(tcase, test_sscanf_L);
  tcase_add_test(tcase, test_sscanf_string);
  tcase_add_test(tcase, test_sscanf_string_2);
  tcase_add_test(tcase, test_sscanf_string_tag);
  tcase_add_test(tcase, test_sscanf_string_spaces);
  tcase_add_test(tcase, test_sscanf_string_percent);
  tcase_add_test(tcase, test_sscanf_string_minus);
  tcase_add_test(tcase, test_sscanf_string_4);
  tcase_add_test(tcase, test_sscanf_char);
  tcase_add_test(tcase, test_sscanf_unsigned_char);
  tcase_add_test(tcase, test_sscanf_hex);
  tcase_add_test(tcase, test_sscanf_hex_2);
  tcase_add_test(tcase, test_sscanf_hex_3);
  tcase_add_test(tcase, test_sscanf_octal);
  tcase_add_test(tcase, test_sscanf_ptr);
  tcase_add_test(tcase, test_sscanf_exp);
  tcase_add_test(tcase, test_sscanf_wrong);
  tcase_add_test(tcase, test_sscanf_wrong_2);
  tcase_add_test(tcase, test_sscanf_wrong_3);

  tcase_add_test(tcase, test_to_upper);
  tcase_add_test(tcase, test_to_upper_null);

  tcase_add_test(tcase, test_to_lower);
  tcase_add_test(tcase, test_to_lower_null);

  tcase_add_test(tcase, test_insert);
  tcase_add_test(tcase, test_insert_null);

  tcase_add_test(tcase, test_trim);
  tcase_add_test(tcase, test_trim_null);

  suite_add_tcase(suite, tcase);

  return suite;
}

int main(void) {
  int failed = 0;
  Suite *suite = string_suite();
  SRunner *srunner = srunner_create(suite);
  srunner_run_all(srunner, CK_NORMAL);
  failed = srunner_ntests_failed(srunner);
  srunner_free(srunner);
  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
