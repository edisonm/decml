#include "pl-dec.h"
#include <dec64.h>
#include <dec128.h>
#include <dec64_dec128.h>
#include <string.h>
#include <stdio.h>
#include <foreign_interface.h>

#define __IMPL_INTERN_dec_OUT_STR(__type)                               \
    int intern_dec##__type##_out_str(IOSTREAM *stream,                  \
                                     const intern_dec##__type##_t *d) { \
        char s[__type/2];                                               \
        if (intern_dec##__type##_get_str(d, s, __type/2))               \
            return FALSE;                                               \
        if (Sfputs(s, stream) == EOF)                                   \
            return FALSE;                                               \
        return TRUE;                                                    \
    }

#define __IMPL_dec_OUT_STR(__dec)                                       \
    int __dec##_out_str(IOSTREAM *stream, const __dec##_t *d) {         \
        intern_##__dec##_t i;                                           \
        __dec##_to_internal_##__dec(d, &i);                             \
        return intern_##__dec##_out_str(stream, &i);                    \
    }

#define GEN_dec_write_ref(__type)                               \
    static int write_dec##__type##_ref(IOSTREAM *stream,        \
                                       atom_t aref, int flags)  \
    {                                                           \
        dec##__type##_t *ref = PL_blob_data(aref, NULL, NULL);  \
        return dec##__type##_out_str(stream, ref);              \
    }

#define GEN_dec_release(__type)                                 \
    static int release_dec##__type(atom_t aref)                 \
    {                                                           \
        dec##__type##_t *ref = PL_blob_data(aref, NULL, NULL);  \
        PL_free(ref);                                           \
        return TRUE;                                            \
    }

#define GEN_dec_aquire(__type)                                  \
    static void aquire_dec##__type(atom_t aref)                 \
    {                                                           \
        dec##__type##_t *ref = PL_blob_data(aref, NULL, NULL);  \
        (void) ref;                                             \
    }

#define GEN_dec___record(__type)                \
    static PL_blob_t __record_dec##__type##_t = \
    {                                           \
        PL_BLOB_MAGIC,                          \
        PL_BLOB_NOCOPY,                         \
        "dec"#__type,                           \
        release_dec##__type,                    \
        NULL,                                   \
        write_dec##__type##_ref,                \
        aquire_dec##__type                      \
    };

#define GEN_dec_record(__type)                                  \
    PL_blob_t *record_dec##__type##_t = &__record_dec##__type##_t;

#define GEN_dec_is(__type)                                       \
    foreign_t is_dec##__type##_t(term_t v) {                     \
        void *src;                                               \
        PL_blob_t *type;                                         \
        return PL_get_blob(v, (void *)&src, NULL, &type)         \
            && type == record_dec##__type##_t;                   \
    }

#define GEN_dec_caster(__type)                                          \
    foreign_t pl_dec##__type(term_t v, term_t t) {                      \
        intern_dec##__type##_t ref;                                     \
        switch (PL_term_type(v)) {                                      \
        case PL_VARIABLE:                                               \
            ref = intern_dec##__type##_zero;                            \
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
        case PL_BLOB:                                                   \
        {   PL_blob_t *type;                                            \
            if (is_dec##__type##_t(v)) {                                \
                return PL_unify(t, v);                                  \
            } else                                                      \
                return FALSE;                                           \
        }                                                               \
        default:                                                        \
            return FALSE;                                               \
        }                                                               \
        dec##__type##_t *res = PL_malloc(sizeof(dec##__type##_t));      \
        internal_to_dec##__type(&ref, res);                             \
        return FI_unify_dec##__type##_t(t, res);                        \
    }

#define GEN_dec_pl_2(__type, __func)                                    \
    foreign_t pl_dec##__type##_##__func(term_t res, term_t x) {         \
        dec##__type##_t *a;                                             \
        if (PL_get_dec##__type##_t(x, &a)) {                            \
            dec##__type##_t *b = PL_malloc(sizeof(dec##__type##_t));    \
            dec##__type##_##__func(b, a);                               \
            return FI_unify_dec##__type##_t(res, b);                    \
        }                                                               \
        return FALSE;                                                   \
    }

