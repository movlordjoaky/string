#include <stdarg.h>

#ifndef S21_STRING_H_
#include "s21_string.h"
#define WIDTH_UNSET -1
#define PRECISION_UNSET -1
#define ALIGN_UNSET 0
#define ALIGN_RIGHT 1
#define ALIGN_LEFT 2
#define INT_LEN_DEFAULT 0
#define INT_LEN_LONG 1
#define INT_LEN_LONG_LONG 2
#define INT_LEN_SHORT 3
#define INT_LEN_CHAR 4
#define S21_STRING_H_

#define ROUND_TO_PRECISION(d, p)                    \
  ((d < 0.) ? d - s21_pow_10(-(p)->precision) * 0.5 \
            : d + s21_pow_10(-(p)->precision) * 0.5)

#define PUT_CHAR(c, p)               \
  if ((p)->counter < (p)->ps_size) { \
    if ((p)->ps != S21_NULL) {       \
      *(p)->ps++ = (c);              \
    }                                \
    (p)->counter++;                  \
  }

#define PUT_PLUS(d, p)                        \
  if ((d) > 0 && (p)->align == ALIGN_RIGHT) { \
    PUT_CHAR('+', p);                         \
  }

#define PUT_SPACE(d, p)           \
  if ((p)->is_space && (d) > 0) { \
    PUT_CHAR(' ', p);             \
  }

#define PAD_RIGHT(p)                                \
  if ((p)->width > 0 && (p)->align != ALIGN_LEFT) { \
    for (; (p)->width > 0; (p)->width--) {          \
      PUT_CHAR((p)->pad, p);                        \
    }                                               \
  }

#define PAD_LEFT(p)                                 \
  if ((p)->width > 0 && (p)->align == ALIGN_LEFT) { \
    for (; (p)->width > 0; (p)->width--) {          \
      PUT_CHAR((p)->pad, p);                        \
    }                                               \
  }

#define WIDTH_AND_PRECISION_ARGS(p)     \
  if ((p)->is_star_w) {                 \
    (p)->width = va_arg(args, int);     \
  }                                     \
  if ((p)->is_star_p) {                 \
    (p)->precision = va_arg(args, int); \
  }

#define INTEGER_ARG(p, type, ll)                  \
  WIDTH_AND_PRECISION_ARGS(p);                    \
  if ((p)->a_long == INT_LEN_LONG_LONG) {         \
    ll = (long long)va_arg(args, type long long); \
  } else if ((p)->a_long == INT_LEN_LONG) {       \
    ll = (long long)va_arg(args, type long);      \
  } else {                                        \
    type int a = va_arg(args, type int);          \
    if ((p)->a_long == INT_LEN_SHORT) {           \
      ll = (type short)a;                         \
    } else if ((p)->a_long == INT_LEN_CHAR) {     \
      ll = (type char)a;                          \
    } else {                                      \
      ll = a;                                     \
    }                                             \
  }

#define DOUBLE_ARG(p, d)                   \
  WIDTH_AND_PRECISION_ARGS(p);             \
  if ((p)->precision == PRECISION_UNSET) { \
    (p)->precision = 6;                    \
  }                                        \
  d = va_arg(args, double);

#endif
struct DATA {
  s21_size_t counter;
  s21_size_t ps_size;
  char *ps;
  const char *pf;
  int width;
  int precision;
  unsigned int align : 2;
  unsigned int is_square : 1;
  unsigned int is_space : 1;
  unsigned int is_dot : 1;
  unsigned int is_star_w : 1;
  unsigned int is_star_p : 1;
  unsigned int a_long : 3;
  char pad;
};

int s21_toupper(int c) {
  int res;

  if (c >= 'a' && c <= 'z')
    res = c - 32;
  else
    res = c;

  return res;
}

int s21_isdigit(char c) {
  if (c >= '0' && c <= '9') {
    return 1;
  }
  return 0;
}

