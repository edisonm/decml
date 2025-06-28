#include <stdio.h>
#include <inttypes.h> // For PRIu64 and such
#include "intern/dec64.h"
#include "intern/impl_dec.h"
#include "intern/impl_dec_compare.h"
#include "intern/impl_dec_atan.h"
#include "intern/impl_dec_sqrt.h"
#include "intern/impl_dec_sincos.h"
#include "intern/impl_dec_asin.h"
#include "intern/impl_dec_acos.h"
#include "intern/impl_dec_log10.h"
#include "intern/impl_dec_exp10m1.h"

__IMPL_CONST_INTERN_DEC(dec64, pi,      0, 3141592653589793238ULL, -I_dec64_MAX_DIGITS+1, DEC_NORMAL);
__IMPL_CONST_INTERN_DEC(dec64, log10_2, 0, 3321928094887362348ULL, -I_dec64_MAX_DIGITS+1, DEC_NORMAL);
__IMPL_CONST_INTERN_DEC(dec64, log_10,  0, 2302585092994045684ULL, -I_dec64_MAX_DIGITS+1, DEC_NORMAL);
__IMPL_CONST_INTERN_DEC(dec64, epsilon, 0,                   1ULL, -I_dec64_MAX_DIGITS,   DEC_NORMAL);

void print_uint64(uint64_t n) {
    printf("%" PRIu64, n);
}

__IMPL_CONST_INTERN_DECS(dec64)
__IMPL_NORMALIZE_COEFF_EXP_DEC(dec64)
__IMPL_NORMALIZE_INTERN_DEC(dec64)
__IMPL_DEC_COMPARE(dec64)
__IMPL_INTERN_DEC_ATAN(dec64, 63)
__IMPL_INTERN_DEC_SQRT(dec64)
__IMPL_INTERN_DEC_MOD_2PI(dec64)
__IMPL_INTERN_DEC_SIN(dec64)
__IMPL_INTERN_DEC_COS(dec64)
__IMPL_INTERN_DEC_ASIN(dec64)
__IMPL_INTERN_DEC_ACOS(dec64)
__IMPL_INTERN_DEC_LOG10(dec64)
__INTF_INTERN_DEC_EXP10M1_REC(dec64);
__IMPL_INTERN_DEC_EXP10M1_REC(dec64)
__IMPL_INTERN_DEC_EXP10M1(dec64)
