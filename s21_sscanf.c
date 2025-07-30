#include <limits.h>
#include <stdarg.h>

#include "s21_string.h"

#define S21_UINT_MAX 4294967295U
#define E_suppressed 1 << 0
#define E_char 1 << 6
#define E_short 1 << 7
#define E_long 1 << 8
#define E_llong 1 << 9
#define E_intmax 1 << 10
#define E_size 1 << 11
#define E_ptrdiff 1 << 12
#define E_intptr 1 << 13
#define E_ldouble 1 << 14
#define E_unsigned 1 << 16
#define E_float 1 << 17
#define E_double 1 << 18
#define EOF -1
#define SIZE_MAX 500
typedef unsigned int uintmax_t;
typedef int intmax_t;
typedef int s21_ptrdiff_t;

#define bool int
#define true 1
#define false 0

int s21_isalpha(int ch) {
  return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) ? 1 : 0;
}

int s21_isalnum(int ch) {
  return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') ||
          (ch >= '0' && ch <= '9'))
             ? 1
             : 0;
}

int s21_isupper(int ch) { return (ch >= 'A' && ch <= 'Z') ? 1 : 0; }

int s21_isspace(int c) {
  if (c == ' ') return true;
  if (c == '\t') return true;

  return false;
}

int s21_tolower(int c) {
  int res;

  if (c >= 'A' && c <= 'Z')
    res = c + 32;
  else
    res = c;

  return res;
}

long int s21_strtol(const char *nptr, char **endptr, int base) {
  const char *s = nptr;
  long int result = 0;
  int sign = 1;

  while (s21_isspace((unsigned char)*s)) {
    s++;
  }

  if (*s == '-') {
    sign = -1;
    s++;
  } else if (*s == '+') {
    s++;
  }

  if (base == 0) {
    if (*s == '0') {
      if (s[1] == 'x' || s[1] == 'X') {
        base = 16;
        s += 2;
      } else {
        base = 8;
        s++;
      }
    } else {
      base = 10;
    }
  } else if (base == 16) {
    if (*s == '0' && (s[1] == 'x' || s[1] == 'X')) {
      s += 2;
    }
  }

  while (1) {
    int digit;
    if (s21_isdigit((unsigned char)*s)) {
      digit = *s - '0';
    } else if (s21_isalpha((unsigned char)*s)) {
      if (s21_isupper((unsigned char)*s))
        digit = *s - 'A' + 10;
      else
        digit = *s - 'a' + 10;
    } else {
      break;
    }

    if (digit >= base) {
      break;
    }

    if (result > (LONG_MAX - digit) / base) {
      result = (sign == 1) ? LONG_MAX : LONG_MIN;
      while (s21_isalnum((unsigned char)*s)) s++;
      break;
    }

    result = result * base + digit;
    s++;
  }

  if (endptr != NULL) {
    *endptr = (char *)s;
  }

  return sign * result;
}

struct status_t {
  int base;
  unsigned int flags;
  unsigned n;
  unsigned i;
  unsigned current;
  unsigned width;
  int prec;

  const char *s;

  va_list arg;
};

static int GET(struct status_t *status) {
  int rc = EOF;
  rc = (*status->s == '\0') ? EOF : (unsigned char)*((status->s)++);
  if (rc != EOF) {
    ++(status->i);
    ++(status->current);
  }
  return rc;
}

static void UNGET(struct status_t *status) {
  --(status->s);
  --(status->i);
  --(status->current);
}

static bool IN_SCANSET(const char *scanlist, const char *end_scanlist, int rc) {
  int previous = -1;
  while (scanlist != end_scanlist) {
    if ((*scanlist == '-') && (previous != -1)) {
      if (++scanlist == end_scanlist) {
        return rc == '-';
      }
      while (++previous <= (unsigned char)*scanlist) {
        if (previous == rc) {
          return true;
        }
      }
      previous = -1;
    } else {
      if (rc == (unsigned char)*scanlist) {
        return true;
      }
      previous = (unsigned char)(*scanlist++);
    }
  }
  return false;
}

