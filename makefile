-include config.mk

include_dirs = -I./include

lib_file = libcstl.a
lib = lib/$(lib_file)
test = test/test_cstl

# 可以定义下面两个变量来定义安装头目录和安装库目录
install_header_dir ?= /usr/include/cstl
install_lib_dir ?= /usr/lib

lib_sources = $(wildcard src/*.c)
test_sources = $(wildcard test/*.c)

lib_objects = $(patsubst src/%.c,build/%.o,$(lib_sources))
test_objects = $(patsubst test/%.c,build/%.o,$(test_sources))

all: $(lib) $(test)

# $?表示依赖比目标新的依赖文件
$(lib): $(lib_objects)
	$(AR) $(ARFLAGS) $@ $^

# 生成测试程序
$(test): LOADLIBES= -lcstl -lcheck -liconv
$(test): LDFLAGS = -Llib
$(test): $(test_objects) $(lib)
	$(CC) $(LDFLAGS) -o $(test) $(test_objects) $(LOADLIBES) $(LDLIBS)

# 模式规则
build/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

build/%.o: test/%.c
	$(CC) -c $(CFLAGS) $(extra_include_dirs) $< -o $@

# 上下两条规则，同时存在，不会出现覆盖问题
dep/%.d: src/%.c
	@set -e; $(CC) -MM $(CFLAGS) $< >/tmp/$$$$;\
		sed -E 's,(\w+).o[ :]+,build/\1.o dep/\1.d : ,g' </tmp/$$$$ >$@

dep/%.d: test/%.c
	@set -e; $(CC) -MM $(CFLAGS) $< >/tmp/$$$$;\
		sed -E 's,(\w+).o[ :]+,build/\1.o dep/\1.d : ,g' </tmp/$$$$ >$@

# 导入编译器自动生成的依赖文件(并且会检测依赖规则，如果过期则重新生成)
include $(patsubst src/%.c,dep/%.d,$(lib_sources)) $(patsubst test/%.c,dep/%.d,$(test_sources))

.PHONY: all clean test install uninstall

clean:
	$(RM) build/*.o
	$(RM) dep/*.d

# 值保留一个老的版本
install: $(lib)
	rm -rf $(install_lib_dir)/$(lib_file).old;[ -e $(install_lib_dir)/$(lib_file) ] && mv $(install_lib_dir)/$(lib_file) $(install_lib_dir)/$(lib_file).old; cp $(lib) $(install_lib_dir)
	rm -rf $(install_header_dir).old; [ -e $(install_header_dir) ] && mv $(install_header_dir) $(install_header_dir).old;mkdir $(install_header_dir); cp include/*.h $(install_header_dir)

# 后面有意返回true, 这样make会执行后面的命令, 是根据整个命令行执行结果来判断是否执行下一行命令
uninstall: 
	rm -rf $(install_header_dir); [ -e $(install_header_dir).old ] && mv $(install_header_dir).old $(install_header_dir); true
	rm -rf $(install_lib_dir)/$(lib_file); [ -e $(install_lib_dir)/$(lib_file).old ] && mv $(install_lib_dir)/$(lib_file).old $(install_lib_dir)/$(lib_file); true

# 启用单进程模式进行测试(设置CK_FORK=no, 默认是多进程的，就是使用fork), 
test: $(test)
	./$(test)
