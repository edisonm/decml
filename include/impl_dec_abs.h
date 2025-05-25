#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memset
#include <ctype.h>

#define __IMPL_DEC_ABS(__dec)                                           \
    void __dec##_abs(__dec##_t *result, const __dec##_t *a) {           \
        *result = *a;                                                   \
        /* If NaN or Inf, just clear sign bit (Inf stays Inf, NaN stays NaN) */ \
        __dec##_set_sign(result, 0);                                    \
    }
