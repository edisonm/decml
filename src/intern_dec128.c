#include <stdio.h>
#include <inttypes.h> // For PRIu64 and such
#include "intern/dec128.h"
#include "intern/impl_dec.h"
#include "intern/impl_dec_compare.h"
#include "intern/impl_dec_atan.h"
#include "intern/impl_dec_sqrt.h"
#include "intern/impl_dec_cbrt.h"
#include "intern/impl_dec_sincos.h"
#include "intern/impl_dec_asin.h"
#include "intern/impl_dec_acos.h"
#include "intern/impl_dec_log10.h"
#include "intern/impl_dec_exp10m1.h"
/* We implemented 2 lgamma methods to be able to compare, so far the best is lanczos */
/* #define USE_SPOUGE */
#ifdef USE_SPOUGE
# include "intern/impl_dec_lgamma_spouge.h"
#else
# include "intern/impl_dec_lgamma.h"
#endif

#include "intern/impl_dec_arithmetic.h"

// 31415926535897932384626433832795028842e-37 (38 digits)
__IMPL_CONST_INTERN_DEC(dec128, pi, 0, 
                        ((__uint128_t)3141592653589793238ULL * 10000000000000000000ULL)
                        + 4626433832795028842ULL,
                        -I_dec128_MAX_DIGITS+1, DEC_NORMAL);
// 33219280948873623478703194294893901759e-37
__IMPL_CONST_INTERN_DEC(dec128, log2_10, 0, 
                        ((__uint128_t)3321928094887362347ULL * 10000000000000000000ULL)
                        + 8703194294893901759ULL,
                        -I_dec128_MAX_DIGITS+1, DEC_NORMAL);

__IMPL_CONST_INTERN_DEC(dec128, log_10, 0,
                        ((__uint128_t)2302585092994045684ULL * 10000000000000000000ULL)
                        +  179914546843642076ULL,
                        -I_dec128_MAX_DIGITS+1, DEC_NORMAL);

__IMPL_CONST_INTERN_DEC(dec128, log10_e, 0,
                        ((__uint128_t)4342944819032518276ULL * 10000000000000000000ULL)
                        +  5112891891660508229ULL,
                        -I_dec128_MAX_DIGITS, DEC_NORMAL);

__IMPL_CONST_INTERN_DEC(dec128, epsilon, 0, (__uint128_t)1ULL, -I_dec128_MAX_DIGITS, DEC_NORMAL);


#ifndef USE_SPOUGE
// echo "r=dump_lanczos_dec128_coef(18,25)"| BC_LINE_LENGTH=0 bc -l lanczos.bc
// lanczos(18,25,G),maplist(dump_dec128,G),fail.

static const int LANCZOS_dec128_N = 25;
static const intern_dec128_t lanczos_dec128_g = {0, 18, 0, DEC_NORMAL};

const intern_dec128_t lanczos_dec128_coef[] = {
    {0,  (__uint128_t)9999999999999999999ULL * 10000000000000000000ULL + 9999999999999950240ULL, -38, DEC_NORMAL},
    {0,  (__uint128_t)6646339528683197792ULL * 10000000000000000000ULL + 4099909081195860925ULL, -30, DEC_NORMAL},
    {1,  (__uint128_t)3917373070961464362ULL * 10000000000000000000ULL + 6131890285799442382ULL, -29, DEC_NORMAL},
    {0, (__uint128_t)10168895516980457825ULL * 10000000000000000000ULL +  724792561192594618ULL, -29, DEC_NORMAL},
    {1, (__uint128_t)15304417720266604741ULL * 10000000000000000000ULL + 9497722137586126914ULL, -29, DEC_NORMAL},
    {0, (__uint128_t)14797089070429975149ULL * 10000000000000000000ULL + 4401059374328143383ULL, -29, DEC_NORMAL},
    {1,  (__uint128_t)9625314199609573134ULL * 10000000000000000000ULL + 1059899492094357731ULL, -29, DEC_NORMAL},
    {0,  (__uint128_t)4290422619246258943ULL * 10000000000000000000ULL + 2701407330181952149ULL, -29, DEC_NORMAL},
    {1, (__uint128_t)13106638839759456867ULL * 10000000000000000000ULL + 1022946165497922607ULL, -30, DEC_NORMAL},
    {0, ((__uint128_t)2702948464965313996ULL * 10000000000000000000ULL + 6825618317420853334ULL)*10+1, -31, DEC_NORMAL},
    {1,  (__uint128_t)3648630829312252970ULL * 10000000000000000000ULL +  960490625680316178ULL, -31, DEC_NORMAL},
    {0, ((__uint128_t)3065483005597971028ULL * 10000000000000000000ULL)*10 + 11424108080257549702ULL, -33, DEC_NORMAL},
    {1, (__uint128_t)14831056232551733655ULL * 10000000000000000000ULL + 7602761533938222691ULL, -34, DEC_NORMAL},
    {0,  (__uint128_t)3662067717866306977ULL * 10000000000000000000ULL + 8951402453569534733ULL, -35, DEC_NORMAL},
    {1,  (__uint128_t)3795979461432854863ULL * 10000000000000000000ULL + 4254119707029618720ULL, -37, DEC_NORMAL},
    {0, (__uint128_t)11735971427346459004ULL * 10000000000000000000ULL + 6348057509891376131ULL, -40, DEC_NORMAL},
    {1,  (__uint128_t)5472890941427576835ULL * 10000000000000000000ULL + 8872708669284379511ULL, -43, DEC_NORMAL},
    {0,  (__uint128_t)6874730963681002586ULL * 10000000000000000000ULL + 2945691989946887927ULL, -48, DEC_NORMAL},
    {1, ((__uint128_t)3150169841533466895ULL * 10000000000000000000ULL + 5304464128206672094ULL)*10+7, -53, DEC_NORMAL},
    {0,  (__uint128_t)2354846995100611245ULL * 10000000000000000000ULL + 8219479355440424837ULL, -52, DEC_NORMAL},
    {1, (__uint128_t)10341226774991169378ULL * 10000000000000000000ULL + 5586759018751395749ULL, -53, DEC_NORMAL},
    {0, ((__uint128_t)2586460398969970644ULL * 10000000000000000000ULL + 9882973544196895729ULL)*10+5, -54, DEC_NORMAL},
    {1,  (__uint128_t)3365942223013066763ULL * 10000000000000000000ULL + 1075754945952264666ULL, -54, DEC_NORMAL},
    {0,  (__uint128_t)9298129334428721936ULL * 10000000000000000000ULL + 5251191786236301098ULL, -56, DEC_NORMAL},
    {0, ((__uint128_t)2044999006729453561ULL * 10000000000000000000ULL + 8070042123871469051ULL)*10+2, -57, DEC_NORMAL},
};
#endif

