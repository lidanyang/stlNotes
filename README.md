# stlNotes
record my learning notes of stl

## 2022/7/10

终于弄清楚了string中的allocator和rebind的机制。
顺便还搞清楚了默认的allocator用的是最基础的new和delete，
以及标准库配置默认allocator的机制（bits/c++allocator.h）。