static int s21_strtoi(const char *a, int *res) {
  int i = 0;

  *res = 0;

  for (; a[i] != '\0' && s21_isdigit(a[i]); i++) {
    *res = *res * 10 + (a[i] - '0');
  }

  return i;
}

static void s21_inttoa(long long number, int is_signed, int precision, int base,
                       char *output, s21_size_t output_size) {
  s21_size_t i = 0, j;

  output_size--;

  if (number != 0) {
    unsigned long long n;

    if (is_signed && number < 0) {
      n = (unsigned long long)-number;
      output_size--;
    } else {
      n = (unsigned long long)number;
    }

    while (n != 0 && i < output_size) {
      int r = (int)(n % (unsigned long long)(base));
      output[i++] = (char)r + (r < 10 ? '0' : 'a' - 10);
      n /= (unsigned long long)(base);
    }

    if (precision > 0) {
      for (; i < (s21_size_t)precision && i < output_size; i++) {
        output[i] = '0';
      }
    }

    if (is_signed && number < 0) {
      output[i++] = '-';
    }

    output[i] = '\0';

    for (i--, j = 0; j < i; j++, i--) {
      char tmp = output[i];
      output[i] = output[j];
      output[j] = tmp;
    }
  } else {
    precision = precision < 0 ? 1 : precision;
    for (i = 0; i < (s21_size_t)precision && i < output_size; i++) {
      output[i] = '0';
    }
    output[i] = '\0';
  }
}

static double s21_pow_10(int n) {
  int i = 1;
  double p = 1., m;

  if (n < 0) {
    n = -n;
    m = .1;
  } else {
    m = 10.;
  }

  for (; i <= n; i++) {
    p *= m;
  }

  return p;
}

static int s21_log_10(double r) {
  int i = 0;
  double result = 1.;

  if (r == 0.) {
    return 0;
  }

  if (r < 0.) {
    r = -r;
  }

  if (r < 1.) {
    for (; result >= r; i++) {
      result *= .1;
    }

    i = -i;
  } else {
    for (; result <= r; i++) {
      result *= 10.;
    }

    --i;
  }

  return i;
}

static double s21_integral(double real, double *ip) {
  int log;
  double real_integral = 0.;

  if (real == 0.) {
    *ip = 0.;
    return 0.;
  }

  if (real < 0.) {
    real = -real;
  }

  if (real < 1.) {
    *ip = 0.;
    return real;
  }

  for (log = s21_log_10(real); log >= 0; log--) {
    double i = 0., p = s21_pow_10(log);
    double s = (real - real_integral) / p;
    for (; i + 1. <= s; i++) {
    }
    real_integral += i * p;
  }

  *ip = real_integral;
  return (real - real_integral);
}

#define MAX_INTEGRAL_SIZE (99 + 1)
#define MAX_FRACTION_SIZE (29 + 1)
#define PRECISION (1.e-6)

static void s21_floattoa(double number, int precision, char *output_integral,
                         s21_size_t output_integral_size, char *output_fraction,
                         s21_size_t output_fraction_size) {
  s21_size_t i, j;
  int is_negative = 0;
  double ip, fp;
  double fraction;

  if (number == 0.) {
    output_integral[0] = output_fraction[0] = '0';
    output_integral[1] = output_fraction[1] = '\0';

    return;
  }

  if (number < 0.) {
    number = -number;
    is_negative = 1;
    output_integral_size--;
  }

  fraction = s21_integral(number, &ip);
  number = ip;

  if (ip == 0.) {
    output_integral[0] = '0';
    i = 1;
  } else {
    for (i = 0; i < output_integral_size - 1 && number != 0.; ++i) {
      number /= 10;

      output_integral[i] =
          (char)((s21_integral(number, &ip) + PRECISION) * 10) + '0';
      if (!s21_isdigit(output_integral[i])) {
        break;
      }
      number = ip;
    }
  }

  if (number != 0.) {
    for (i = 0; i < output_integral_size - 1; ++i) {
      output_integral[i] = '9';
    }
  }

  if (is_negative) {
    output_integral[i++] = '-';
  }

  output_integral[i] = '\0';

  for (i--, j = 0; j < i; j++, i--) {
    char tmp = output_integral[i];
    output_integral[i] = output_integral[j];
    output_integral[j] = tmp;
  }

  for (i = 0, fp = fraction; precision > 0 && i < output_fraction_size - 1;
       i++, precision--) {
    output_fraction[i] = (char)(int)((fp + PRECISION) * 10. + '0');
    if (!s21_isdigit(output_fraction[i])) {
      break;
    }

    fp = (fp * 10.0) - (double)(long)((fp + PRECISION) * 10.);
  }
  output_fraction[i] = '\0';
}

