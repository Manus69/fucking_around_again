#include "dbg/dbg.h"
#include "type/I64.h"

#include "struct/Arr.h"
void Arr_test(I64 len)
{
    Arr a = Arr_init(I64, len);
    for (I64 k = 0; k < len; k ++)
    {
        Arr_set(& a, k, len - k);
    }

    // I64 x = deref(I64) Arr_get(& a, len - 1);
    // I64_dbg(& x);
    Arr_dbg(& a);

    Arr_del_struct(& a);
}

#include "struct/Vec.h"
void Vec_test(I64 len)
{
    Vec v = Vec_init(I64);
    Vec_reserve(& v, len);

    for (I64 k = 0; k < len; k ++)
    {
        Vec_push(& v, len - k);
    }

    Vec_push(& v, (I64) -1);
    I64_dbg(Vec_last(& v));
    Vec_del_struct(& v);
}

#include "alg/sort.h"
void sort_test(I64 len)
{
    Vec v = Vec_init(I64);
    for (I64 k = 0; k < len; k ++)
    {
        Vec_push(& v, len - k);
    }

    Slc s = Vec_to_Slc(& v);
    // Slc_dbg(& s);
    // sort_merge_slice_cmp(& s, I64_cmp);
    sort(& v, Vec, I64);
    // sort_insert_slice_cmp(& s, I64_cmp);
    // Slc_dbg(& s);
    I64_dbg(Slc_last(& s));

    // Vec_del_struct(& v);
    Vec_erase(& v);
}

void sort_test2(I64 len)
{
    Vec v = Vec_init(I64);
    for (I64 k = 0; k < len; k ++)
    {
        Vec_push(& v, len - k);
    }

    Slc slc = Vec_to_Slc(& v);
    // sort_insert_slice_cmp(& slc, I64_cmp);
    sort_ins_test(Slc_first(& slc), Slc_len(& slc));
    // Vec_dbg(& v);
    I64_dbg(Vec_last(& v));

    Vec_del(& v);
}

#include "type/Str.h"
#include "type/Cstr.h"
#include "io/io.h"
#define FNAME "text_file.txt"
void Str_test()
{
    // char * x = "eat ass";
    // Cstr_dbg(& x);
    Vec v = io_read_txt_file_ln(FNAME);
    sort(& v, Vec, Str);
    // Vec_dbg(& v);
    Str_dbg(Vec_last(& v));
    Vec_erase(& v);
}

#include "struct/Pair.h"
void Pair_test()
{
    Pair p = Pair_init(Str, Str);
    Pair_set(& p, Str_init("eat"), Str_init("dick"));

    Str_dbg(Pair_first(& p));
    Str_dbg(Pair_second(& p));

    Pair_erase(& p);
}

#include "struct/Set.h"
void Set_test()
{
    Set s = Set_init(I64);

    I64 n = 1 << 25;
    for (I64 k = 0; k < n; k ++)
    {
        Set_insert(& s, n - k);
    }
    // Set_dbg(& s);

    for (I64 k = 0; k < n; k ++)
    {
        Set_insert(& s, n - k);
    }

    // Set_dbg(& s);

    for (I64 k = 0; k < n; k ++)
    {
        Set_remove(& s, k);
    }

    I64_dbg(Set_any(& s));
    // Set_dbg(& s);
    Set_del(& s);
}

//
//refactor __ to __attr__ (...)
//static in intr * ?
//
int main()
{
    // Arr_test(1 << 10);
    // Vec_test(1 << 25);
    // sort_test(1 << 25);
    // sort_test2(1 << 16);
    // Str_test();
    // Pair_test();
    Set_test();
}