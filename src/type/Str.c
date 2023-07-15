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
    if (unlikely(_capacity(str) == 0)) _add_capacity(str, Str_len(str));

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

Slc_find_gen(char)

Slc Slc_split_nextc(Slc * slc, char c)
{
    I64 index;
    Slc chop;

    index = Slc_find_char(slc, c);
    if (index == NO_INDEX) return Slc_chop_all(slc);

    chop = Slc_chop_front(slc, index);
    Slc_shift(slc, 1);

    return chop;
}

Vec Str_splitc(const Str * str, char c)
{
    Vec strings;
    Slc slc;
    Slc chop;

    strings = Vec_init(Str);
    slc = Str_to_Slc(str);
    while (Slc_empty(& slc) == false)
    {
        chop = Slc_split_nextc(& slc, c);
        Vec_push(& strings, Str_init_Slc(& chop));
    }

    return strings;
}

Vec Str_split_ln(const Str * str)
{
    return Str_splitc(str, '\n');
}