void print_uint128(__uint128_t n) {
    if (n > UINT64_MAX) {
        print_uint128(n / 10);
        putchar((char)('0' + (n % 10)));
    } else {
        printf("%" PRIu64, (uint64_t)n);
    }
}

static inline __uint128_t lo_ddec128(uint256_t *x) {return x->lo;}
static inline __uint128_t hi_ddec128(uint256_t *x) {return x->hi;}

/* Multiply two __uint128_t to get a 256 bit result in (hi, lo) */
static void mul_bits_dec128_ddec128(__uint128_t a, __uint128_t b, uint256_t *product) {
    const __uint128_t MASK_64 = (__uint128_t)0xFFFFFFFFFFFFFFFFULL;
    
    __uint128_t a_lo = a & MASK_64;
    __uint128_t a_hi = a >> 64;
    __uint128_t b_lo = b & MASK_64;
    __uint128_t b_hi = b >> 64;

    __uint128_t p0 = a_lo * b_lo;
    __uint128_t p1 = a_lo * b_hi;
    __uint128_t p2 = a_hi * b_lo;
    __uint128_t p3 = a_hi * b_hi;

    __uint128_t mid1 = p1 + (p0 >> 64);
    __uint128_t mid2 = p2;

    // Carry between mid1 and mid2 should be considered
    __uint128_t mid = mid1 + mid2;
    __uint128_t carry = (mid < mid1) ? ((__uint128_t)1 << 64) : 0;  // Overflow check
    
    product->hi = p3 + (mid >> 64) + carry;
    product->lo = (mid << 64) | (p0 & MASK_64);
}

// Divides numerator (256 bits) by denominator (128 bits)
// Returns quotient (256 bits) via *quotient, remainder (128 bits) via *remainder if not NULL.
/*
// bit-by-bit division, only for testing:
void div_bits_ddec128_dec128(const uint256_t *numerator,
                             const __uint128_t denominator,
                             uint256_t *quotient,
                             __uint128_t *remainder) {
    uint256_t q = {0, 0};  // Initialize quotient
    __uint128_t r = 0;     // Remainder

    for (int i = 255; i >= 0; i--) {
        // Shift left remainder by 1 bit, bring in current bit from numerator
        r = (r << 1) | ((i < 128) ? (numerator->lo >> i) & 1 : (numerator->hi >> (i - 128)) & 1);

        // If remainder is greater than or equal to denominator, perform subtraction
        if (r >= denominator) {
            r -= denominator;
            // Set the corresponding bit in quotient
            if (i < 128)
                q.lo |= (__uint128_t)1 << i;
            else
                q.hi |= (__uint128_t)1 << (i - 128);
        }
    }

    // Set the results
    *quotient = q;
    if (remainder != NULL) {
        *remainder = r;
    }
}
*/

