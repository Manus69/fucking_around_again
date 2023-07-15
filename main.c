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

    Arr_del(& a);
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
    Vec_del(& v);
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

    Vec_del(& v);
}

#include "type/Str.h"
void Str_test()
{
    
}

int main()
{
    // Arr_test(1 << 10);
    // Vec_test(1 << 25);
    sort_test(1 << 25);
}