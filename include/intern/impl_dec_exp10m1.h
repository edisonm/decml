#define __INTF_INTERN_DEC_EXP10M1_REC(__dec)                            \
    static void intern_##__dec##_exp10m1_rec(const bits_##__dec##_t coeff, \
                                             const int exp,             \
                                             intern_##__dec##_t *y)

#define __IMPL_INTERN_DEC_EXP10M1_REC(__dec)                            \
    __INTF_INTERN_DEC_EXP10M1_REC(__dec) {                              \
        if (coeff == 0) {                                               \
            *y = intern_##__dec##_zero;                                 \
            return;                                                     \
        }                                                               \
        if (exp == 0) {                                                 \
            intern_##__dec##_t a = {0, 1, coeff, DEC_NORMAL};           \
            intern_##__dec##_sub(y, &a, &intern_##__dec##_one);         \
            return;                                                     \
        }                                                               \
        intern_##__dec##_t p;                                           \
        if (exp < -I_##__dec##_MAX_DIGITS) {                            \
            /* for small numbers, approximate exp10m1 via:*/            \
            /* exp10m1(x) = exp(x*log(10))-1 ~= x*log(10) */            \
            intern_##__dec##_exp10m1_rec(coeff/__pow10_##__dec[-exp-I_##__dec##_MAX_DIGITS], \
                                         -I_##__dec##_MAX_DIGITS, y);   \
            uint64_t res = coeff % __pow10_##__dec[-exp-I_##__dec##_MAX_DIGITS]; \
            if (res==0)                                                 \
                return;                                                 \
            intern_##__dec##_t x = {0, res, exp, DEC_NORMAL};           \
            intern_##__dec##_mul(&p, &x, &intern_##__dec##_log_10);     \
        } else {                                                        \
            intern_##__dec##_exp10m1_rec(coeff/10, exp+1, y);           \
            int digit = coeff % 10;                                     \
            if (digit==0)                                               \
                return;                                                 \
            p = pow10m1_##__dec[-exp-1][digit-1];                       \
        }                                                               \
                                                                        \
        intern_##__dec##_t a, b;                                        \
        intern_##__dec##_add(&a,  y, &intern_##__dec##_one); /* 1+y */  \
        intern_##__dec##_mul(&b, &a, &p); /* p*(1+y) */                 \
        intern_##__dec##_add(&a,  y, &b); /* y+p*(1+y) */               \
        *y = a;                                                         \
    }

#define __IMPL_INTERN_DEC_EXP10M1(__dec)                                \
    void intern_##__dec##_exp10m1(intern_##__dec##_t *result, const intern_##__dec##_t *x) { \
        static intern_##__dec##_t fmax = {0, UINT_##__dec##_MAX, 0, DEC_NORMAL}; \
        static intern_##__dec##_t feps = {0, 0x1ULL, -20, DEC_NORMAL};  \
        static intern_##__dec##_t fmin = {1, I_##__dec##_MAX_DIGITS+1, 0, DEC_NORMAL}; \
                                                                        \
        if (intern_##__dec##_cmp(x, &fmin) <=0) {                       \
            *result = intern_##__dec##_one;                             \
            result->sign = 1;                                           \
            return;                                                     \
        }                                                               \
        if (x->sign) {                                                  \
            intern_##__dec##_t a, b;                                    \
            intern_##__dec##_exp10m1_rec(x->coeff, x->exponent, result); /* expm1(x) */ \
            intern_##__dec##_add(&a, result, &intern_##__dec##_one); /* 1+expm1(x) */ \
            intern_##__dec##_div(&b, result, &a); /* expm1(x) / (1+expm1(x)) */ \
            *result = b;                                                \
            result->sign = x->sign; /* expm1(-x) == -expm1(x) / (1+expm1(x)) */ \
            return;                                                     \
        }                                                               \
        if (intern_##__dec##_cmp(x, &fmax) >= 0) {                      \
            intern_##__dec##_set_inf(result, 0);                        \
            return;                                                     \
        }                                                               \
        if (intern_##__dec##_cmp(x, &feps) <= 0) {                      \
            intern_##__dec##_mul(result, x, &intern_##__dec##_log_10);  \
            return;                                                     \
        }                                                               \
        intern_##__dec##_exp10m1_rec(x->coeff, x->exponent, result);    \
    }

#define __IMPL_INTERN_DEC_EXPM1(__dec)                                  \
    void intern_##__dec##_expm1(intern_##__dec##_t *result, const intern_##__dec##_t *x) { \
        intern_##__dec##_t y;                                           \
        intern_##__dec##_mul(&y, x, &intern_##__dec##_log10_e);         \
        intern_##__dec##_exp10m1(result, &y);                           \
    }

