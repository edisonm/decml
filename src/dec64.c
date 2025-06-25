#include "dec64.h"
#include "intern/dec.h"
#include "impl_dec.h"
#include "impl_dec_str.h"
#include "impl_dec_abs.h"

const dec64_t dec64_zero= encode_dec64(0, 0, 0);
const dec64_t dec64_one = encode_dec64(0, 1, 0);
const dec64_t dec64_two = encode_dec64(0, 2, 0);
const dec64_t dec64_nan = { dec64_NAN_BITS };
const dec64_t dec64_inf = { dec64_INF_BITS };


__IMPL_DEC_SET_STR(dec64)
__IMPL_DEC_GET_STR(dec64)
__IMPL_DEC_FUNC2(add,   dec64)
__IMPL_DEC_FUNC2(sub,   dec64)
__IMPL_DEC_FUNC2(mul,   dec64)
__IMPL_DEC_FUNC2(div,   dec64)
__IMPL_DEC_FUNC2i(powi, dec64)
__IMPL_DEC_FUNCi2(cmp,  dec64)
__IMPL_DEC_FUNC1(log10, dec64)
__IMPL_DEC_FUNC1(atan,  dec64)
__IMPL_DEC_FUNC1(sqrt,  dec64)
__IMPL_DEC_FUNC1(asin,  dec64)
__IMPL_DEC_FUNC1(acos,  dec64)
__IMPL_DEC_FUNC1(sin,   dec64)
__IMPL_DEC_FUNC1(cos,   dec64)
__IMPL_DEC_FUNC1(exp10m1, dec64)
__IMPL_DEC_ABS(dec64)
__IMPL_NORMALIZE_TO_DEC(dec64)
__IMPL_DEC_TO_INTERNAL(dec64)
__IMPL_INTERNAL_TO_DEC(dec64)