static void s21_decimal(struct DATA *p, long long ll) {
  char number[MAX_INTEGRAL_SIZE], *pnumber = number;
  s21_inttoa(ll, *p->pf == 'i' || *p->pf == 'd', p->precision, 10, number,
             sizeof(number));

  p->width -= s21_strlen(number);
  PAD_RIGHT(p);

  PUT_PLUS(ll, p);
  PUT_SPACE(ll, p);

  for (; *pnumber != '\0'; pnumber++) {
    PUT_CHAR(*pnumber, p);
  }

  PAD_LEFT(p);
}

static void s21_octal(struct DATA *p, long long ll) {
  char number[MAX_INTEGRAL_SIZE], *pnumber = number;
  s21_inttoa(ll, 0, p->precision, 8, number, sizeof(number));

  p->width -= s21_strlen(number);
  PAD_RIGHT(p);

  if (p->is_square && *number != '\0') {
    PUT_CHAR('0', p);
  }

  for (; *pnumber != '\0'; pnumber++) {
    PUT_CHAR(*pnumber, p);
  }

  PAD_LEFT(p);
}

static void s21_hex(struct DATA *p, long long ll) {
  char number[MAX_INTEGRAL_SIZE], *pnumber = number;
  s21_inttoa(ll, 0, p->precision, 16, number, sizeof(number));

  p->width -= s21_strlen(number);
  PAD_RIGHT(p);

  if (p->is_square && *number != '\0') {
    PUT_CHAR('0', p);
    PUT_CHAR(*p->pf == 'p' ? 'x' : *p->pf, p);
  }

  for (; *pnumber != '\0'; pnumber++) {
    PUT_CHAR((*p->pf == 'X' ? (char)s21_toupper(*pnumber) : *pnumber), p);
  }

  PAD_LEFT(p);
}

static void s21_strings(struct DATA *p, char *s) {
  int len = (int)s21_strlen(s);
  if (p->precision != PRECISION_UNSET && len > p->precision) {
    len = p->precision;
  }

  p->width -= len;

  PAD_RIGHT(p);

  for (; len-- > 0; s++) {
    PUT_CHAR(*s, p);
  }

  PAD_LEFT(p);
}

static void s21_floating(struct DATA *p, double d) {
  char integral[MAX_INTEGRAL_SIZE], *pintegral = integral;
  char fraction[MAX_FRACTION_SIZE], *pfraction = fraction;

  d = ROUND_TO_PRECISION(d, p);
  s21_floattoa(d, p->precision, integral, sizeof(integral), fraction,
               sizeof(fraction));

  if (d > 0. && p->align == ALIGN_RIGHT) {
    p->width -= 1;
  }
  p->width -= p->is_space + (int)s21_strlen(integral) + p->precision + 1;
  if (p->precision == 0) {
    p->width += 1;
  }

  PAD_RIGHT(p);
  PUT_PLUS(d, p);
  PUT_SPACE(d, p);

  for (; *pintegral != '\0'; pintegral++) {
    PUT_CHAR(*pintegral, p);
  }

  if (p->precision != 0 || p->is_square) {
    PUT_CHAR('.', p);
  }

  if (*p->pf == 'g' || *p->pf == 'G') {
    s21_size_t i;
    for (i = s21_strlen(fraction); i > 0 && fraction[i - 1] == '0'; i--) {
      fraction[i - 1] = '\0';
    }
  }

  for (; *pfraction != '\0'; pfraction++) {
    PUT_CHAR(*pfraction, p);
  }

  PAD_LEFT(p);
}

