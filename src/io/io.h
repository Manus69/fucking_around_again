#ifndef IO_H
#define IO_H

#include "../struct/Buf.h"
#include "../struct/Vec.h"
#include "../type/Str.h"

Buf io_read_file_fd(int fd);
Buf io_read_file(const char * name);
Str io_read_txt_file(const char * name);
Vec io_read_txt_file_ln(const char * name);

#endif