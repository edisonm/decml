/* Helper: reduce x mod 2*pi into [-pi, pi] */
#define __IMPL_INTERN_DEC_MOD_2PI(__dec)                                \
    static void intern_##__dec##_mod_2pi(intern_##__dec##_t* r, const intern_##__dec##_t* x) { \
        /* r = x - n*2pi,  n = round(x/(2pi)) */                        \
        intern_##__dec##_t two_pi;                                      \
        intern_##__dec##_mul(&two_pi, &intern_##__dec##_two, &intern_##__dec##_pi); \
                                                                        \
        /* n = nearest integer(x/(2pi)) */                              \
        intern_##__dec##_t n_real;                                      \
        intern_##__dec##_div(&n_real, x, &two_pi);                      \
                                                                        \
        /* Truncate n_real to nearest integer (no rounding function exposed, so truncate towards zero) */ \
        int n = 0;                                                      \
        if (!intern_##__dec##_is_nan(&n_real) && !intern_##__dec##_is_inf(&n_real)) { \
            /* n_real.coeff has absolute integer value, */              \
            /* n_real.sign = +1 or -1 */                                \
            n = n_real.sign * (int)n_real.coeff;                        \
        }                                                               \
        /* r = x - n * 2pi */                                           \
        intern_##__dec##_t n_two_pi;                                    \
        if (n == 0) {                                                   \
            *r = *x;                                                    \
        } else {                                                        \
            intern_##__dec##_t n_val = { n < 0 ? -1 : 1, (uint64_t)(n < 0 ? -n : n), 0, DEC_NORMAL }; \
            intern_##__dec##_mul(&n_two_pi, &n_val, &two_pi);           \
            intern_##__dec##_sub(r, x, &n_two_pi);                      \
        }                                                               \
    }

/* Main: Calculate sin(x) using Taylor series, argument reduction. */
#define __IMPL_INTERN_DEC_SIN(__dec)                                    \
    void intern_##__dec##_sin(intern_##__dec##_t *result, const intern_##__dec##_t *x) { \
        /* Handle special values */                                     \
        if (intern_##__dec##_is_nan(x) || intern_##__dec##_is_inf(x)) { \
            *result = intern_##__dec##_nan;                             \
            return;                                                     \
        }                                                               \
                                                                        \
        /* Argument reduction: bring x into [-pi, pi] */                \
        intern_##__dec##_t xred, ab_xred;                               \
        intern_##__dec##_mod_2pi(&xred, x);                             \
        intern_##__dec##_abs(&ab_xred, &xred);                          \
                                                                        \
        /* If |x| > pi/2, sin(x) = sin(pi - x) (for |x| in [pi/2, pi]) */ \
        intern_##__dec##_t pi_over_two;                                 \
        intern_##__dec##_div(&pi_over_two, &intern_##__dec##_pi, &intern_##__dec##_two); \
                                                                        \
        intern_##__dec##_t y;                                           \
        if (intern_##__dec##_cmp(&ab_xred, &pi_over_two) > 0) {         \
            /* y = pi - |xred|, use sign of xred */                     \
            intern_##__dec##_t tmp;                                     \
            intern_##__dec##_sub(&tmp, &intern_##__dec##_pi, &ab_xred); \
            y = tmp;                                                    \
            y.sign = xred.sign;                                         \
        } else {                                                        \
            y = xred;                                                   \
        }                                                               \
                                                                        \
        /* Sine Taylor series: sum_{k=0}^{N} (-1)^k y^{2k+1} / (2k+1)! */ \
        /* We'll use up to k=7 (order 15, which is usually plenty for 16 decimal digits) */ \
        intern_##__dec##_t term, sum, tmp;                              \
        sum.sign = 0; sum.coeff = 0; sum.exponent = 0; sum.special = DEC_NORMAL; \
                                                                        \
        /* Precompute y^n */                                            \
        /* y_pow = y^(2k+1) */                                          \
        intern_##__dec##_t y_pow = y;                                   \
                                                                        \
        for (int k = 0;; ++k) {                                         \
            /* exponent = 2k+1 */                                       \
            int n = 2*k+1;                                              \
            bits_##__dec##_t fact = factorial(n);                       \
                                                                        \
            /* denom = fact */                                          \
            intern_##__dec##_t denom = {1, fact, 0, DEC_NORMAL};        \
                                                                        \
            /* term = y_pow / denom */                                  \
            intern_##__dec##_div(&term, &y_pow, &denom);                \
            if (k % 2 == 1) {                                           \
                /* Negative sign */                                     \
                term.sign = -term.sign;                                 \
            }                                                           \
            /* sum = sum + term */                                      \
            intern_##__dec##_add(&tmp, &sum, &term);                    \
                                                                        \
            if (intern_##__dec##_cmp(&tmp, &sum)==0)                    \
                break;                                                  \
                                                                        \
            sum = tmp;                                                  \
            /* Prepare y_pow *= y*y for next (2(k+1)+1) */              \
            /* y_pow *= y*y */                                          \
            intern_##__dec##_t y2;                                      \
            intern_##__dec##_mul(&y2, &y, &y);                          \
            intern_##__dec##_mul(&y_pow, &y_pow, &y2);                  \
        }                                                               \
                                                                        \
        *result = sum;                                                  \
    }

