#define __INTF_DEC_FUNC1(__func, __dec_t) \
    void __dec_t##_##__func(__dec_t##_t *result, const __dec_t##_t *a)
