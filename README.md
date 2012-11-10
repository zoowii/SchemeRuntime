SchemeRuntime
=============

Simple Scheme Interpreter written in C++

开发环境：Win7 64bit + Visual Studio 2012
因为使用了#pragma once，所以g++直接编译会失败。Makefile因为暂时没有用到。所以如果要用g++或clang++，请自行把#pragma once改成#ifndef ...的形式，并重写Makefile。

是我另一个项目SchemeScript(运行在浏览器或node.js中的Scheme解释器)的C++版，不过因为对C++不熟，而且刚写，所以支持特性比较少。另外，性能似乎SchemeScript还低。纠结...不过内存泄漏应该很少或没有。以后可能会为无副作用的函数加上缓存。

支持变量定义，函数定义和调用，递归，柯里化，lambda表达式等

因为是第一版，所以只支持不超过int范围的整数，字符串，布尔值的字面量。

支持特性列表：
1. Int范围整数
2. 字符串
3. 布尔值
4. if, cond等流程控制函数
5. 函数定义，调用，递归等
6. lambda表达式，柯里化
7. display, define, +, -, *, <, >, <=, >=等函数