const char *s21_scan(const char *spec, struct status_t *status) {
  char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz.";
  int rc;
  const char *orig_spec = spec;
  if (*(++spec) == '%') {
    rc = GET(status);
    switch (rc) {
      case EOF:
        if (status->n == 0) {
          status->n = -1;
        }
        return S21_NULL;
      case '%':
        return ++spec;
      default:
        UNGET(status);
        break;
    }
  }

  status->flags = 0;
  status->base = -1;
  status->current = 0;
  status->width = 0;
  status->prec = 0;

  if (*spec == '*') {
    status->flags |= E_suppressed;
    ++spec;
  }

  char const *prev_spec = spec;
  status->width = (int)s21_strtol(spec, (char **)&spec, 10);
  if (spec == prev_spec) {
    status->width = S21_UINT_MAX;
  }

  switch (*(spec++)) {
    case 'h':
      if (*spec == 'h') {
        status->flags |= E_char;
        ++spec;
      } else {
        status->flags |= E_short;
      }
      break;
    case 'l':
      if (*spec == 'l') {
        status->flags |= E_llong;
        ++spec;
        break;
      }
      if (*spec == 'f') {
        status->flags |= E_double;
        break;
      } else {
        status->flags |= E_long;
      }

      break;
    case 'L':
      if (*spec == 'f') {
        status->flags |= E_ldouble;
      }
      break;
    default:
      --spec;
      break;
  }

  bool value_parsed = false;

  switch (*spec) {
    case 'd':
      status->base = 10;
      break;
    case 'i':
      status->base = 0;
      break;
    case 'o':
      status->base = 8;
      status->flags |= E_unsigned;
      break;
    case 'u':
      status->base = 10;
      status->flags |= E_unsigned;
      break;
    case 'x':
      status->base = 16;
      status->flags |= E_unsigned;
      break;
    case 'f':
      --spec;

      if (*(spec) == 'l') {
        status->flags |= E_double;
        status->base = 10;
      }
      if (*(spec) == 'L') {
        status->flags |= E_ldouble;
        status->base = 10;
      }
      if (*(spec) == '%') {
        status->flags |= E_float;
        status->base = 10;
      }

      break;
    case 'F':
    case 'e':
    case 'E':
    case 'g':
    case 'G':
    case 'a':
    case 'A':
      break;
    case 'c': {
      char *c = va_arg(status->arg, char *);
      if (status->width == SIZE_MAX) {
        status->width = 1;
      }
      while ((status->current < status->width) && ((rc = GET(status)) != EOF)) {
        *(c++) = rc;
        value_parsed = true;
      }
      if (value_parsed) {
        ++status->n;
        return ++spec;
      } else {
        if (status->n == 0) {
          status->n = -1;
        }
        return S21_NULL;
      }
    }
    case 's': {
      char *c = va_arg(status->arg, char *);
      while ((status->current < status->width) && ((rc = GET(status)) != EOF)) {
        if (s21_isspace(rc)) {
          UNGET(status);
          if (value_parsed) {
            *c = '\0';
            ++status->n;
            return ++spec;
          } else {
            return S21_NULL;
          }
        } else {
          value_parsed = true;
          *(c++) = rc;
        }
      }
      if (value_parsed) {
        *c = '\0';
        ++status->n;
        return ++spec;
      } else {
        if (status->n == 0) {
          status->n = -1;
        }
        return S21_NULL;
      }
    }
    case '[': {
      const char *endspec = spec;
      bool negative_scanlist = false;
      if (*(++endspec) == '^') {
        negative_scanlist = true;
        ++endspec;
      }
      spec = endspec;
      do {
        ++endspec;
      } while (*endspec != ']');
      char *c = va_arg(status->arg, char *);
      while ((status->current < status->width) && ((rc = GET(status)) != EOF)) {
        if (negative_scanlist) {
          if (IN_SCANSET(spec, endspec, rc)) {
            UNGET(status);
            break;
          }
        } else {
          if (!IN_SCANSET(spec, endspec, rc)) {
            UNGET(status);
            break;
          }
        }
        value_parsed = true;
        *(c++) = rc;
      }
      if (value_parsed) {
        *c = '\0';
        ++status->n;
        return ++endspec;
      } else {
        if (rc == EOF) {
          status->n = -1;
        }
        return S21_NULL;
      }
    }
    case 'p':
      status->base = 16;
      status->flags |= E_unsigned | E_long;
      break;
    case 'n': {
      int *val = va_arg(status->arg, int *);
      *val = status->i;
      return ++spec;
    }
    default:
      return orig_spec;
  }

  if (status->base != -1) {
    int dp_count = 0;
    bool dp_seen = false;
    uintmax_t value = 0;
    long double value_f = 0;
    bool prefix_parsed = false;
    int sign = 0;
    while ((status->current < status->width) && ((rc = GET(status)) != EOF)) {
      if (s21_isspace(rc)) {
        if (sign) {
          UNGET(status);
          break;
        } else {
          status->current--;
        }
      } else if (!sign) {
        switch (rc) {
          case '-':
            sign = -1;
            break;
          case '+':
            sign = 1;
            break;
          default:
            sign = 1;
            UNGET(status);
            break;
        }
      } else if (!prefix_parsed) {
        prefix_parsed = true;
        if (rc != '0') {
          if (status->base == 0) {
            status->base = 10;
          }
          UNGET(status);
        } else {
          if ((status->current < status->width) &&
              ((rc = GET(status)) != EOF)) {
            if (s21_tolower(rc) == 'x') {
              if ((status->base == 0) || (status->base == 16)) {
                status->base = 16;
              } else {
                UNGET(status);
                value_parsed = true;
              }
            } else {
              UNGET(status);
              if (status->base == 0) {
                status->base = 8;
              }
              value_parsed = true;
            }
          } else {
            value_parsed = true;
            break;
          }
        }
      } else {
        char *digitptr =
            (char *)s21_memchr(digits, s21_tolower(rc), status->base);
        if (rc != '.') {
          if (digitptr == S21_NULL) {
            UNGET(status);
            break;
          } else {
            value *= status->base;
            value += digitptr - digits;

            value_f *= status->base;
            value_f += digitptr - digits;
          }
        } else {
          dp_seen = true;
        }
        if (dp_seen) dp_count++;
        value_parsed = true;
      }
    }
    if (!value_parsed) {
      if ((status->n == 0) && (rc == EOF)) {
        status->n = -1;
      }
      return S21_NULL;
    }
    if (!(status->flags & E_suppressed)) {
      switch (status->flags &
              (E_char | E_short | E_long | E_llong | E_intmax | E_size |
               E_ptrdiff | E_float | E_double | E_ldouble | E_unsigned)) {
        case E_char:
          *(va_arg(status->arg, char *)) = (char)(value * sign);
          break;
        case E_char | E_unsigned:
          *(va_arg(status->arg, unsigned char *)) =
              (unsigned char)(value * sign);
          break;

        case E_short:
          *(va_arg(status->arg, short *)) = (short)(value * sign);
          break;
        case E_short | E_unsigned:
          *(va_arg(status->arg, unsigned short *)) =
              (unsigned short)(value * sign);
          break;

        case 0:
          *(va_arg(status->arg, int *)) = (int)(value * sign);
          break;
        case E_unsigned:
          *(va_arg(status->arg, unsigned int *)) = (unsigned int)(value * sign);
          break;

        case E_long:
          *(va_arg(status->arg, long *)) = (long)(value * sign);
          break;
        case E_long | E_unsigned:
          *(va_arg(status->arg, unsigned long *)) =
              (unsigned long)(value * sign);
          break;

        case E_llong:
          *(va_arg(status->arg, long long *)) = (long long)(value * sign);
          break;
        case E_llong | E_unsigned:
          *(va_arg(status->arg, unsigned long long *)) =
              (unsigned long long)(value * sign);
          break;

        case E_float: {
          int n;
          for (n = 1; n < dp_count; n++) value_f = value_f / 10;
          *(va_arg(status->arg, float *)) = (float)((value_f * sign));
        } break;
        case E_double: {
          int n;
          for (n = 1; n < dp_count; n++) value_f = value_f / 10;
          *(va_arg(status->arg, double *)) = (double)((value_f * sign));
        } break;
        case E_ldouble: {
          int n;
          for (n = 1; n < dp_count; n++) value_f = value_f / 10;
          *(va_arg(status->arg, long double *)) =
              (long double)((value_f * sign));
        } break;
        case E_intmax | E_unsigned:
          *(va_arg(status->arg, uintmax_t *)) = (uintmax_t)(value * sign);
          break;

        case E_size:
          *(va_arg(status->arg, s21_size_t *)) = (s21_size_t)(value * sign);
          break;

        case E_ptrdiff:
          *(va_arg(status->arg, s21_ptrdiff_t *)) =
              (s21_ptrdiff_t)(value * sign);
          break;

        default:
          return S21_NULL;
      }
      ++(status->n);
    }
    return ++spec;
  }
  return S21_NULL;
}

int s21_sscanf(const char *s, const char *format, ...) {
  struct status_t status;
  status.base = 0;
  status.flags = 0;
  status.n = 0;
  status.i = 0;
  status.current = 0;
  status.s = (char *)s;
  status.width = 0;
  status.prec = 0;

  va_list ap;
  va_start(ap, format);
  va_copy(status.arg, ap);

  while (*format != '\0') {
    const char *rc;
    if ((*format != '%') || ((rc = s21_scan(format, &status)) == format)) {
      if (s21_isspace(*format)) {
        while (s21_isspace(*status.s)) {
          ++status.s;
          ++status.i;
        }
      } else {
        if (*status.s != *format) {
          if (*status.s == '\0' && status.n == 0) {
            return EOF;
          }
          return status.n;
        } else {
          ++status.s;
          ++status.i;
        }
      }
      ++format;
    } else {
      if (rc == S21_NULL) {
        if ((*status.s == '\n') && (status.n == 0)) {
          status.n = EOF;
        }
        break;
      }
      format = rc;
    }
  }
  va_end(status.arg);
  return status.n;
}
