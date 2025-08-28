#include "pl-dec.h"
#include <dec64.h>
#include <dec128.h>
#include <dec64_dec128.h>
#include <string.h>
#include <stdio.h>
#include <foreign_interface.h>

/* #define dec64_to_dec64(  a, b) (*(b) = *(a)) */
/* #define dec128_to_dec128(a, b) (*(b) = *(a)) */

#define __cast_to_internal_dec128(ref, v) ({                            \
            if ((PL_new_atom("$dec64")==name)&&(arity==1)) {            \
                dec64_t src;                                            \
                term_t a = PL_new_term_ref();                           \
                __rtcheck(PL_get_arg(1, v, a));                         \
                __rtcheck(PL_get_int64(a, (int64_t *)&src));            \
                dec64_to_internal_dec128(&src, &ref);                   \
                break;                                                  \
            }})

#define __cast_to_internal_dec64(ref, v) ({                             \
            if ((PL_new_atom("$dec128")==name)&&(arity==2)) {           \
                dec128_t src;                                           \
                term_t a = PL_new_term_ref();                           \
                term_t b = PL_new_term_ref();                           \
                __rtcheck(PL_get_arg(1, v, a));                         \
                __rtcheck(PL_get_arg(2, v, b));                         \
                __rtcheck(PL_get_int64(a, 0+(int64_t *)&src));          \
                __rtcheck(PL_get_int64(b, 1+(int64_t *)&src));          \
                dec128_to_internal_dec64(&src, &ref);                   \
                break;                                                  \
            }})

#define GEN_DEC_caster(__type, _)                                       \
    foreign_t pl_dec##__type(term_t v, term_t t) {                      \
        intern_dec##__type##_t ref;                                     \
        switch (PL_term_type(v)) {                                      \
        case PL_VARIABLE:                                               \
            break;                                                      \
        case PL_INTEGER:                                                \
        {                                                               \
            long long i;                                                \
            int result = PL_get_int64(v, (int64_t *)&i);                \
            if (!result) return FALSE;                                  \
            ref.sign=(i<0?1:0);                                         \
            ref.coeff=(i<0?-i:i);                                       \
            ref.exponent=0;                                             \
            ref.special=DEC_NORMAL;                                     \
            break;                                                      \
        }                                                               \
        case PL_ATOM:                                                   \
        {   char *c;                                                    \
            __rtcheck(PL_get_atom_chars(v, &c));                        \
            if (intern_dec##__type##_set_str(&ref, c)!=0) return FALSE; \
            break;                                                      \
        }                                                               \
        case PL_STRING:                                                 \
        {   char *c;                                                    \
            size_t len;                                                 \
            __rtcheck(PL_get_string_chars(v, &c, &len));                \
            if (intern_dec##__type##_set_str(&ref, c)!=0) return FALSE; \
            break;                                                      \
        }                                                               \
        case PL_FLOAT:                                                  \
        {   double d;                                                   \
            __rtcheck(PL_get_float(v, &d));                             \
            char c[32];                                                 \
            sprintf(c, "%12g", d);                                      \
            if (intern_dec##__type##_set_str(&ref, c)!=0) return FALSE; \
            break;                                                      \
        }                                                               \
        case PL_TERM:                                                   \
        {   atom_t name;                                                \
            size_t arity;                                               \
            __rtcheck(PL_get_name_arity(v, &name, &arity));             \
            if ((PL_new_atom("$dec"#__type)==name)&&(arity==__type/64)) { \
                return PL_unify(t, v);                                  \
            } else {                                                    \
                __cast_to_internal_dec##__type(ref, v);                 \
            }                                                           \
            return FALSE;                                               \
        }                                                               \
        default:                                                        \
            return FALSE;                                               \
        }                                                               \
        dec##__type##_t res;                                            \
        internal_to_dec##__type(&ref, &res);                            \
        return FI_unify_dec##__type##_t(t, &res);                       \
    }

#define GEN_DEC_pl_2(__type, __func)                                    \
    foreign_t pl_dec##__type##_##__func(term_t res, term_t x) {         \
        dec##__type##_t a;                                              \
        dec##__type##_t b;                                              \
        if (PL_get_dec##__type##_t(x, &a)) {                            \
            dec##__type##_##__func(&b, &a);                             \
            return FI_unify_dec##__type##_t(res, &b);                   \
        }                                                               \
        return FALSE;                                                   \
    }

#define GEN_DEC_pl_3(__type, __func)                                    \
    foreign_t pl_dec##__type##_##__func(term_t res, term_t x, term_t y) { \
        dec##__type##_t a, b, c;                                        \
        if (FI_get_dec##__type##_t(NULL, x, &a)                         \
            && FI_get_dec##__type##_t(NULL, y, &b)) {                   \
            dec##__type##_##__func(&c, &a, &b);                         \
            return FI_unify_dec##__type##_t(res, &c);                   \
        }                                                               \
        return FALSE;                                                   \
    }

#define GEN_DEC_pn_3(__type, __func)                                    \
    foreign_t pn_dec##__type##_##__func(term_t res, term_t x, term_t y) { \
        dec##__type##_t a, b, c;                                          \
        if (FI_get_dec##__type##_t(NULL, x, &a)                         \
            && FI_get_dec##__type##_t(NULL, y, &b)) {                   \
            dec##__type##_##__func(&c, &a, &b);                         \
            return FI_unify_dec##__type##_t(res, &c);                   \
        }                                                               \
        return FALSE;                                                   \
    }

#define GEN_DEC_is_2(__type, __func)                                    \
    foreign_t is_dec##__type##_##__func(term_t x, term_t y) {           \
        dec##__type##_t a, b;                                           \
        if (FI_get_dec##__type##_t(NULL, x, &a)                         \
            && FI_get_dec##__type##_t(NULL, y, &b)) {                   \
            return dec##__type##_##__func(&a, &b);                      \
        }                                                               \
        return FALSE;                                                   \
    }

