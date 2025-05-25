#ifndef TEST_DEC64_H
#define TEST_DEC64_H

int test_dec64_1(void (*func_1)(dec64_t *, const dec64_t *),
                 const char *input_str, const char *expected_str, int expect_error);
int test_dec64_2(void (*func_2)(dec64_t *, const dec64_t *, const dec64_t *),
                 const char *input_str_a, const char *input_str_b, const char *expected_str,
                 int expect_error);
int test_dec64_i2(int (*func_2)(const dec64_t *, const dec64_t *),
                  const char *input_str_a, const char *input_str_b,
                  int expected_value, int expect_error);
int test_dec64_2i(void (*func_2)(dec64_t *, const dec64_t *, const int),
                  const char *input_str, const int input_int,
                  const char *expected_str, int expect_error);

void report_failed(int failed);

#endif /* TEST_DEC64_H */
