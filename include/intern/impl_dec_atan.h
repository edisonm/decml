#define __IMPL_INTERN_DEC_atan(__dec)                                   \
    void intern_##__dec##_atan(intern_##__dec##_t *result,              \
                               const intern_##__dec##_t *a) {           \
        intern_##__dec##_t b = *a, t, u, v;                             \
        int __bitsm1 = intern_##__dec##_BITS-1;                         \
        int a_sign = a->sign;                                           \
        int b_sign = 0;                                                 \
        bits_##__dec##_t y_coeff = 0;                                   \
        int i = 0;                                                      \
                                                                        \
        b.sign = 0;                                                     \
                                                                        \
        static const intern_##__dec##_t small_threshold = {0, 1, -3, DEC_NORMAL}; \
                                                                        \
        normalize_intern_##__dec(&b);                                   \
                                                                        \
        if (intern_##__dec##_cmp(&b, &small_threshold)<=0) {            \
            /* Use Taylor series: atan(x) ≈ x - x³/3 + x⁵/5 - ... */    \
            intern_##__dec##_t xpow = intern_##__dec##_one,             \
                sum = intern_##__dec##_one, term;                       \
            intern_##__dec##_t denom = intern_##__dec##_one;            \
            intern_##__dec##_mul(&term, a, a);        /* term = x^2 */  \
            int sign = 1;                                               \
            int n;                                                      \
            for (n = 3;; n += 2) {                                      \
                intern_##__dec##_mul(&xpow, &xpow, &term); /* xpow *= x^2 */ \
                intern_##__dec##_t d = denom;                           \
                intern_##__dec##_add(&denom, &d, &intern_##__dec##_two); \
                intern_##__dec##_t this_term;                           \
                intern_##__dec##_div(&this_term, &xpow, &denom);        \
                intern_##__dec##_t tmp;                                 \
                if (sign)                                               \
                    intern_##__dec##_sub(&tmp, &sum, &this_term);       \
                else                                                    \
                    intern_##__dec##_add(&tmp, &sum, &this_term);       \
                /* Early exit: if sum is not changing anymore */        \
                if (intern_##__dec##_cmp(&tmp, &sum)==0)                \
                    break;                                              \
                sum = tmp;                                              \
                sign = !sign;                                           \
            }                                                           \
                                                                        \
            intern_##__dec##_mul(result, a, &sum);                      \
                                                                        \
            return;                                                     \
        }                                                               \
                                                                        \
        for (i = 1;; i++) { /* only up to __bitsm1, otherwise overflow */ \
            intern_##__dec##_abs(&t, &b);                               \
                                                                        \
            if (intern_##__dec##_cmp(&t, &intern_##__dec##_one)==-1) {  \
                /* b = 2*b/(1-b^2) % better for b near 0 */             \
                intern_##__dec##_add(&u, &b,                    &b);    \
                intern_##__dec##_mul(&t, &b,                    &b);    \
                intern_##__dec##_sub(&v, &intern_##__dec##_one, &t);    \
                intern_##__dec##_div(&b, &u,                    &v);    \
            } else {                                                    \
                /* 2/(1/b-b) % works with atan(1) */                    \
                intern_##__dec##_div(&t, &intern_##__dec##_one, &b);    \
                intern_##__dec##_sub(&u, &t,                    &b);    \
                intern_##__dec##_div(&b, &intern_##__dec##_two, &u);    \
            }                                                           \
            /* printf("b="); __print_##__dec(b); printf("\n"); */       \
            y_coeff *= 2;                                               \
                                                                        \
            if (b_sign != b.sign) {                                     \
                y_coeff += b_sign?-1:1;                                 \
                b_sign = b.sign;                                        \
            }                                                           \
            if ((y_coeff > ((bits_##__dec##_t)1<<(__bitsm1-1))-1)       \
                || (b.coeff==0 && b.special==DEC_NORMAL)) {             \
                break;                                                  \
            }                                                           \
        }                                                               \
                                                                        \
        intern_##__dec##_t numerator = {a_sign, y_coeff, 0, DEC_NORMAL}; \
        intern_##__dec##_t divisor1 = {0, (bits_##__dec##_t)(((bits_##__dec##_t)1)<<(__bitsm1)*(i/__bitsm1)), 0, DEC_NORMAL}; \
        intern_##__dec##_t divisor2 = {0, (bits_##__dec##_t)(((bits_##__dec##_t)1)<<(i%__bitsm1)), 0, DEC_NORMAL}; \
                                                                        \
        intern_##__dec##_mul(&t,     &numerator, &intern_##__dec##_pi); \
        intern_##__dec##_div(&b,     &t,         &divisor1);            \
        intern_##__dec##_div(result, &b,         &divisor2);            \
    }
