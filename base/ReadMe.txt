如果要使用base模块中的nanolog或者nanolog_readable，需要编译器支持c++17。
nanolog，3KW笔日志，平均时耗在2.9秒左右，输出的日志文件不可读，需要解压缩。
nanolog_readable，3KW笔日志,平均时耗在65s左右，输出两个日志文件，原始文件不可读，和nanolog一致，默认解压后可读的文件为xx.log。