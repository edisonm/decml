
#define __INTF_DEC_GREATER(__dec)                                       \
    static inline int __dec##_greater(const __dec##_t *a, const __dec##_t *b) { \
        int cmp = __dec##_cmp(a, b);                                    \
        return cmp != 2 && cmp > 0;                                     \
    }

#define __INTF_DEC_LESS(__dec)                                          \
    static inline int __dec##_less(const __dec##_t *a, const __dec##_t *b) { \
        int cmp = __dec##_cmp(a, b);                                    \
        return cmp != 2 && cmp < 0;                                     \
    }

#define __INTF_DEC_EQUAL(__dec)                                         \
    static inline int __dec##_equal(const __dec##_t *a, const __dec##_t *b) { \
        int cmp = __dec##_cmp(a, b);                                    \
        return cmp != 2 && cmp == 0;                                    \
    }

#define __INTF_DEC_LESSEQUAL(__dec)                                     \
    static inline int __dec##_lessequal(const __dec##_t *a, const __dec##_t *b) { \
        int cmp = __dec##_cmp(a, b);                                    \
        return cmp != 2 && cmp <= 0;                                    \
    }

#define __INTF_DEC_GREATEREQUAL(__dec)                                  \
    static inline int __dec##_greaterequal(const __dec##_t *a, const __dec##_t *b) { \
        int cmp = __dec##_cmp(a, b);                                    \
        return cmp != 2 && cmp >= 0;                                    \
    }

#define __INTF_DEC_IS_UNORDERED(__dec)                                  \
    static inline int __dec##_is_unordered(const __dec##_t *a, const __dec##_t *b) { \
        int cmp = __dec##_cmp(a, b);                                    \
        return cmp == 2;                                                \
    }

#define __INTF_DEC_FUNC_1(__func, __dec)                                \
    void __dec##_##__func(__dec##_t *result, const __dec##_t *a)

#define __DEC_FUNC_1_ALL(__pref, __dec)         \
    __pref##_DEC_FUNC_1(sqrt,    __dec);        \
    __pref##_DEC_FUNC_1(log10,   __dec);        \
    __pref##_DEC_FUNC_1(log,     __dec);        \
    __pref##_DEC_FUNC_1(atan,    __dec);        \
    __pref##_DEC_FUNC_1(asin,    __dec);        \
    __pref##_DEC_FUNC_1(acos,    __dec);        \
    __pref##_DEC_FUNC_1(sin,     __dec);        \
    __pref##_DEC_FUNC_1(cos,     __dec);        \
    __pref##_DEC_FUNC_1(exp10m1, __dec);        \
    __pref##_DEC_FUNC_1(expm1,   __dec);        \
    __pref##_DEC_FUNC_1(sinh,    __dec);        \
    __pref##_DEC_FUNC_1(cosh,    __dec);        \
    __pref##_DEC_FUNC_1(tanh,    __dec);        \
    __pref##_DEC_FUNC_1(cbrt,    __dec);

#define __DEC_FUNC_2_ALL(__pref, __dec)         \
    __pref##_DEC_FUNC_2(add,     __dec);        \
    __pref##_DEC_FUNC_2(sub,     __dec);        \
    __pref##_DEC_FUNC_2(mul,     __dec);        \
    __pref##_DEC_FUNC_2(div,     __dec);

#define __DEC_FUNC_2i_ALL(__pref, __dec)        \
    __pref##_DEC_FUNC_2i(powi,     __dec);

#define __DEC_FUNC_i2_ALL(__pref, __dec)                \
    __pref##_DEC_FUNC_i2(equal,        __dec);          \
    __pref##_DEC_FUNC_i2(less,         __dec);          \
    __pref##_DEC_FUNC_i2(greater,      __dec);          \
    __pref##_DEC_FUNC_i2(lessequal,    __dec);          \
    __pref##_DEC_FUNC_i2(greaterequal, __dec);