#define GEN_dec_pl_3(__type, __func)                                    \
    foreign_t pl_dec##__type##_##__func(term_t res, term_t x, term_t y) { \
        dec##__type##_t *a, *b;                                         \
        if (FI_get_dec##__type##_t(NULL, x, &a)                         \
            && FI_get_dec##__type##_t(NULL, y, &b)) {                   \
            dec##__type##_t *c = PL_malloc(sizeof(dec##__type##_t));    \
            dec##__type##_##__func(c, a, b);                            \
            return FI_unify_dec##__type##_t(res, c);                    \
        }                                                               \
        return FALSE;                                                   \
    }

#define GEN_dec_pn_3(__type, __func)                                    \
    foreign_t pn_dec##__type##_##__func(term_t res, term_t x, term_t y) { \
        dec##__type##_t *a, *b;                                         \
        if (FI_get_dec##__type##_t(NULL, x, &a)                         \
            && FI_get_dec##__type##_t(NULL, y, &b)) {                   \
            dec##__type##_t *c = PL_malloc(sizeof(dec##__type##_t));    \
            dec##__type##_##__func(c, a, b);                            \
            return FI_unify_dec##__type##_t(res, c);                    \
        }                                                               \
        return FALSE;                                                   \
    }

#define GEN_dec_is_2(__type, __func)                                    \
    foreign_t is_dec##__type##_##__func(term_t x, term_t y) {           \
        dec##__type##_t *a, *b;                                         \
        if (FI_get_dec##__type##_t(NULL, x, &a)                         \
            && FI_get_dec##__type##_t(NULL, y, &b)) {                   \
            return dec##__type##_##__func(a, b);                        \
        }                                                               \
        return FALSE;                                                   \
    }

#define GEN_dec_pi_2(__type, __func)                                    \
    foreign_t pi_dec##__type##_##__func(term_t res, term_t x) {         \
        dec##__type##_t *a;                                             \
        if (FI_get_dec##__type##_t(NULL, x, &a)) {                      \
            DEC_SINT64 b;                                               \
            dec##__type##_to_int64_##__func(&b, a);                     \
            return PL_unify_int64(res, (int64_t)b);                     \
        }                                                               \
        return FALSE;                                                   \
    }

#define GEN_dec_pt_2(__type, __func)                                    \
    foreign_t pt_dec##__type##_##__func(term_t t, term_t r) {           \
        char s[__type/2];                                               \
        dec##__type##_t *v;                                             \
        __rtcheck(PL_get_dec##__type##_t(t, &v));                       \
        dec##__type##_get_str(v, s, __type/2);                          \
        __rtcheck(PL_unify_##__func##_chars(r, s));                     \
        return TRUE;                                                    \
    }

#define GEN_dec_pl_unify(__type)                                        \
    int PL_unify_dec##__type##_t(term_t t, dec##__type##_t *fr) {       \
        return PL_unify_blob(t, fr, sizeof(dec##__type##_t),            \
                             record_dec##__type##_t);                   \
    }

#define GEN_dec_pl_get(__type)                                          \
    int PL_get_dec##__type##_t(term_t t, dec##__type##_t **v) {         \
        PL_blob_t *type;                                                \
        if (PL_get_blob(t, (void **)v, NULL, &type)                     \
            && type == record_dec##__type##_t)                          \
            return TRUE;                                                \
        return FALSE;                                                   \
    }

#define GEN_dec_ALL(__pre, __func) \
    GEN_dec_##__pre( 64, __func)   \
    GEN_dec_##__pre(128, __func)

#define GEN_dec_ALL_1(__pre) \
    GEN_dec_##__pre( 64)     \
    GEN_dec_##__pre(128)

__IMPL_INTERN_dec_OUT_STR(64)
__IMPL_INTERN_dec_OUT_STR(128)
__IMPL_dec_OUT_STR(dec64)
__IMPL_dec_OUT_STR(dec128)

GEN_dec_ALL_1(caster)
GEN_dec_ALL_1(write_ref)
GEN_dec_ALL_1(release)
GEN_dec_ALL_1(aquire)
GEN_dec_ALL_1(__record)
GEN_dec_ALL_1(record)
GEN_dec_ALL_1(is)
GEN_dec_ALL_1(pl_unify)
GEN_dec_ALL_1(pl_get)

#include "pl-dec_auto.h"
