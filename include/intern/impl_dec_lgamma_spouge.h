#include "intern/dec.h"
#include "dec64.h"
#include "dec128.h"

/* Spouge approximation core:
   ln Γ(z) ≈ (z-1/2) ln(z+a) - (z+a) + ln( S ),
   where S = c0 + sum_{k=1}^{a-1} c_k / (z + k),
   c0 = sqrt(2π), c_k = (-1)^{k-1} a^{k-1/2} e^{-k} / (k-1)!.

   Requires Re(z) > 0.
*/

#define __IMPL_LOG_GAMMA_SPOUGE(__dec, A_DEFAULT)                       \
    static void intern_##__dec##_log_gamma_spouge(intern_##__dec##_t *out, \
                                                  const intern_##__dec##_t *z, \
                                                  int a_param) {        \
        const int a = (a_param > 0 ? a_param : A_DEFAULT);              \
        intern_##__dec##_t half = {0, 5, -1, DEC_NORMAL};               \
        intern_##__dec##_t a_dec = {0, a, 0, DEC_NORMAL};               \
        intern_##__dec##_t zmh, zpa, ln_zpa, term1, S, c0, tmp, t;      \
        intern_##__dec##_t two_pi;                                      \
                                                                        \
        /* c0 = sqrt(2π) */                                             \
        intern_##__dec##_mul(&two_pi, &intern_##__dec##_two, &intern_##__dec##_pi); \
        intern_##__dec##_sqrt(&c0, &two_pi);                            \
                                                                        \
        S = c0;                                                         \
                                                                        \
        /* Accumulate S += c_k / (z + k), k=1..a-1 */                   \
        intern_##__dec##_t ck, denom, pow_amk_kmh, exp_amk;             \
        intern_##__dec##_t fact = intern_##__dec##_one;                 \
        for (int k = 1; k < a; ++k) {                                   \
            /* printf("S ="); __print_intern_##__dec(S); printf("\n"); */ \
                                                                        \
            /* k - 1/2 */                                               \
            intern_##__dec##_t kmh = {0, 10*k - 5, -1, DEC_NORMAL};     \
            /* a - k */                                                 \
            intern_##__dec##_t amk = {0, a - k, 0, DEC_NORMAL};         \
            intern_##__dec##_pow(&pow_amk_kmh, &amk, &kmh);             \
                                                                        \
            /* exp_amk = e^{-k+a} */                                    \
            intern_##__dec##_exp(&exp_amk, &amk);                       \
                                                                        \
            /* ck = sign * pow_amk_kmh * exp(-k+a) / fact */            \
            intern_##__dec##_mul(&tmp, &pow_amk_kmh, &exp_amk);         \
            intern_##__dec##_div(&ck, &tmp, &fact);                     \
            if ((k % 2) == 0) {                                         \
                ck.sign = 1;                                            \
            }                                                           \
            /* printf("ck="); __print_intern_##__dec(ck); printf("\n"); */ \
                                                                        \
            /* denom = z + k */                                         \
            intern_##__dec##_t zk = {0, k, 0, DEC_NORMAL};              \
            intern_##__dec##_add(&denom, z, &zk);                       \
            intern_##__dec##_div(&tmp, &ck, &denom);                    \
            intern_##__dec##_add(&S, &S, &tmp);                         \
                                                                        \
            /* fact = (k-1)! */                                         \
            intern_##__dec##_mul(&fact, &fact, &zk);                    \
        }                                                               \
                                                                        \
        /* term1 = (z + 1/2) * ln(z + a) */                             \
        intern_##__dec##_add(&zmh, z, &half);                           \
        intern_##__dec##_add(&zpa, z, &a_dec);                          \
        intern_##__dec##_log(&ln_zpa, &zpa);                            \
        intern_##__dec##_mul(&term1, &zmh, &ln_zpa);                    \
                                                                        \
        /* out = term1 - (z + a) + ln(S) */                             \
        intern_##__dec##_log(&t, &S);                                   \
        intern_##__dec##_sub(&tmp, &term1, &zpa);                       \
        intern_##__dec##_add(out, &tmp, &t);                            \
    }

/* Helpers: check if x is integer and <= 0: use floor and comparisons */
#define IS_NONPOS_INTEGER(__dec, x) ({                                  \
            int is_int = 0, is_nonpos = 0;                              \
            intern_##__dec##_t fx;                                      \
            intern_##__dec##_floor(&fx, (x));                           \
            is_int = intern_##__dec##_equal(&fx, (x));                  \
            is_nonpos = intern_##__dec##_lessequal((x), &intern_##__dec##_zero); \
            is_int && is_nonpos;                                        \
        })

/* Reflection formula for x < 0.5:
   lgamma(x) = ln(pi) - ln(|sin(pi*x)|) - lgamma(1-x)
*/

#define __IMPL_INTERN_DEC_lgamma(__dec)                                 \
    void intern_##__dec##_lgamma(intern_##__dec##_t *res,               \
                                 const intern_##__dec##_t *x) {         \
        /* Specials */                                                  \
        if (x->special == DEC_NAN) { *res = intern_##__dec##_nan; return; } \
        if (x->special == DEC_INF) {                                    \
            /* +inf -> +inf; -inf -> NaN (choose NaN to be safe) */     \
            if (x->sign == 0) { *res = intern_##__dec##_inf; }          \
            else { *res = intern_##__dec##_nan; }                       \
            return;                                                     \
        }                                                               \
        /* Poles at non-positive integers */                            \
        if (IS_NONPOS_INTEGER(__dec, x)) { *res = intern_##__dec##_inf; return; } \
                                                                        \
        intern_##__dec##_t half = {0, 5, -1, DEC_NORMAL};               \
                                                                        \
        if (intern_##__dec##_less(x, &half)) {                          \
            /* Reflection */                                            \
            intern_##__dec##_t one = intern_##__dec##_one;              \
            intern_##__dec##_t y, pix, pi, sin_pix, abssin, lnpi, lnabssin, lg1mx; \
            intern_##__dec##_sub(&y, &one, x);                          \
            intern_##__dec##_lgamma(&lg1mx, &y);                        \
                                                                        \
            intern_##__dec##_mul(&pix, &intern_##__dec##_pi, x);        \
            intern_##__dec##_sin(&sin_pix, &pix);                       \
            intern_##__dec##_abs(&abssin, &sin_pix);                    \
                                                                        \
            intern_##__dec##_log(&lnpi, &pi);                           \
            intern_##__dec##_log(&lnabssin, &abssin);                   \
                                                                        \
            intern_##__dec##_t tmp;                                     \
            intern_##__dec##_sub(&tmp, &lnpi, &lnabssin);               \
            intern_##__dec##_sub(res, &tmp, &lg1mx);                    \
            return;                                                     \
        }                                                               \
                                                                        \
        /* Positive half-plane: Spouge */                               \
        intern_##__dec##_log_gamma_spouge(res, x, 0);                   \
    }
