#include <stdio.h>
#include <string.h>
#include "dec64.h"
#include "test_dec64.h"

int main(void) {
    int failed = 0;
    failed += test_dec64_1(dec64_atan, "100000000000000000", "1.570796326794897", 0);
    printf("dec64: Testing atan\n");
    failed += test_dec64_1(dec64_atan, "0.1",      "0.09966865249116203", 0); // a(0.1)
    failed += test_dec64_1(dec64_atan, "0.01",     "0.009999666686665238", 0);
    failed += test_dec64_1(dec64_atan, "0.001",    "0.0009999996666668667", 0);
    failed += test_dec64_1(dec64_atan, "0.0001",   "0.00009999999966666667", 0);
    failed += test_dec64_1(dec64_atan, "0.00001",  "9.999999999666667e-6", 0);
    failed += test_dec64_1(dec64_atan, "0.000001", "9.999999999996667e-7", 0);
    failed += test_dec64_1(dec64_atan, "1",        "0.7853981633974483", 0); // a(1)
    failed += test_dec64_1(dec64_atan, "-1",      "-0.7853981633974483", 0); // a(-1)
    failed += test_dec64_1(dec64_atan, "1.000001", "0.7853986633971983", 0);
    failed += test_dec64_1(dec64_atan, "0.999999", "0.7853976633971983", 0);
    failed += test_dec64_1(dec64_atan, "1.732050807568877",  "1.047197551196598", 0);
    failed += test_dec64_1(dec64_atan, "100000000000000000", "1.570796326794897", 0);
    failed += test_dec64_1(dec64_atan, "1e100",              "1.570796326794897", 0);
    failed += test_dec64_1(dec64_atan,  "1e-100",  "1e-100", 0);
    failed += test_dec64_1(dec64_atan, "-1e-100", "-1e-100", 0);
    printf("dec64: Testing acos\n");
    failed += test_dec64_1(dec64_acos, "1",        "0", 0);
    failed += test_dec64_1(dec64_acos, "0.1",      "1.470628905633337", 0);
    failed += test_dec64_1(dec64_acos, "0.01",     "1.56079616012073", 0);
    failed += test_dec64_1(dec64_acos, "0.001",    "1.56979632662823", 0);
    failed += test_dec64_1(dec64_acos, "0.0001",   "1.57069632679473", 0);
    failed += test_dec64_1(dec64_acos, "0.00001",  "1.570786326794896", 0);
    failed += test_dec64_1(dec64_acos, "0.000001", "1.570795326794897", 0);
    failed += test_dec64_1(dec64_acos, "0",        "1.570796326794897", 0);
    failed += test_dec64_1(dec64_acos, "-0.9",     "2.690565841793531", 0);
    failed += test_dec64_1(dec64_acos, "-1",       "3.141592653589793", 0);
    failed += test_dec64_1(dec64_acos, "-0.1",     "1.670963747956456", 0);
    failed += test_dec64_1(dec64_acos, "2",        "nan", 0);
    printf("dec64: Testing asin\n");
    failed += test_dec64_1(dec64_asin, "1",        "1.570796326794897", 0);
    failed += test_dec64_1(dec64_asin, "0.1",      "0.1001674211615598", 0);
    failed += test_dec64_1(dec64_asin, "0.01",     "0.01000016667416711", 0);
    failed += test_dec64_1(dec64_asin, "0.001",    "0.001000000166666742", 0);
    failed += test_dec64_1(dec64_asin, "0.0001",   "0.0001000000001666667", 0);
    failed += test_dec64_1(dec64_asin, "0.00001",  "0.00001000000000016667", 0);
    failed += test_dec64_1(dec64_asin, "0.000001", "1.000000000000167e-6", 0);
    failed += test_dec64_1(dec64_asin, "0",        "0", 0);
    failed += test_dec64_1(dec64_asin, "-0.9",     "-1.119769514998634", 0);
    failed += test_dec64_1(dec64_asin, "-1",       "-1.570796326794897", 0);
    failed += test_dec64_1(dec64_asin, "-0.1",     "-0.1001674211615598", 0);
    failed += test_dec64_1(dec64_asin, "2",        "nan", 0);
    
    report_failed(failed);

    return failed != 0;
}
