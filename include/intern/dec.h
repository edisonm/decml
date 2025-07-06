#ifndef INTERN_DEC_H
#define INTERN_DEC_H

typedef enum { DEC_NORMAL = 0, DEC_NAN = 1, DEC_INF = 2 } dec_special_t;
extern const char *dec_special[];
extern const uint64_t __pow10_dec64[];
extern const __uint128_t __pow10_dec128[];

#define __IMPL_INTERN_DEC_FUNC_1(__func, __dec) \
    __IMPL_INTERN_DEC_##__func(__dec)

#endif // INTERN_DEC_H