// Main: Calculate cos(x) using Taylor series, argument reduction
#define __IMPL_INTERN_DEC_COS(__dec)                                    \
    void intern_##__dec##_cos(intern_##__dec##_t *result, const intern_##__dec##_t *x) { \
        /* Handle special values */                                     \
        if (intern_##__dec##_is_nan(x) || intern_##__dec##_is_inf(x)) { \
            *result = intern_##__dec##_nan;                             \
            return;                                                     \
        }                                                               \
                                                                        \
        /* Argument reduction: bring x into [-pi, pi] */                \
        intern_##__dec##_t xred, ab_xred;                               \
        intern_##__dec##_mod_2pi(&xred, x);                             \
        intern_##__dec##_abs(&ab_xred, &xred);                          \
                                                                        \
        /* If |x| > pi/2, cos(x) = -cos(pi - |x|) (for |x| in [pi/2, pi]) */ \
        intern_##__dec##_t pi_over_two;                                 \
        intern_##__dec##_div(&pi_over_two, &intern_##__dec##_pi, &intern_##__dec##_two); \
                                                                        \
        intern_##__dec##_t y;                                           \
        int negate = 0;                                                 \
        if (intern_##__dec##_cmp(&ab_xred, &pi_over_two) > 0) {         \
            /* y = pi - |xred| */                                       \
            intern_##__dec##_t tmp;                                     \
            intern_##__dec##_sub(&tmp, &intern_##__dec##_pi, &ab_xred); \
            y = tmp;                                                    \
            negate = 1;                                                 \
        } else {                                                        \
            y = xred;                                                   \
            negate = 0;                                                 \
        }                                                               \
                                                                        \
        /* Cosine Taylor series: sum_{k=0}^{N} (-1)^k y^{2k} / (2k)! */ \
        /* We'll use up to k=7 (order 14, usually enough for 16 decimal digits) */ \
        const int N_TERMS = 7;                                          \
        intern_##__dec##_t term, sum, tmp;                              \
        sum = intern_##__dec##_one; /* first term is always 1 */        \
                                                                        \
        /* Precompute y^2 */                                            \
        intern_##__dec##_t y2;                                          \
        intern_##__dec##_mul(&y2, &y, &y);                              \
                                                                        \
        /* term = 1 (already in sum); for k=1 to N_TERMS, add next terms */ \
        intern_##__dec##_t y_pow = intern_##__dec##_one; /* y^0 = 1 */  \
                                                                        \
        for (int k = 1; k <= N_TERMS; ++k) {                            \
            /* y_pow *= y2   (always even powers for cosine) */         \
            intern_##__dec##_mul(&y_pow, &y_pow, &y2);                  \
            int n = 2*k;                                                \
            uint64_t fact = factorial(n);                               \
            intern_##__dec##_t denom = {1, fact, 0, DEC_NORMAL};        \
            intern_##__dec##_div(&term, &y_pow, &denom);                \
            if (k % 2 == 1) {                                           \
                /* Negative sign for odd k */                           \
                term.sign = -term.sign;                                 \
            }                                                           \
            /* sum = sum + term */                                      \
            intern_##__dec##_add(&tmp, &sum, &term);                    \
            if (intern_##__dec##_cmp(&tmp, &sum)==0)                    \
                break;                                                  \
                                                                        \
            sum = tmp;                                                  \
        }                                                               \
                                                                        \
        if (negate) {                                                   \
            sum.sign = -sum.sign;                                       \
        }                                                               \
        *result = sum;                                                  \
    }
