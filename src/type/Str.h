#ifndef STR_H
#define STR_H

#include "Intr.h"
#include "Cstr.h"
#include "char.h"
#include "../struct/Buf.h"
#include "../struct/Slc.h"
#include "../struct/Vec.h"

typedef struct Str Str;

struct Str
{
    Buf buf;
    I64 index;
};

void Str_pushc(Str * str, char c);
void Str_push_cstr_len(Str * str, const char * cstr, I64 len);
void Str_push_cstr(Str * str, const char * cstr);
Vec Str_splitc(const Str * str, char c);
Vec Str_split_ln(const Str * str);

static inline Str Str_init_len(const char * cstr, I64 len)
{
    Buf buf;

    buf = Buf_init_zero(len + 1);
    Buf_set_many(& buf, 0, cstr, len);

    return (Str) {buf, len};
}

static inline Str Str_init(const char * cstr)
{
    return Str_init_len(cstr, strlen(cstr));
}

static inline Str Str_init_Buf(const Buf * buf)
{
    return Str_init_len(Buf_bytes(buf), Buf_size(buf));
}

static inline Str Str_init_Slc(const Slc * slc)
{
    return Str_init_len(Slc_ptr(slc), Slc_len(slc));
}

static inline void Str_del_Str(Str * str)
{
    Buf_del(& str->buf);
    to_zero(* str);
}

static inline void Str_del(void * ptr)
{
    Str_del_Str(ptr);
}

__attribute__ ((pure, hot))
__always_inline static I64 Str_len(const Str * str)
{
    return str->index;
}

__always_inline static void Str_setc(const Str * str, I64 index, char c)
{
    Buf_set(& str->buf, index, c);
}

__attribute__ ((pure, hot))
__always_inline static char * Str_get(const Str * str, I64 index)
{
    return Buf_get(& str->buf, index);
}

__always_inline static char * Str_cstr(const Str * str)
{
    return Buf_bytes(& str->buf);
}

__always_inline static Str Str_append(Str * lhs, const Str * rhs)
{
    Str_push_cstr_len(lhs, Str_cstr(rhs), Str_len(rhs));

    return * lhs;
}

__always_inline static Slc Str_slice(const Str * str, I64 index, I64 len)
{
    return Slc_init(Str_get(str, index), & char_Intr, len);
}

__always_inline static Slc Str_to_Slc(const Str * str)
{
    return Str_slice(str, 0, Str_len(str));
}

__always_inline static I64 Str_cmp(const void * lhs, const void * rhs)
{
    I64 len;

    len = min(Str_len(lhs), Str_len(rhs));

    return strncmp(Str_cstr(lhs), Str_cstr(rhs), len + 1);
}

__attribute__ ((pure, hot))
__always_inline static U64 Str_hash(const void * ptr)
{
    return Cstr_hash_len(Str_cstr(ptr), Str_len(ptr));
}

static inline void Str_dbg(const void * ptr)
{
    Cstr cstr;

    cstr = Str_cstr(ptr);
    Cstr_dbg(& cstr);
}

mem_put_gen(Str)
mem_swap_gen(Str)
Intr_gen(Str)

#endif