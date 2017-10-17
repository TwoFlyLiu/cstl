vpath %.c src:test
vpath %.h include:test
vpath %.o build
vpath %.d dep
vpath %.a lib
vpath %.md docs

CC = gcc

# 这儿在release情况下，不定义NDEBUG，主要是为了
# 方便在使用库的时候，使用cstl_leak_test()来兼容某个容器是否
# 缺少调用对应的free函数，方便来检测内存泄露
debug ?= 0
ifeq ($(debug),1)
	#CFLAGS = -Wall -O3 -DNDEBUG  $(include_dirs)
	CFLAGS = -Wall -O3 $(include_dirs)
else
	CFLAGS = -Wall -g $(include_dirs)
endif

ARFLAGS = rcs
RM = rm -rf

LDFLAGS = $(lib_dirs)
LOADLIBES = $(static_libs)
