#include "sort.h"

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