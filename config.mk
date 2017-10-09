vpath %.c src:test
vpath %.h include:test
vpath %.o build
vpath %.d dep
vpath %.a lib
vpath %.md docs

CC = gcc

debug ?= 0
ifeq ($(debug),0)
	CFLAGS = -Wall -O3 -DNDEBUG  $(include_dirs)
else
	CFLAGS = -Wall -g $(include_dirs)
endif

ARFLAGS = rcs
RM = rm -rf

LDFLAGS = $(lib_dirs)
LOADLIBES = $(static_libs)
