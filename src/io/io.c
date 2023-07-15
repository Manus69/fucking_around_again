#include "io.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

Buf io_read_file_fd(int fd)
{
    struct stat stat;
    Buf         buf;

    fstat(fd, & stat);
    buf = Buf_init(stat.st_size);
    read(fd, Buf_bytes(& buf), stat.st_size);

    return buf;
}

Buf io_read_file(const char * name)
{
    int fd;

    if ((fd = open(name, O_RDONLY)) < 0) return Buf_init_zero(0);

    return io_read_file_fd(fd);
}

Str io_read_txt_file(const char * name)
{
    Buf buf;
    Str str;

    buf = io_read_file(name);
    str = Str_init_Buf(& buf);
    Buf_del(& buf);

    return str;
}

Vec io_read_txt_file_ln(const char * name)
{
    Str str;
    Vec vec;

    str = io_read_txt_file(name);
    vec = Str_split_ln(& str);
    Str_del(& str);

    return vec;
}