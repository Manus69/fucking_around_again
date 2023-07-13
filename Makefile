cc = gcc
flags = -Wall -Wextra -g -fsanitize=address
flags_release = -Wall -Wextra -O3 -flto -mavx -D NDEBUG
flags_link = -flto
main = main.c
src_dir = ./src
obj_dir = ./obj
exe = a.out
include_dir = $(src_dir)
headers = $(shell find $(src_dir) -name "*.h")
src = $(shell find $(src_dir) -name "*.c") $(main)
obj = $(addprefix $(obj_dir)/,$(subst .c,.o,$(notdir $(src))))
lib = libwhy.a

VPATH = ./src $(src_dir)/type $(src_dir)/struct $(src_dir)/core \
$(src_dir)/alg $(src_dir)/io $(src_dir)/dbg $(src_dir)/math

all: dir $(exe)

release: flags := $(flags_release)
release: re

lib: flags := $(flags_release)
lib: dir $(lib)

$(lib) : $(obj)
	ar rcs $(lib) $(obj)

$(exe) : $(obj)
	$(cc) $(flags) $^ -o $@

$(obj_dir)/%.o : %.c $(headers)
	$(cc) $(flags) -I $(include_dir) -c $< -o $@

dir:
	mkdir -p $(obj_dir)

clean:
	rm -f $(obj)

rm_exe:
	rm -f $(exe)

rm_lib:
	rm -f $(lib)

re: clean rm_exe rm_lib all