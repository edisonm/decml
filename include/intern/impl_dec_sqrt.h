#define __IMPL_INTERN_DEC_SQRT(__dec)                                   \
    void intern_##__dec##_sqrt(intern_##__dec##_t *result,              \
                               const intern_##__dec##_t *a) {           \
        /* Handle special values: */                                    \
        if (intern_##__dec##_is_nan(a)) {                               \
            *result = intern_##__dec##_nan;                             \
            return;                                                     \
        }                                                               \
                                                                        \
        if (a->sign) {                                                  \
            /* sqrt of negative number: NaN */                          \
            intern_##__dec##_set_nan(result);                           \
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
            /* sqrt(0) = 0 */                                           \
            *result = intern_##__dec##_zero;                            \
            return;                                                     \
        }                                                               \
                                                                        \
        int exp = a->exponent;                                          \
        /* Make exponent even */                                        \
        if (exp % 2 != 0) {                                             \
            coeff /= 10;                                                \
            exp += 1;                                                   \
        }                                                               \
                                                                        \
        /* Now exp is even */                                           \
        int sqrt_exp = exp / 2;                                         \
                                                                        \
        /* Compute sqrt of coeff using Newton-Raphson */                \
                                                                        \
        /* Initial guess bigger than the actual root so that the residual will be positive: */ \
        bits_##__dec##_t x = 1;                                         \
        while (x*x < coeff && x < (bits_##__dec##_t)1<<(I_##__dec##_BITS/2)) { \
            x *= 2;                                                     \
        }                                                               \
                                                                        \
        bits_##__dec##_t residual = x*x - coeff;                        \
                                                                        \
        for (int i = 0; i < 20; i++) { /* iterate enough times for convergence */ \
            if (x == 0) break;                                          \
            while ((x <= UINT_##__dec##_MAX/10)&&                       \
                   (residual <= UINT_##__dec##_MAX/100)) {              \
                /* scale up x to increase precision */                  \
                x *= 10;                                                \
                sqrt_exp--;                                             \
                residual *= 100;                                        \
            }                                                           \
            bits_##__dec##_t delta = residual / 2 / x;                  \
            bits_##__dec##_t x_next = x - delta;                        \
            if (x_next == x) break;                                     \
            residual -= delta * (x + x_next);                           \
            x = x_next;                                                 \
        }                                                               \
                                                                        \
        /* since we approximated the root from the right, it could be that the value */ \
        /* at the left is more precise, so we can prove that by taking delta=1 and */ \
        /* simplifying the expressions, the next simple line will improve x: */ \
        if (x <= residual)                                              \
            x--;                                                        \
                                                                        \
        result->sign = 0;                                               \
        result->coeff = x;                                              \
        result->exponent = sqrt_exp;                                    \
        result->special = DEC_NORMAL;                                   \
    }
