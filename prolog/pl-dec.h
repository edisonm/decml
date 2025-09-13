#ifndef __pl_dec_H
#define __pl_dec_H

#include <dec64.h>
#include <dec128.h>
#include <SWI-Stream.h>
#include <SWI-Prolog.h>

#define FI_get_dec64_t(_,  t, v) PL_get_dec64_t( (t), (v))
#define FI_get_dec128_t(_, t, v) PL_get_dec128_t((t), (v))

#define FI_unify_dec64_t(t, v) PL_unify_dec64_t((t), (v))
#define FI_unify_dec128_t(t, v) PL_unify_dec128_t((t), (v))

int PL_unify_dec64_t(term_t t, dec64_t * const b);
int PL_unify_dec128_t(term_t t, dec128_t * const b);
int PL_get_dec64_t( term_t t, dec64_t **b);
int PL_get_dec128_t(term_t t, dec128_t **b);
foreign_t is_dec64_t(term_t v);
foreign_t is_dec128_t(term_t v);

#endif
