#ifndef SORT_H
#define SORT_H

#include "../struct/Slc.h"
#define sort(ptr, container_t, item_t) \
{Slc _s = container_t##_to_Slc(ptr); sort_merge_slice_cmp(& _s, item_t##_cmp);}

void sort_merge_slice_cmp(Slc * slc, Cmp cmp);
void sort_insert_slice_cmp(Slc * slc, Cmp cmp);

#endif