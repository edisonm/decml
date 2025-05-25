#include <stdio.h>
#include <string.h>
#include "dec64.h"
#include "test_dec64.h"

int main(void) {
    int failed = 0;
    printf("testing log10 dec64\n");
    failed += test_dec64_1(dec64_log10, "2",    "0.3010299956639812", 0); // approximate
    failed += test_dec64_1(dec64_log10, "0.5", "-0.3010299956639812", 0); // approximate
    failed += test_dec64_1(dec64_log10, "20",   "1.301029995663981", 0); // approximate
    failed += test_dec64_1(dec64_log10, "200",  "2.301029995663981", 0); // approximate
    failed += test_dec64_1(dec64_log10, "2000", "3.301029995663981", 0); // approximate
    failed += test_dec64_1(dec64_log10, "1e100",   "100", 0);
    failed += test_dec64_1(dec64_log10, "1e-100", "-100", 0);
    failed += test_dec64_1(dec64_log10, "1.84e100", "100.2648178230095", 0);
    failed += test_dec64_1(dec64_log10, "3", "0.4771212547196624", 0);
    failed += test_dec64_1(dec64_log10,  "0", "-inf", 0); // error expected
    failed += test_dec64_1(dec64_log10, "-1", "nan", 0); // error expected
    
    report_failed(failed);

    return failed != 0;
}
