#define __IMPL_INTERN_DEC_LOG10(__dec)                                  \
    void intern_##__dec##_log10(intern_##__dec##_t *result,             \
                                const intern_##__dec##_t *x) {          \
        if (x->sign) {                                                  \
            intern_##__dec##_set_nan(result); /* log(negative) = nan */ \
            return;                                                     \
        }                                                               \
                                                                        \
        if (x->coeff==0) {                                              \
            intern_##__dec##_set_inf(result, 1); /* log(0) = -inf */    \
            return;                                                     \
        }                                                               \
                                                                        \
        intern_##__dec##_t b = *x;                                      \
                                                                        \
        normalize_intern_##__dec(&b);                                   \
                                                                        \
        int b_digits = 0;                                               \
        bits_##__dec##_t tmp = b.coeff;                                 \
                                                                        \
        while (tmp>=10) { tmp /= 10; b_digits++; }                      \
                                                                        \
        int exp = b.exponent+b_digits;                                  \
        int digits = I_##__dec##_MAX_DIGITS-1;                          \
        int tmp2 = exp;                                                 \
        while (tmp2) { tmp2 /= 10; digits--; }                          \
                                                                        \
        int coeff_digits = 9; /* 9 = floor(log10(INT64_MAX)) */         \
                                                                        \
        /* now we calculate the log10 in few iteractions, just using powi and then */ \
        /* we move the exponent to the coefficient in a clever way: */  \
        bits_##__dec##_t r_coeff;                                       \
        int sign;                                                       \
        if (exp < 0) {                                                  \
            sign = -1;                                                  \
            r_coeff = -exp;                                             \
        } else {                                                        \
            sign = 1;                                                   \
            r_coeff = exp;                                              \
        }                                                               \
        int r_exponent = 0;                                             \
                                                                        \
        while (coeff_digits != 0) {                                     \
            b.exponent = -b_digits;                                     \
            intern_##__dec##_powi(result, &b, (int)__pow10_##__dec[coeff_digits]); \
            b_digits = 0;                                               \
            tmp = result->coeff;                                        \
            while (tmp>=10) { tmp /= 10; b_digits++; }                  \
            r_coeff = r_coeff*__pow10_##__dec[coeff_digits]+sign*(result->exponent+b_digits); \
            r_exponent -= coeff_digits;                                 \
            digits -= coeff_digits;                                     \
            if (coeff_digits >= digits) {                               \
                coeff_digits = digits;                                  \
            }                                                           \
            b = *result;                                                \
        }                                                               \
                                                                        \
        int r_sign = (sign==1)?0:1;                                     \
                                                                        \
        result->sign = r_sign;                                          \
        result->coeff = r_coeff;                                        \
        result->exponent = r_exponent;                                  \
    }
