#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memset, strncpy
#include <ctype.h>

#define __IMPL_DEC_SET_STR(__dec)                               \
    int __dec##_set_str(__dec##_t *d, const char *str) {        \
        intern_##__dec##_t out;                                 \
        int err = intern_##__dec##_set_str(&out, str);          \
        if (err != 0) return err;                               \
        internal_to_##__dec(&out, d);                           \
        return 0;                                               \
    }

// Format coefficient into string, 39 digits max (IEEE 128 max is 34 digits,
// internal can be up to 38)

// Trim trailing zeros and then decimal point

#define __IMPL_DEC_GET_STR(__dec)                                       \
    int __dec##_get_str(const __dec##_t *d, char *buf, size_t bufsize) { \
        intern_##__dec##_t i;                                           \
        __dec##_to_internal(d, &i);                                     \
        return intern_##__dec##_get_str(&i, buf, bufsize);              \
    }