#define GEN_DEC_pi_2(__type, __func)                                    \
    foreign_t pi_dec##__type##_##__func(term_t res, term_t x) {         \
        dec##__type##_t a;                                              \
        DEC_SINT64 b;                                                   \
        if (FI_get_dec##__type##_t(NULL, x, &a)) {                      \
            dec##__type##_to_int64_##__func(&b, &a);                    \
            return PL_unify_int64(res, (int64_t)b);                     \
        }                                                               \
        return FALSE;                                                   \
    }

#define GEN_DEC_pt_2(__type, __func)                                    \
    foreign_t pt_dec##__type##_##__func(term_t t, term_t r) {           \
        char s[__type/2];                                               \
        dec##__type##_t v;                                              \
        __rtcheck(PL_get_dec##__type##_t(t, &v));                       \
        dec##__type##_get_str(&v, s, __type/2);                         \
        __rtcheck(PL_unify_##__func##_chars(r, s));                     \
        return TRUE;                                                    \
    }

int FI_unify_dec64_t(term_t t, dec64_t * const v)
{
    dec64_t src;
    switch (PL_term_type(t)) {
    case PL_VARIABLE:
        return PL_unify_term(t,
                             PL_FUNCTOR_CHARS,
                             "$dec64", 1,
                             PL_INT64,
                             *v);
    case PL_TERM:
        return PL_get_dec64_t(t, &src)
            && (src.bits == v->bits);
    }
    return FALSE;
}

int FI_unify_dec128_t(term_t t, dec128_t * const v)
{
    dec128_t src;
    const __uint128_t MASK_64 = (__uint128_t)0xFFFFFFFFFFFFFFFFULL;
    switch (PL_term_type(t)) {
    case PL_VARIABLE:
        return PL_unify_term(t,
                             PL_FUNCTOR_CHARS,
                             "$dec128", 2,
                             PL_INT64,
                             (uint64_t)(v->bits & MASK_64),
                             PL_INT64,
                             (uint64_t)(v->bits >> 64));
    case PL_TERM:
        return PL_get_dec128_t(t, &src)
            && (src.bits == v->bits);
    }
    return FALSE;
}

int PL_get_dec64_t(term_t t, dec64_t *v) {
    atom_t name;
    size_t arity;
    term_t a = PL_new_term_ref();
    return PL_get_name_arity(t, &name, &arity)
        && (arity == 1) && (PL_new_atom("$dec64")==name)
        && PL_get_arg(1, t, a) && PL_get_int64(a, (int64_t *)&(v->bits));
}

int PL_get_dec128_t(term_t t, dec128_t *v) {
    atom_t name;
    size_t arity;
    term_t a = PL_new_term_ref();
    term_t b = PL_new_term_ref();
    int64_t w[2];
    int result = PL_get_name_arity(t, &name, &arity)
        && (arity == 2) && (PL_new_atom("$dec128")==name)
        && PL_get_arg(1, t, a) && PL_get_int64(a, (int64_t *)&(w[0]))
        && PL_get_arg(2, t, b) && PL_get_int64(b, (int64_t *)&(w[1]));
    v->bits = w[0] + ((__uint128_t)w[1] << 64);
    return result;
}

#define GEN_DEC_ALL(__pre, __func) \
    GEN_DEC_##__pre( 64, __func) \
    GEN_DEC_##__pre(128, __func)

GEN_DEC_ALL(caster, _)

#include "pl-dec_auto.h"
