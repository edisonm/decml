/* asin(x) = 2 * atan(x / (1 + sqrt(1-x^2)))

   this is often preferred for improved numeric stability near ∣x∣≈1, since
   sqrt(1−x^2) can approach zero.
*/

#define __IMPL_INTERN_DEC_asin(__dec)                                   \
    void intern_##__dec##_asin(intern_##__dec##_t *result, const intern_##__dec##_t *x) { \
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
        /* x^2 */                                                       \
        intern_##__dec##_t xx;                                          \
        intern_##__dec##_mul(&xx, x, x);                                \
        /* 1 - x^2 */                                                   \
        intern_##__dec##_t one_minus_xx;                                \
        intern_##__dec##_sub(&one_minus_xx, &intern_##__dec##_one, &xx); \
        /* sqrt(1 - x^2) */                                             \
        intern_##__dec##_t s;                                           \
        intern_##__dec##_sqrt(&s, &one_minus_xx);                       \
        /* denom = 1 + s */                                             \
        intern_##__dec##_t denom;                                       \
        intern_##__dec##_add(&denom, &intern_##__dec##_one, &s);        \
        /* ratio = x / denom */                                         \
        intern_##__dec##_t ratio;                                       \
        intern_##__dec##_div(&ratio, x, &denom);                        \
        /* t = atan(ratio) */                                           \
        intern_##__dec##_t t;                                           \
        intern_##__dec##_atan(&t, &ratio);                              \
        /* result = 2 * t */                                            \
        intern_##__dec##_mul(result, &intern_##__dec##_two, &t);        \
    }
