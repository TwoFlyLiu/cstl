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
ifeq ($(debug),0)
	CFLAGS = -Wall -O3 -DNDEBUG $(include_dirs)
else
	CFLAGS = -Wall -g  $(include_dirs)
endif

# 如果是LINUX则为编译器预定义LINUX宏
ifeq ($(shell uname),Linux)
	linux = 1
	CFLAGS += -DLINUX
else
	linux = 0
endif

ARFLAGS = rcs
RM = rm -rf

LDFLAGS = $(lib_dirs)
LOADLIBES = $(static_libs)
