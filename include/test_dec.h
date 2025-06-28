#ifndef TEST_DEC_H
#define TEST_DEC_H

typedef struct {
    const char *input;
    const char *expected;
} tests_1_t;

typedef struct {
    const char *input1;
    const char *input2;
    const char *expected;
} tests_2_t;

typedef struct {
    const char *input1;
    const char *input2;
    int expected;
} tests_i2_t;

typedef struct {
    const char *input1;
    const int input2;
    const char *expected;
} test_2i_t;

void report_failed(int failed);

#define RUN_TESTS_1(__tests, __func) ({                         \
            RUN_TESTS_1_DEC(intern_dec128, __tests, __func);    \
            RUN_TESTS_1_DEC(dec128, __tests, __func);           \
            RUN_TESTS_1_DEC(intern_dec64, __tests, __func);     \
            RUN_TESTS_1_DEC(dec64, __tests, __func);            \
        })

#define RUN_TESTS_1_DEC(__dec, __tests, __func) ({                      \
            char buf[256], b_x[256], b_e[256];                          \
            int passed = 0;                                             \
            int total = sizeof(__tests##_##__func) / sizeof(__tests##_##__func[0]); \
            __dec##_t x, r, e;                                          \
            printf(#__dec ": Testing " #__func "\n");                   \
            for (int i = 0; i < total; ++i) {                           \
                if (check_input(__dec##_set_str(&x, __tests##_##__func[i].input), \
                                __tests##_##__func[i].input)==0) {      \
                    __dec##_##__func(&r, &x);                           \
                    __dec##_get_str(&r, buf, sizeof(buf));              \
                    __dec##_set_str(&e, __tests##_##__func[i].expected); \
                    __dec##_get_str(&x, b_x, sizeof(b_x));              \
                    if ((__dec##_cmp(&r, &e)==0) ||                     \
                        (__dec##_is_nan(&r) && __dec##_is_nan(&e))) {   \
                        printf("Test %d passed: '%s(%s)=%s'\n", i + 1,  \
                               #__func, b_x, buf);                      \
                        passed++;                                       \
                    } else {                                            \
                        __dec##_t d;                                    \
                        __dec##_sub(&d, &e, &r);                        \
                        int r_exp = __dec##_get_exponent(&r);           \
                        int d_exp = __dec##_get_exponent(&d);           \
                        int diff = __dec##_get_coefficient(&d);         \
                        while (d_exp < r_exp) {                         \
                            d_exp++;                                    \
                            diff /= 10;                                 \
                        }                                               \
                        if (diff <= 30) {                               \
                            __dec##_get_str(&d, b_e, sizeof(b_e));      \
                            printf("Test %d passed: '%s(%s)=%s (+%s)'\n", i + 1, \
                                   #__func, b_x, buf, b_e);             \
                            passed++;                                   \
                        } else {                                        \
                            __dec##_get_str(&e, b_e, sizeof(b_e));      \
                            printf("Test %d failed: '%s(%s)=%s' expected='%s'\n", \
                                   i + 1, #__func, b_x, buf, b_e);      \
                            failed++;                                   \
                        }                                               \
                    }                                                   \
                }                                                       \
            }                                                           \
            printf("%d/%d tests passed.\n\n", passed, total);           \
        })

#define RUN_TESTS_2(__tests, __func) ({                         \
            RUN_TESTS_2_DEC(intern_dec128, __tests, __func);    \
            RUN_TESTS_2_DEC(dec128, __tests, __func);           \
            RUN_TESTS_2_DEC(intern_dec64, __tests, __func);     \
            RUN_TESTS_2_DEC(dec64, __tests, __func);            \
        })

#define RUN_TESTS_2_DEC(__dec, __tests, __func) ({                      \
            char buf[256], b_x[256], b_y[256], b_e[256];                \
            int passed = 0;                                             \
            int total = sizeof(__tests##_##__func) / sizeof(__tests##_##__func[0]); \
            __dec##_t x, y, r, e;                                       \
            printf(#__dec ": Testing " #__func "\n");                   \
            for (int i = 0; i < total; ++i) {                           \
                if (check_input(__dec##_set_str(&x, __tests##_##__func[i].input1), \
                                __tests##_##__func[i].input1)==0 &&     \
                    check_input(__dec##_set_str(&y, __tests##_##__func[i].input2), \
                                __tests##_##__func[i].input2)==0) {     \
                    __dec##_##__func(&r, &x, &y);                       \
                    __dec##_get_str(&r, buf, sizeof(buf));              \
                    __dec##_set_str(&e, __tests##_##__func[i].expected); \
                    __dec##_get_str(&x, b_x, sizeof(b_x));              \
                    __dec##_get_str(&y, b_y, sizeof(b_y));              \
                    if ((__dec##_cmp(&r, &e)==0) ||                     \
                        (__dec##_is_nan(&r) && __dec##_is_nan(&e))) {   \
                        printf("Test %d passed: '%s(%s,%s)=%s'\n", i + 1, \
                               #__func, b_x, b_y, buf);                 \
                        passed++;                                       \
                    } else {                                            \
                        __dec##_t d;                                    \
                        __dec##_sub(&d, &e, &r);                        \
                        int r_exp = __dec##_get_exponent(&r);           \
                        int d_exp = __dec##_get_exponent(&d);           \
                        int diff = __dec##_get_coefficient(&d);         \
                        while (d_exp < r_exp) {                         \
                            d_exp++;                                    \
                            diff /= 10;                                 \
                        }                                               \
                        if (diff <= 5) {                                \
                            __dec##_get_str(&d, b_e, sizeof(b_e));      \
                            printf("Test %d passed: '%s(%s,%s)=%s (+%s)'\n", i + 1, \
                                   #__func, b_x, b_y, buf, b_e);        \
                            passed++;                                   \
                        } else {                                        \
                            __dec##_get_str(&e, b_e, sizeof(b_e));      \
                            printf("Test %d failed: '%s(%s,%s)=%s' expected='%s'\n", \
                                   i + 1, #__func, b_x, b_y, buf, b_e); \
                            failed++;                                   \
                        }                                               \
                    }                                                   \
                }                                                       \
            }                                                           \
            printf("%d/%d tests passed.\n\n", passed, total);           \
        })

#define RUN_TESTS_2i(__tests, __func) ({                         \
            RUN_TESTS_2i_DEC(intern_dec128, __tests, __func);    \
            RUN_TESTS_2i_DEC(dec128, __tests, __func);           \
            RUN_TESTS_2i_DEC(intern_dec64, __tests, __func);     \
            RUN_TESTS_2i_DEC(dec64, __tests, __func);            \
        })

#define RUN_TESTS_2i_DEC(__dec, __tests, __func) ({                     \
            char buf[256], b_x[256], b_e[256];                          \
            int passed = 0;                                             \
            int total = sizeof(__tests##_##__func) / sizeof(__tests##_##__func[0]); \
            __dec##_t x, r, e;                                          \
            printf(#__dec ": Testing " #__func "\n");                   \
            for (int i = 0; i < total; ++i) {                           \
                if (check_input(__dec##_set_str(&x, __tests##_##__func[i].input1), \
                                __tests##_##__func[i].input1)==0) {     \
                    __dec##_##__func(&r, &x, __tests##_##__func[i].input2); \
                    __dec##_get_str(&r, buf, sizeof(buf));              \
                    __dec##_set_str(&e, __tests##_##__func[i].expected); \
                    __dec##_get_str(&x, b_x, sizeof(b_x));              \
                    if ((__dec##_cmp(&r, &e)==0) ||                     \
                        (__dec##_is_nan(&r) && __dec##_is_nan(&e))) {   \
                        printf("Test %d passed: '%s(%s,%d)=%s'\n", i + 1, \
                               #__func, b_x, __tests##_##__func[i].input2, buf); \
                        passed++;                                       \
                    } else {                                            \
                        __dec##_t d;                                    \
                        __dec##_sub(&d, &e, &r);                        \
                        int r_exp = __dec##_get_exponent(&r);           \
                        int d_exp = __dec##_get_exponent(&d);           \
                        int diff = __dec##_get_coefficient(&d);         \
                        while (d_exp < r_exp) {                         \
                            d_exp++;                                    \
                            diff /= 10;                                 \
                        }                                               \
                        if (diff <= 70) {                               \
                            __dec##_get_str(&d, b_e, sizeof(b_e));      \
                            printf("Test %d passed: '%s(%s,%d)=%s (+%s)'\n", i + 1, \
                                   #__func, b_x, __tests##_##__func[i].input2, buf, b_e); \
                            passed++;                                   \
                        } else {                                        \
                            __dec##_get_str(&e, b_e, sizeof(b_e));      \
                            printf("Test %d failed: '%s(%s,%d)=%s' expected='%s'\n", \
                                   i + 1, #__func, b_x,                 \
                                   __tests##_##__func[i].input2, buf, b_e); \
                            failed++;                                   \
                        }                                               \
                    }                                                   \
                }                                                       \
            }                                                           \
            printf("%d/%d tests passed.\n\n", passed, total);           \
        })

#define RUN_TESTS_i2(__tests, __func) ({                         \
            RUN_TESTS_i2_DEC(intern_dec128, __tests, __func);    \
            RUN_TESTS_i2_DEC(dec128, __tests, __func);           \
            RUN_TESTS_i2_DEC(intern_dec64, __tests, __func);     \
            RUN_TESTS_i2_DEC(dec64, __tests, __func);            \
        })

#define RUN_TESTS_i2_DEC(__dec, __tests, __func) ({                     \
            int passed = 0;                                             \
            int total = sizeof(__tests##_##__func) / sizeof(__tests##_##__func[0]); \
            __dec##_t x, y;                                             \
            int r;                                                      \
            printf(#__dec ": Testing " #__func "\n");                   \
            for (int i = 0; i < total; ++i) {                           \
                if (check_input(__dec##_set_str(&x, __tests##_##__func[i].input1), \
                                __tests##_##__func[i].input1)==0 &&     \
                    check_input(__dec##_set_str(&y, __tests##_##__func[i].input2), \
                                __tests##_##__func[i].input2)==0) {     \
                    r = __dec##_##__func(&x, &y);                       \
                    if (r == __tests##_##__func[i].expected) { \
                        printf("Test %d passed: '%s(%s,%s)=%d'\n", i + 1, \
                               #__func, __tests##_##__func[i].input1, __tests##_##__func[i].input2, r); \
                        passed++;                                       \
                    } else {                                            \
                        printf("Test %d failed: '%s(%s,%s)=%d' expected='%d'\n", \
                               i + 1,                                   \
                               #__func,                                 \
                               __tests##_##__func[i].input1,            \
                               __tests##_##__func[i].input2,            \
                               r,                                       \
                               __tests##_##__func[i].expected);         \
                        failed++;                                       \
                    }                                                   \
                }                                                       \
            }                                                           \
            printf("%d/%d tests passed.\n\n", passed, total);           \
        })

void report_failed(int failed);
int check_input(int err, const char *input_str);

#endif /* TEST_DEC_H */
