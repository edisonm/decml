#include "intern/dec128.h"
#include "intern/impl_dec_arithmetic.h"

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
        // Shift the `current_rem` accumulator left by 1 bit.
        // This effectively makes space for the next bit from `n_lo`.
        // The most significant bit of `current_rem` might "overflow" temporarily,
        // but `__uint128_t` arithmetic handles this correctly internally for comparison/subtraction.
        if (current_rem >= (d - current_rem)) {
            current_rem = (current_rem - (d - current_rem)) + ((n_lo >> i) & 1);
            q_lo |= ((__uint128_t)1 << i);
        } else {
            current_rem = (current_rem << 1) | ((n_lo >> i) & 1);

            // If the current accumulated remainder (with the new bit) is greater than or
            // equal to the denominator, perform a subtraction.
            // This is equivalent to finding the next quotient bit.
            if (current_rem >= d) {
                current_rem -= d;
                // Set the corresponding bit in the low part of the quotient.
                q_lo |= ((__uint128_t)1 << i);
            }
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

__IMPL_INTERN_DEC_ADD( dec128)
__IMPL_INTERN_DEC_SUB( dec128)
__IMPL_INTERN_DEC_MUL( dec128)
__IMPL_INTERN_DEC_DIV( dec128)
__IMPL_INTERN_DEC_POWI(dec128)
__IMPL_INTERN_DEC_ABS( dec128)
