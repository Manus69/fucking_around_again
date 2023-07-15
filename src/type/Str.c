#include "Str.h"

__always_inline static I64 _capacity(const Str * str)
{
    return Buf_size(& str->buf) - Str_len(str) - 1;
}

__always_inline static void _add_capacity(Str * str, I64 capacity)
{
    Buf_extend_zero(& str->buf, capacity);
}

__always_inline static void _check_add_capacity(Str * str, I64 capacity)
{
    capacity = capacity > Str_len(str) ? capacity : Str_len(str);
    _add_capacity(str, capacity);
}

__always_inline void Str_pushc(Str * str, char c)
{
    if (_capacity(str) == 0) _add_capacity(str, Str_len(str));

    Str_setc(str, str->index ++, c);
}

__always_inline void Str_push_cstr_len(Str * str, const char * cstr, I64 len)
{
    if (_capacity(str) < len) _check_add_capacity(str, len);

    Buf_set_many(& str->buf, str->index, cstr, len);
}

__always_inline void Str_push_cstr(Str * str, const char * cstr)
{
    Str_push_cstr_len(str, cstr, strlen(cstr));
}


