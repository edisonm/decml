#ifndef TEST_DEC128_H
#define TEST_DEC128_H

int test_dec128_1(void (*func_1)(dec128_t *, const dec128_t *),
                  const char *input_str, const char *expected_str, int expect_error);
int test_dec128_2(void (*func_2)(dec128_t *, const dec128_t *, const dec128_t *),
                  const char *input_str_a, const char *input_str_b, const char *expected_str,
                  int expect_error);
int test_dec128_i2(int (*func_2)(const dec128_t *, const dec128_t *),
                   const char *input_str_a, const char *input_str_b,
                   int expected_value, int expect_error);
int test_dec128_2i(void (*func_2)(dec128_t *, const dec128_t *, const int),
                   const char *input_str, const int input_int,
                   const char *expected_str, int expect_error);

int test_i_dec128_2(void (*func_1)(intern_dec128_t *, const intern_dec128_t *, const intern_dec128_t *),
                    const char *input_str_a, const char *input_str_b, const char *expected_str, int expect_error);

void report_failed(int failed);

#endif /* TEST_DEC128_H */
