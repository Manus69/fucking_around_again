#include "sort.h"

__attribute__ ((nonnull, flatten))
void sort_insert_slice_cmp(Slc * slc, Cmp cmp)
{
    I64 len;
    I64 current;
    Ptr lhs;
    Ptr rhs;

    len = Slc_len(slc);
    for (I64 sorted = 1; sorted < len; sorted ++)
    {
        current = sorted - 1;
        while (current >= 0)
        {
            lhs = Slc_get(slc, current);
            rhs = Slc_get(slc, current + 1);

            if (cmp(lhs, rhs) < 0) break ;

            Intr_swap(Slc_item_intr(slc))(lhs, rhs);
            -- current;
        }
    }
}

void sort_ins_test(I64 * ptr, I64 len)
{
    I64 current;

    for (I64 sorted = 1; sorted < len; sorted ++)
    {
        current = sorted - 1;
        while (current >= 0)
        {
            if (ptr[current] <= ptr[current + 1]) break;

            mem_swap_t(ptr + current, ptr + current + 1, I64);
            --current;
        }    
    }
}