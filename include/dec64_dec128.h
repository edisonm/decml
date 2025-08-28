#ifndef DEC64_DEC128_H
#define DEC64_DEC128_H

#include "dec64.h"
#include "dec128.h"

void dec64_to_internal_dec128(const dec64_t *d, intern_dec128_t *out);
void dec128_to_internal_dec64(const dec128_t *d, intern_dec64_t *out);

#endif /* DEC64_DEC128_H */