#define __IMPL_INTERN_DEC_SINH(__dec)                                   \
    void intern_##__dec##_sinh(intern_##__dec##_t *result, const intern_##__dec##_t *x) { \
        intern_##__dec##_t y, z, a, b, c, d;                            \
        intern_##__dec##_abs(&z, x);                                    \
        intern_##__dec##_expm1(&y, &z);                                 \
        intern_##__dec##_add(&a, &y, &intern_##__dec##_one); /* y+1 */  \
        intern_##__dec##_add(&b, &a, &intern_##__dec##_one); /* y+2 */  \
        intern_##__dec##_add(&c, &a, &a); /* 2*(y+1) */                 \
        intern_##__dec##_div(&d, &b, &c); /* (y+2)/(2*(y+1)) */         \
        intern_##__dec##_mul(result, &y, &d);                           \
        result->sign = x->sign;                                         \
    }

#define __IMPL_INTERN_DEC_COSH(__dec)                                   \
    void intern_##__dec##_cosh(intern_##__dec##_t *result, const intern_##__dec##_t *x) { \
        intern_##__dec##_t y, z, a, b, c, d;                            \
        intern_##__dec##_abs(&z, x);                                    \
        intern_##__dec##_expm1(&y, &z);                                 \
        intern_##__dec##_add(&a, &y, &intern_##__dec##_one); /* y+1 */  \
        intern_##__dec##_add(&b, &a, &a); /* 2*(y+1) */                 \
        intern_##__dec##_div(&c, &y, &b); /* y/(2*(y+1)) */             \
        intern_##__dec##_mul(&d, &y, &c); /* y^2/(2*(y+1)) */           \
        intern_##__dec##_add(result, &d, &intern_##__dec##_one); /* 1+y^2/(2*(y+1)) */ \
    }

#define __IMPL_INTERN_DEC_TANH(__dec)                                   \
    void intern_##__dec##_tanh(intern_##__dec##_t *result, const intern_##__dec##_t *x) { \
        static intern_##__dec##_t feps = {0, 1, -(I_##__dec##_MAX_DIGITS/6+1), DEC_NORMAL}; \
        intern_##__dec##_t y, z, a, b, c, d;                            \
        intern_##__dec##_abs(&z, x);                                    \
                                                                        \
        /* If small enough, use 3 terms of tahn(x)'s Tailor series: */  \
        if (intern_##__dec##_cmp(&z, &feps) <=0) {                      \
            static intern_##__dec##_t k_15_2 = {0, 75, -1, DEC_NORMAL}; \
            static intern_##__dec##_t k_3 = {0, 3, 0, DEC_NORMAL};      \
            intern_##__dec##_mul(&a, &z, &z); /* x^2 */                 \
            intern_##__dec##_mul(&b, &a, &a); /* x^4 */                 \
            intern_##__dec##_div(&c, &b, &k_15_2); /* (2/15)**x^4 */    \
            intern_##__dec##_div(&d, &a, &k_3);  /* (1/3)**x^2 */       \
            intern_##__dec##_sub(&a, &c, &d);  /* (1/3)*x^2 */          \
            intern_##__dec##_add(&b, &a, &intern_##__dec##_one);        \
            intern_##__dec##_mul(result, x, &b);                        \
            return;                                                     \
        }                                                               \
                                                                        \
        /* if bigger than ||MAX_DIGITS * log(10)/2+1||, use sign(x): */ \
        static intern_##__dec##_t fmax = {0, (11513*I_##__dec##_MAX_DIGITS/10000)+1, 0, DEC_NORMAL}; \
        if (intern_##__dec##_cmp(&z, &fmax) >=0) {                      \
            *result = intern_##__dec##_one;                             \
            result->sign = x->sign;                                     \
            return;                                                     \
        }                                                               \
                                                                        \
        /* use y = e(x)-1, y*(y+2)/(y*(y+2)+2) */                       \
        intern_##__dec##_expm1(&y, &z);                                 \
        intern_##__dec##_add(&a, &y, &intern_##__dec##_two); /* y+2 */  \
        intern_##__dec##_mul(&b, &y, &a); /* y*(y+2) */                 \
        intern_##__dec##_add(&c, &b, &intern_##__dec##_two); /* y*(y+2)+2 */ \
        intern_##__dec##_div(&d, &a, &c); /* (y+2)/(y*(y+2)+2) */       \
        intern_##__dec##_mul(result, &y, &d); /* y*(y+2)/(y*(y+2)+2) */ \
        result->sign = x->sign;                                         \
    }