/**
 * @brief Divides a 256-bit numerator by a 128-bit denominator.
 *
 * This function performs long division of a 256-bit unsigned integer by a
 * 128-bit unsigned integer. It uses native `__uint128_t` operations for
 * efficiency, breaking down the 256-bit division into operations on 128-bit
 * limbs, similar to manual long division.
 *
 * @param numerator Pointer to the 256-bit numerator.
 * @param denominator The 128-bit denominator.
 * @param quotient Pointer to the output 256-bit quotient.
 * @param remainder_out Optional pointer to the output 128-bit remainder.
 * If NULL, the remainder is not stored.
 */

void div_bits_ddec128_dec128(
    const uint256_t *numerator,
    const __uint128_t denominator,
    uint256_t *quotient,
    __uint128_t *remainder_out
) {
    // Handle division by zero.
    // In a production environment, this might ideally return an error code or
    // assert to signal an invalid operation. Given the `void` return type,
    // we set the output variables to zero and return.
    if (denominator == 0) {
        if (quotient) {
            quotient->hi = 0;
            quotient->lo = 0;
        }
        if (remainder_out) {
            *remainder_out = 0;
        }
        return;
    }

    // Extract numerator limbs and denominator for easier access
    __uint128_t n_hi = numerator->hi;
    __uint128_t n_lo = numerator->lo;
    __uint128_t d = denominator;

    __uint128_t q_hi = 0;         // High 128 bits of the quotient
    __uint128_t q_lo = 0;         // Low 128 bits of the quotient
    __uint128_t current_rem;      // Stores the remainder from the high part division,
                                  // then acts as an accumulator for the low part division.

    // Case 1: Denominator is greater than the entire 256-bit numerator.
    // This happens if the high 128 bits of the numerator are zero, AND
    // the low 128 bits of the numerator are less than the denominator.
    if (n_hi == 0 && n_lo < d) {
        if (quotient) {
            quotient->hi = 0;
            quotient->lo = 0;
        }
        if (remainder_out) {
            *remainder_out = n_lo; // The numerator itself is the remainder
        }
        return;
    }

    // --- Step 1: Divide the high 128 bits of the numerator by the denominator ---
    // This calculation yields the high 128 bits of the final quotient (q_hi)
    // and an intermediate remainder (current_rem) that needs to be carried over.
    q_hi = n_hi / d;
    current_rem = n_hi % d;

    // --- Step 2: Divide the combined (remainder from step 1, low 128 bits of numerator)
    //             by the denominator to get the low 128 bits of the quotient and the final remainder. ---
    // The effective dividend for this step is a 256-bit number formed by:
    // (current_rem << 128) | n_lo
    // Since `current_rem` (the remainder from the previous step) is guaranteed to be less than `d`,
    // the resulting low quotient (`q_lo`) will fit within a `__uint128_t`.

    // We simulate a 256-bit division using a bit-by-bit long division loop,
    // operating on 128-bit chunks. `current_rem` serves as a 128-bit accumulator
    // that incorporates bits from `n_lo` sequentially.
    for (int i = 127; i >= 0; --i) {
        // Shift the `current_rem` accumulator left by 1 bit, but be aware of
        // overflow (see if branches).  This effectively makes space for the
        // next bit from `n_lo`.  The most significant bit of `current_rem`
        // might "overflow" temporarily, but `__uint128_t` arithmetic handles
        // this correctly internally for comparison/subtraction.
        __uint128_t tmp = current_rem + ((n_lo >> i) & 1);
        __uint128_t dif = d - current_rem;
        if (tmp >= dif) {
            current_rem = tmp - dif;
            q_lo |= ((__uint128_t)1 << i);
        } else {
            current_rem += tmp;
        }
    }

    // --- Final assignments ---
    // Store the calculated high and low parts into the quotient structure.
    if (quotient) {
        quotient->hi = q_hi;
        quotient->lo = q_lo;
    }

    // The `current_rem` after the loop contains the final 128-bit remainder
    // of the entire 256-bit division.
    if (remainder_out) {
        *remainder_out = current_rem;
    }
}

__IMPL_CONST_INTERN_DECS(dec128)
__IMPL_NORMALIZE_COEFF_EXP_DEC(dec128)
__IMPL_NORMALIZE_INTERN_DEC(dec128)
__IMPL_DEC_COMPARE(dec128)

__IMPL_INTERN_DEC_MOD_2PI(dec128)
__INTF_INTERN_DEC_EXP10M1_REC(dec128);
__IMPL_INTERN_DEC_EXP10M1_REC(dec128)
__IMPL_DEC_BITS_DIV_POW10_ROUND_UP(dec128)

#ifdef USE_SPOUGE
__IMPL_LOG_GAMMA_SPOUGE(dec128, 36)
#else
__IMPL_LANCZOS_CORE(dec128)
#endif

__DEC_FUNC_1_ALL(__IMPL_INTERN, dec128)
__DEC_FUNC_2_ALL(__IMPL_INTERN, dec128)

/* __IMPL_DEC_BITS_DIV_POW10_ROUND(dec128) */
__IMPL_INTERN_DEC_POWI(dec128)
__IMPL_INTERN_DEC_abs( dec128)
