#include <stdio.h>
#include <string.h>
#include "test_dec.h"
#include "dec64.h"
#include "test_dec64.h"

int process_result_dec64(dec64_t *result, int err, const char *input_str,
                         const char *expected_str, int expect_error) {
    if (expect_error) {
        if (err == 0) {
            printf("Test failed: input='%s' expected error but got success\n", input_str);
            return 1;
        } else {
            printf("Test passed (error as expected): input='%s'\n", input_str);
            return 0;
        }
    } else {
        if (err != 0) {
            printf("Test failed: input='%s' unexpected error\n", input_str);
            return 1;
        }

        char buf[64];
        if (result==NULL) {
            strcpy(buf, "");
        } else {
            dec64_get_str(result, buf, sizeof(buf));
        }

        if (strcmp(buf, expected_str) != 0) {
            printf("Test failed: input='%s' expected='%s' got='%s'\n", input_str, expected_str, buf);
            return 1;
        } else {
            printf("Test passed: input='%s' result='%s'\n", input_str, buf);
            return 0;
        }
    }
}

int test_dec64_1(void (*func_1)(dec64_t *, const dec64_t *),
                 const char *input_str, const char *expected_str, int expect_error) {
    dec64_t input, result;

    int err = dec64_set_str(&input, input_str);

    if (check_input(err, input_str))
        return 1;
    
    func_1(&result, &input);

    return process_result_dec64(&result, 0, input_str, expected_str, expect_error);
}

int test_dec64_2(void (*func_2)(dec64_t *, const dec64_t *, const dec64_t *),
                 const char *input_str_a, const char *input_str_b,
                 const char *expected_str, int expect_error) {
    dec64_t input_a, input_b, result;

    int err = dec64_set_str(&input_a, input_str_a);

    if (check_input(err, input_str_a))
        return 1;

    err = dec64_set_str(&input_b, input_str_b);
    
    if (check_input(err, input_str_b))
        return 1;

    func_2(&result, &input_a, &input_b);

    size_t length = strlen(input_str_a)+strlen(",")+strlen(input_str_b)+1;
    char input_str[length];
    strcat(strcat(strcpy(input_str, input_str_a),","), input_str_b);
    
    return process_result_dec64(&result, err, input_str, expected_str, expect_error);
}

int test_dec64_i2(int (*func_2)(const dec64_t *, const dec64_t *),
                  const char *input_str_a, const char *input_str_b,
                  int expected_value, int expect_error) {
    dec64_t input_a, input_b;
    
    int err = dec64_set_str(&input_a, input_str_a);

    if (check_input(err, input_str_a))
        return 1;

    err = dec64_set_str(&input_b, input_str_b);
    
    if (check_input(err, input_str_b))
        return 1;

    int value = func_2(&input_a, &input_b);

    err = (value == expected_value)?0:1;

    size_t length = strlen(input_str_a)+strlen(",")+strlen(input_str_b)+1;
    char input_str[length];
    strcat(strcat(strcpy(input_str, input_str_a),","), input_str_b);
    
    return process_result_dec64(NULL, err, input_str, "", expect_error);
}

int test_dec64_2i(void (*func_2i)(dec64_t *, const dec64_t *, const int),
                  const char *input_str, const int input_int,
                  const char *expected_str, int expect_error) {
    dec64_t input, result;

    int err = dec64_set_str(&input, input_str);

    if (check_input(err, input_str))
        return 1;
    
    func_2i(&result, &input, input_int);

    return process_result_dec64(&result, 0, input_str, expected_str, expect_error);
}
