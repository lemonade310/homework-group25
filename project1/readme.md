#Project1: implement the naïve birthday attack of reduced SM3

运行环境：

处理器：11th Gen Intel(R) Core(TM) i5-11300H @ 3.10GHz

内存：16GB LPDDR4

操作系统：Win11

编译器：VS2019

代码语言：C++

运行方法：

在VS2019中打开sm3.h头文件与birthattacksm3.cpp源文件，运行即可。

因数据过大，可以将if (list[j].substr(0, 64)==result.substr(0, 64))语句中的64换成更小的数字，如4，进行运行验证代码正确性。
