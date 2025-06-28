#include <string.h>

#define __IMPL_INTERN_DEC_GET_STR(__dec_t, __min_buf_size, __small_threshold_exp) \
    int intern_##__dec_t##_get_str(const intern_##__dec_t##_t *d, char *buf, size_t bufsize) { \
        if (bufsize < (__min_buf_size)) return -1;                      \
        if (intern_##__dec_t##_is_nan(d)) {                             \
            snprintf(buf, bufsize, "nan");                              \
            return 0;                                                   \
        }                                                               \
        if (intern_##__dec_t##_is_inf(d)) {                             \
            if (d->sign)                                                \
                snprintf(buf, bufsize, "-inf");                         \
            else                                                        \
                snprintf(buf, bufsize, "inf");                          \
            return 0;                                                   \
        }                                                               \
        int sign = d->sign;                                             \
        __uint128_t coeff = d->coeff;                                   \
        int exponent = d->exponent;                                     \
                                                                        \
        char coeff_str[__min_buf_size+__min_buf_size/2] = {0};          \
        int exp_pos = __min_buf_size;                                   \
        char *e = coeff_str + exp_pos;                                  \
        char *p = e;                                                    \
        bool dot = false;                                               \
        int digit, dot_offset = 0;                                      \
                                                                        \
        intern_##__dec_t##_t small_threshold =                          \
            {0, 1, -__small_threshold_exp, DEC_NORMAL};                 \
                                                                        \
        intern_##__dec_t##_t b = *d;                                    \
        b.sign = 0;                                                     \
                                                                        \
        if (intern_##__dec_t##_cmp(&small_threshold, &b) <= 0           \
            && intern_##__dec_t##_cmp(&b, &intern_##__dec_t##_one) < 0) { \
            while (exponent != 0) {                                     \
                digit = (int)(coeff % 10);                              \
                *--p = '0' + digit;                                     \
                coeff /= 10;                                            \
                exponent++;                                             \
            }                                                           \
            *--p = '.';                                                 \
            *--p = '0';                                                 \
            dot = true;                                                 \
        } else {                                                        \
            if (coeff < 10) {                                           \
                *--p = '0' + coeff;                                     \
            } else {                                                    \
                do {                                                    \
                    digit = (int)(coeff % 10);                          \
                    *--p = '0' + digit;                                 \
                    coeff /= 10;                                        \
                    if (!dot) {                                         \
                        dot_offset++;                                   \
                    }                                                   \
                    if (!dot && (exponent != 0) &&                      \
                        (exponent + dot_offset == 0 || coeff < 10)) {   \
                        *--p = '.';                                     \
                        dot = true;                                     \
                        exponent += dot_offset;                         \
                    }                                                   \
                } while (coeff != 0 || (!dot && exponent + dot_offset < 0)); \
            }                                                           \
        }                                                               \
        *e = '\0';                                                      \
        if (sign)                                                       \
            *--p = '-';                                                 \
        if (dot) {                                                      \
            while (*(e - 1) == '0' && (e - 1) != p) {                   \
                e--;                                                    \
            }                                                           \
            if (*(e - 1) == '.' && (e - 1) != p)                        \
                e--;                                                    \
        }                                                               \
        if (exponent == 0) {                                            \
            *e = '\0';                                                  \
        } else {                                                        \
            snprintf(e, sizeof(coeff_str)-exp_pos, "e%d", exponent);    \
        }                                                               \
        strncpy(buf, p, bufsize);                                       \
        buf[bufsize-1] = 0;                                             \
        return 0;                                                       \
    }
