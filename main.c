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
    for (I64 k = 0; k < len; k ++)
    {
        Vec_push(& v, len - k);
    }

    I64_dbg(Vec_last(& v));
    Vec_del(& v);
}
int main()
{
    // Arr_test(1 << 10);
    // Vec_test(1 << 25);
    printf("%zu\n", sizeof(Vec));
}