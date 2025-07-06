/* Implements acos(x) = 2 * atan(sqrt((1-x)/(1+x))) */
#define __IMPL_INTERN_DEC_acos(__dec)                                   \
    void intern_##__dec##_acos(intern_##__dec##_t *result,              \
                               const intern_##__dec##_t *x) {           \
        /* Handle special values: */                                    \
        if (intern_##__dec##_is_nan(x)) {                               \
            *result = intern_##__dec##_nan;                             \
            return;                                                     \
        }                                                               \
        /* |x| > 1: NaN */                                              \
        intern_##__dec##_t absx;                                        \
        intern_##__dec##_abs(&absx, x);                                 \
        if (intern_##__dec##_cmp(&absx, &intern_##__dec##_one) > 0) {   \
            *result = intern_##__dec##_nan;                             \
            return;                                                     \
        }                                                               \
        /* t1 = 1 - x */                                                \
        intern_##__dec##_t t1;                                          \
        intern_##__dec##_sub(&t1, &intern_##__dec##_one, x);            \
                                                                        \
        /* t2 = 1 + x */                                                \
        intern_##__dec##_t t2;                                          \
        intern_##__dec##_add(&t2, &intern_##__dec##_one, x);            \
                                                                        \
        /* r = t1 / t2 */                                               \
        intern_##__dec##_t r;                                           \
        intern_##__dec##_div(&r, &t1, &t2);                             \
                                                                        \
        /* s = sqrt(r) */                                               \
        intern_##__dec##_t s;                                           \
        intern_##__dec##_sqrt(&s, &r);                                  \
                                                                        \
        /* atan(s) */                                                   \
        intern_##__dec##_t atan_s;                                      \
        intern_##__dec##_atan(&atan_s, &s);                             \
                                                                        \
        /* result = 2 * atan_s */                                       \
        intern_##__dec##_mul(result, &intern_##__dec##_two, &atan_s);   \
    }
