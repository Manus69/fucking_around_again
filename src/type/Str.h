#ifndef STR_H
#define STR_H

#include "Intr.h"
#include "Cstr.h"
#include "../struct/Buf.h"

typedef struct Str Str;

struct Str
{
    Buf buf;
    I64 index;
};

void Str_pushc(Str * str, char c);
void Str_push_cstr_len(Str * str, const char * cstr, I64 len);
void Str_push_cstr(Str * str, const char * cstr);

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

static inline void Str_del(Str * str)
{
    Buf_del(& str->buf);
    to_zero(* str);
}

__always_inline static I64 Str_len(const Str * str)
{
    return str->index;
}

__always_inline static void Str_setc(const Str * str, I64 index, char c)
{
    Buf_set(& str->buf, index, c);
}

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

__always_inline static I64 Str_cmp(const void * lhs, const void * rhs)
{
    I64 len;

    len = min(Str_len(lhs), Str_len(rhs));

    return strncmp(Str_cstr(lhs), Str_cstr(rhs), len + 1);
}

__always_inline static U64 Str_hash(const void * ptr)
{
    return Cstr_hash_len(Str_cstr(ptr), Str_len(ptr));
}

static inline void Str_dbg(const void * ptr)
{
    Cstr_dbg(Str_cstr(ptr));
}

mem_put_gen(Str)
mem_swap_gen(Str)
Intr_gen(Str)

#endif