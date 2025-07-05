
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

#define __INTF_DEC_FUNC1(__func, __dec)                                 \
    void __dec##_##__func(__dec##_t *result, const __dec##_t *a)
