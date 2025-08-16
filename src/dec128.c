#include "dec128.h"
#include "intern/dec.h"
#include "impl_dec.h"
#include "impl_dec_str.h"
#include "impl_dec_abs.h"

const dec128_t dec128_zero  = encode_dec128(0, 0, 0);
const dec128_t dec128_one   = encode_dec128(0, 1, 0);
const dec128_t dec128_two   = encode_dec128(0, 2, 0);
const dec128_t dec128_three = encode_dec128(0, 3, 0);
const dec128_t dec128_nan   = { dec128_NAN_BITS };
const dec128_t dec128_inf   = { dec128_INF_BITS };
const dec128_t dec128_epsilon = encode_dec128(0, 1, -dec128_MAX_DIGITS);

__IMPL_DEC_SET_STR(dec128)
__IMPL_DEC_GET_STR(dec128)
__IMPL_DEC_FUNC2(add,     dec128)
__IMPL_DEC_FUNC2(sub,     dec128)
__IMPL_DEC_FUNC2(mul,     dec128)
__IMPL_DEC_FUNC2(div,     dec128)
__IMPL_DEC_FUNC2i(powi,   dec128)
__IMPL_DEC_FUNCi2(cmp,    dec128)

__DEC_FUNC_1_ALL(__IMPL, dec128)

__IMPL_DEC_ABS(dec128)
__IMPL_NORMALIZE_TO_DEC(dec128)
__IMPL_DEC_TO_INTERNAL(dec128)
__IMPL_INTERNAL_TO_DEC(dec128)
