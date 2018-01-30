## 简单说明

cstl是使用c语言编写的类似c++容器接口的通用库

## 构建

这个库不依赖于外部库，但是他的测试实现依赖于check库，并且里面的c实现需要编译器支持c99标准，经过测试gcc时符合的

    make
  
生成libcstl.a, libcstl.so到lib目录中，生成test_cstl到test目录中，生成依赖规则到dep目录中

    make test
  
调用test/test_cstl来进行测试

    make debug=1

生成上面库的debug版本，就是在编译选项上加上一些编译调试选项，会生成一些调试信息。

    make install
    
拷贝头文件到/usr/include/cstl目录中，拷贝lib/libcstl.a, lib/libcstl.so到/usr/lib目录中， 这两个目录可以在make install中指定install_header_dir和install_lib_dir具体的值来进行设置

## 编译后目录说明

- build 保存编译过程中生成的.o文件
- dep 生成编译器是个的各个源文件的依赖关系，然后使用sed命令替换生成的对应.d文件的依赖关系
- docs 文档文件目录，默认为空，可以允许doxygen来生成文档，当前所有文档的编写还没有完成, 文档会被生成到docs文档中。
- include 所有库头文件目录
- src 所有库源文件目录
- test 所有库测试文件目录，其中test_main.c包含test的main函数
- lib 最终生成的库文件所在的目录
- makefile, config.mk 这个文件会安装这个整个目录编写编译规则，要求所有的库文件都要放在src目录中，所有的测试文件都要放在test目录中，那么就可以做到不用修改一行makefile代码就可能编译整个库，测试整个库，并且自动导入依赖关系（哈哈，其实写makefile也可以实现类似ide的自动依赖扫描，自动编译的）

## 各个容器的功能说明和使用简介

### 通用的设计原则

内部使用void\*来作为元素类型，并且会记录每一种类型在内存中占用的尺寸。所以也就形成一套统一的原则，比如添加元素的时候，传过来的参数只是参考，内部元素
并不会拷贝他，而是通过内存拷贝，将这个参数的实际拷贝拷贝到容器内部分配的内存中，所以这样容器内部元素和原来的传过来的参数指针是没有任何关系的，这样不
就模拟了c++中，比如vector::push_back，当使用一个对象作为参数的时候，内部新产生的元素和外面的元素他们之间只是内容相同，其他病没有什么影响。

当然设计角度，c语言可以使用void\*类型，作为添加元素的时候，直接在外面使用malloc分配内存，然后将分配到的内存直接移动到容器内部也是可以的，但是设计
时候是那样考虑的，比如如果容器内部是short类型，如果传递参数，是强制转换为void\*吗，如果是的话，那么问题就来了，如果像获取short类型的容器的元素，
因为你使用void\*来作为所有数据类型，那么对于vector你应该使用这样设计的int vector_get(vector_t \*vec, size_t index, void \*\*data)，你一旦
将short变量的地址作为这儿函数的第三个参数使用，内存就会存在，内存访问越界，因为他内部的逻辑应该是\*data = 具体元素值（并且类型是void\*类型，在32位
系统上占用4个字节，在64位系统上占用8个字节），但是short只占用两个字节，这样就肯定会发生内存越界访问，出现一些让你莫名其妙的问题。

但是如果按照第一个设计来考虑的话，你只需要这样设计vector_get函数，int vector_get(vector_t \*vec, size_t index, void \*data), 内部使用
memcpy(data, 元素地址，元素长度), 这样数据就不会产生越界访问。我也可以这样设计，因为内部维护自己的内存，所以如果元素存在的话，那么元素
地址肯定不为NULL，否则元素不存在的时候返回NULL，所以直接这样写，void\* vector_get(vector_t \*vec, size_t index)，不用使用int来作为返回
值区分不存在的情况，这个设计通过检测返回值是否是NULL来检测指定索引元素是否存在。

因为一般在c语言中存放到容器中的对象一般是指针，所以第二种设计也是一种合适的方案，本库采用第一种方案来保存元素值。

### vec容器

类似于c++中的vector，内部占用内存会根据需求来动态增长，实现增加，删除尾部元素，时间复杂度是O(1)。

#### 工厂函数

