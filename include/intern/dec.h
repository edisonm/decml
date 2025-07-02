#ifndef INTERN_DEC_H
#define INTERN_DEC_H

typedef enum { DEC_NORMAL = 0, DEC_NAN = 1, DEC_INF = 2 } dec_special_t;
extern const char *dec_special[];
extern const uint64_t __pow10_dec64[];
extern const __uint128_t __pow10_dec128[];
uint64_t factorial(int n);

#define __INTF_INTERN_DEC(__name, __dec)                            \
    void intern_##__dec##_##__name(intern_##__dec##_t *result,      \
                                   const intern_##__dec##_t  *a)

#endif // INTERN_DEC_H
