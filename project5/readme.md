# Project5: Impl Merkle Tree following RFC6962

### 运行环境：

处理器：11th Gen Intel(R) Core(TM) i5-11300H @ 3.10GHz

内存：16GB LPDDR4

操作系统：Win11

编译器：VS2019

代码语言：C++

### 运行方法：

在VS2019中打开sm3.h头文件、merkletree.h头文件和merkletreemain.cpp源文件，运行即可。

先输入一组叶子结点的数据，以'#'为结尾，即可构建merkletree。再输入想要验证的数据原值（非哈希值），即可验证该结点是否在树上以及树是否被改变过。
