#include "intern/dec.h"
#include "dec64.h"
#include "dec128.h"

/* non-positive integer test: floor(x) == x and x <= 0 */
#define IS_NONPOS_INTEGER(__dec, x) ({                                  \
            intern_##__dec##_t fx, zero = intern_##__dec##_zero;        \
            intern_##__dec##_floor(&fx, (x));                           \
            int is_int = intern_##__dec##_equal(&fx, (x));              \
            int is_nonpos = intern_##__dec##_lessequal((x), &zero);     \
            is_int && is_nonpos;                                        \
        })

/* ---------- core evaluator: positive half-plane (z >= 0.5) ---------- */

#define __IMPL_LANCZOS_CORE(__dec)                                      \
    static void intern_##__dec##_lgamma_lanczos_pos(                    \
        intern_##__dec##_t *out, const intern_##__dec##_t *z)           \
    {                                                                   \
        /* pick the right table */                                      \
        const int n = LANCZOS_##__dec##_N;                              \
                                                                        \
        intern_##__dec##_t half = {0, 5, -1, DEC_NORMAL};               \
        intern_##__dec##_t t, ln_t, xsum, tmp;                          \
                                                                        \
        /* xsum = a0 + Σ a_k/(z+k), k=1..n-1 */                         \
        xsum = lanczos_##__dec##_coef[0];                               \
        for (int k = 1; k < n; ++k) {                                   \
            intern_##__dec##_t denom, frac, kk = {0, k, 0, DEC_NORMAL}; \
            intern_##__dec##_add(&denom, z, &kk);                       \
            intern_##__dec##_div(&frac, &lanczos_##__dec##_coef[k], &denom); \
            intern_##__dec##_add(&xsum, &xsum, &frac);                  \
        }                                                               \
                                                                        \
        /* t = z + g + 0.5 */                                           \
        intern_##__dec##_add(&tmp, z, &lanczos_##__dec##_g);            \
        intern_##__dec##_add(&t, &tmp, &half);                          \
                                                                        \
        intern_##__dec##_t two_pi, ln2pi, half_ln2pi;                   \
        intern_##__dec##_mul(&two_pi, &intern_##__dec##_two, &intern_##__dec##_pi); \
        intern_##__dec##_log(&ln2pi, &two_pi);                          \
        intern_##__dec##_mul(&half_ln2pi, &half, &ln2pi);               \
                                                                        \
        /* ln Γ = 0.5 ln(2π) + ln(xsum) + (z+0.5) ln t − t */           \
        intern_##__dec##_t ln_xsum, zln, zpo5;                          \
        intern_##__dec##_log(&ln_xsum, &xsum);                          \
        intern_##__dec##_add(&zpo5, z, &half);                          \
        intern_##__dec##_log(&ln_t, &t);                                \
        intern_##__dec##_mul(&zln, &zpo5, &ln_t);                       \
                                                                        \
        /* out = half_ln2pi + ln_xsum + zln − t */                      \
        intern_##__dec##_add(out, &half_ln2pi, &ln_xsum);               \
        intern_##__dec##_add(out, out, &zln);                           \
        intern_##__dec##_sub(out, out, &t);                             \
    }

/* ---------- full lgamma with reflection and specials ---------- */

#define __IMPL_INTERN_DEC_lgamma(__dec)                                 \
    void intern_##__dec##_lgamma(intern_##__dec##_t *res,               \
                                 const intern_##__dec##_t *x) {         \
        /* specials */                                                  \
        if (intern_##__dec##_is_nan(x)) {                               \
            *res = intern_##__dec##_nan;                                \
            return;                                                     \
        }                                                               \
        if (intern_##__dec##_is_inf(x)) {                               \
            if (x->sign == 0) { *res = intern_##__dec##_inf; }          \
            else { *res = intern_##__dec##_nan; }                       \
            return;                                                     \
        }                                                               \
        if (IS_NONPOS_INTEGER(__dec, x)) { *res = intern_##__dec##_inf; return; } \
                                                                        \
        intern_##__dec##_t half = {0, 5, -1, DEC_NORMAL};               \
                                                                        \
        if (intern_##__dec##_less(x, &half)) {                          \
            /* Reflection: lnΓ(x) = lnπ − ln|sin(πx)| − lnΓ(1−x) */     \
            intern_##__dec##_t one = intern_##__dec##_one;              \
            intern_##__dec##_t y, lg1mx, pi, pix, sin_pix, abssin, lnabssin, lnpi; \
            intern_##__dec##_sub(&y, &one, x);                          \
            intern_##__dec##_lgamma(&lg1mx, &y);                        \
            intern_##__dec##_mul(&pix, &intern_##__dec##_pi, x);        \
            intern_##__dec##_sin(&sin_pix, &pix);                       \
            intern_##__dec##_abs(&abssin, &sin_pix);                    \
            intern_##__dec##_log(&lnabssin, &abssin);                   \
            intern_##__dec##_log(&lnpi, &pi);                           \
            intern_##__dec##_t tmp;                                     \
            intern_##__dec##_sub(&tmp, &lnpi, &lnabssin);               \
            intern_##__dec##_sub(res, &tmp, &lg1mx);                    \
            return;                                                     \
        }                                                               \
                                                                        \
        intern_##__dec##_lgamma_lanczos_pos(res, x);                    \
    }