static void s21_exponent(struct DATA *p, double d) {
  char integral[MAX_INTEGRAL_SIZE], *pintegral = integral;
  char fraction[MAX_FRACTION_SIZE], *pfraction = fraction;
  int log = s21_log_10(d);
  d /= s21_pow_10(log);
  d = ROUND_TO_PRECISION(d, p);

  s21_floattoa(d, p->precision, integral, sizeof(integral), fraction,
               sizeof(fraction));

  if (d > 0. && p->align == ALIGN_RIGHT) {
    p->width -= 1;
  }
  p->width -= p->is_space + p->precision + 7;

  PAD_RIGHT(p);
  PUT_PLUS(d, p);
  PUT_SPACE(d, p);

  for (; *pintegral != '\0'; pintegral++) {
    PUT_CHAR(*pintegral, p);
  }

  if (p->precision != 0 || p->is_square) {
    PUT_CHAR('.', p);
  }

  if (*p->pf == 'g' || *p->pf == 'G') {
    s21_size_t i;
    for (i = s21_strlen(fraction); i > 0 && fraction[i - 1] == '0'; i--) {
      fraction[i - 1] = '\0';
    }
  }
  for (; *pfraction != '\0'; pfraction++) {
    PUT_CHAR(*pfraction, p);
  }

  if (*p->pf == 'g' || *p->pf == 'e') {
    PUT_CHAR('e', p);
  } else {
    PUT_CHAR('E', p);
  }

  if (log >= 0) {
    PUT_CHAR('+', p);
  }

  s21_inttoa(log, 1, 2, 10, integral, sizeof(integral));
  for (pintegral = integral; *pintegral != '\0'; pintegral++) {
    PUT_CHAR(*pintegral, p);
  }

  PAD_LEFT(p);
}

static void s21_conv_flags(struct DATA *p) {
  p->width = WIDTH_UNSET;
  p->precision = PRECISION_UNSET;
  p->is_star_w = p->is_star_p = 0;
  p->is_square = p->is_space = 0;
  p->a_long = INT_LEN_DEFAULT;
  p->align = ALIGN_UNSET;
  p->pad = ' ';
  p->is_dot = 0;

  for (; p != S21_NULL && p->pf != S21_NULL; p->pf++) {
    switch (*p->pf) {
      case ' ':
        p->is_space = 1;
        break;

      case '#':
        p->is_square = 1;
        break;

      case '*':
        if (p->width == WIDTH_UNSET) {
          p->width = 1;
          p->is_star_w = 1;
        } else {
          p->precision = 1;
          p->is_star_p = 1;
        }
        break;

      case '+':
        p->align = ALIGN_RIGHT;
        break;

      case '-':
        p->align = ALIGN_LEFT;
        break;

      case '.':
        if (p->width == WIDTH_UNSET) {
          p->width = 0;
        }
        p->is_dot = 1;
        break;

      case '0':
        p->pad = '0';
        if (p->is_dot) {
          p->precision = 0;
        }
        break;

      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        p->pf += s21_strtoi(p->pf, p->width == WIDTH_UNSET ? &p->width
                                                           : &p->precision) -
                 1;
        break;

      case '%':
        return;

      default:
        p->pf--;
        return;
    }
  }
}

