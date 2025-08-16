#define __IMPL_INTERN_DEC_cbrt(__dec)                                   \
    void intern_##__dec##_cbrt(intern_##__dec##_t *result,              \
                               const intern_##__dec##_t *a) {           \
        /* Handle special values: */                                    \
        if (intern_##__dec##_is_nan(a)) {                               \
            *result = intern_##__dec##_nan;                             \
            return;                                                     \
        }                                                               \
                                                                        \
        if (intern_##__dec##_is_inf(a)) {                               \
            *result = intern_##__dec##_inf;                             \
            return;                                                     \
        }                                                               \
                                                                        \
        bits_##__dec##_t coeff = a->coeff;                              \
                                                                        \
        if (coeff==0) {                                                 \
            /* cbrt(0) = 0 */                                           \
            *result = intern_##__dec##_zero;                            \
            return;                                                     \
        }                                                               \
                                                                        \
        int exp = a->exponent;                                          \
                                                                        \
        normalize_coeff_exp_##__dec(&coeff, &exp);                      \
                                                                        \
        while (exp%3!=0) {                                              \
            coeff /= 10;                                                \
            exp++;                                                      \
        }                                                               \
                                                                        \
        /* Now exp is multiple of 3 */                                  \
        int cbrt_exp = exp / 3;                                         \
                                                                        \
        /* Compute cbrt of coeff using Newton-Raphson */                \
                                                                        \
        bits_##__dec##_t x = 1;                                         \
        while (x*x*x < coeff && x < (bits_##__dec##_t)1<<(I_##__dec##_BITS/3)) { \
            x *= 2;                                                     \
        }                                                               \
                                                                        \
        bits_##__dec##_t x_new;                                         \
                                                                        \
        /* first stage: approximate cbrt of coefficient */              \
        while (1) {                                                     \
            x_new = (2*x+coeff/(x*x))/3;                                \
            if (x_new >= x)                                             \
                break;                                                  \
            x = x_new;                                                  \
        }                                                               \
        /* Initial guess bigger than the actual root */                 \
        /* so that the residual will be positive: */                    \
        x++;                                                            \
                                                                        \
        /* second stage: approximate cbrt of decimal */                 \
        intern_##__dec##_t v, v2, b, t1, t2, t3, v_new;                 \
        v.sign = 0;                                                     \
        v.coeff = x;                                                    \
        v.exponent = cbrt_exp;                                          \
        v.special = DEC_NORMAL;                                         \
        b = *a;                                                         \
        b.sign = 0;                                                     \
        while (1) {                                                     \
            intern_##__dec##_mul(&v2, &v, &v);                          \
            intern_##__dec##_div(&t1, &b, &v2);                         \
            intern_##__dec##_add(&t2, &v, &v);                          \
            intern_##__dec##_add(&t3, &t1, &t2);                        \
            intern_##__dec##_div(&v_new, &t3, &intern_##__dec##_three); \
            if (intern_##__dec##_cmp(&v_new, &v) >= 0)                  \
                break;                                                  \
            v = v_new;                                                  \
        }                                                               \
        *result = v;                                                    \
        result->sign = a->sign;                                         \
    }