int s21_vsnprintf(char *string, s21_size_t length, const char *format,
                  va_list args) {
  struct DATA data;

  if (string == S21_NULL) {
    length = __SIZE_MAX__;
  } else if (length < 1) {
    return -1;
  }

  data.ps_size = length - 1;
  data.ps = string;
  data.pf = format;
  data.counter = 0;

  for (; *data.pf != '\0' && (data.counter < data.ps_size); data.pf++) {
    if (*data.pf == '%') {
      int is_continue = 1;
      s21_conv_flags(&data);
      while (*data.pf != '\0' && is_continue) {
        switch (*(++data.pf)) {
          case '\0':
            PUT_CHAR('%', &data);
            if (data.ps != S21_NULL) {
              *data.ps = '\0';
            }
            return (int)data.counter;

          case 'f':
          case 'F': {
            double d;
            DOUBLE_ARG(&data, d);
            s21_floating(&data, d);
            is_continue = 0;
            break;
          }

          case 'e':
          case 'E': {
            double d;
            DOUBLE_ARG(&data, d);
            s21_exponent(&data, d);
            is_continue = 0;
            break;
          }

          case 'g':
          case 'G': {
            int log;
            double d;
            DOUBLE_ARG(&data, d);
            log = s21_log_10(d);
            if (-4 < log && log < data.precision) {
              s21_floating(&data, d);
            } else {
              s21_exponent(&data, d);
            }
            is_continue = 0;
            break;
          }

          case 'u': {
            long long ll;
            INTEGER_ARG(&data, unsigned, ll);
            s21_decimal(&data, ll);
            is_continue = 0;
            break;
          }

          case 'i':
          case 'd': {
            long long ll;
            INTEGER_ARG(&data, signed, ll);
            s21_decimal(&data, ll);
            is_continue = 0;
            break;
          }

          case 'o': {
            long long ll;
            INTEGER_ARG(&data, unsigned, ll);
            s21_octal(&data, ll);
            is_continue = 0;
            break;
          }

          case 'x':
          case 'X': {
            long long ll;
            INTEGER_ARG(&data, unsigned, ll);
            s21_hex(&data, ll);
            is_continue = 0;
            break;
          }

          case 'c': {
            int i = va_arg(args, int);
            PUT_CHAR((char)i, &data);
            is_continue = 0;
            break;
          }

          case 's':
            WIDTH_AND_PRECISION_ARGS(&data);
            s21_strings(&data, va_arg(args, char *));
            is_continue = 0;
            break;

          case 'p': {
            void *v = va_arg(args, void *);
            data.is_square = 1;
            if (v == S21_NULL) {
              s21_strings(&data, "(nil)");
            } else {
              s21_hex(&data, (long long)v);
            }
            is_continue = 0;
            break;
          }

          case 'n':
            *(va_arg(args, int *)) = (int)data.counter;
            is_continue = 0;
            break;

          case 'l':
            if (data.a_long == INT_LEN_LONG) {
              data.a_long = INT_LEN_LONG_LONG;
            } else {
              data.a_long = INT_LEN_LONG;
            }
            break;

          case 'h':
            if (data.a_long == INT_LEN_SHORT) {
              data.a_long = INT_LEN_CHAR;
            } else {
              data.a_long = INT_LEN_SHORT;
            }
            break;

          case '%':
            PUT_CHAR('%', &data);
            is_continue = 0;
            break;

          case '#':
          case ' ':
          case '+':
          case '*':
          case '-':
          case '.':
          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
            s21_conv_flags(&data);
            break;

          default:
            PUT_CHAR('%', &data);
            is_continue = 0;
            break;
        }
      }
    } else {
      PUT_CHAR(*data.pf, &data);
    }
  }

  if (data.ps != S21_NULL) {
    *data.ps = '\0';
  }

  return (int)data.counter;
}

int s21_sprintf(char *str, const char *format, ...) {
  int rval;
  va_list args;

  va_start(args, format);
  rval = s21_vsnprintf(str, __SIZE_MAX__, format, args);
  va_end(args);

  return rval;
}
