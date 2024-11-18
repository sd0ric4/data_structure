# 实验 1-4 串操作与应用案例的实现

## 函数文档

[函数文档](./doc/函数文档.md)

## 实验要求

设“串”采用堆分配存储结构，利用 C 语言编制相应的程序（函数），实现以下操作：

### 串的堆分配存储结构定义

定义程序所需要的一些符号常量、串元素类型和存储结构，以及所需的库函数。

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *ch;   // 若是非空串，则按串长分配存储区，否则 ch 为 NULL
    int length; // 串长度
} HString; // 堆分配串的类型定义
```

### 串基本操作函数的实现

根据定义的串结构，编写下列基本操作函数的 C 源代码：

1. **InitString(HString \*S)** —— 初始化字符串，构造一个空串。

2. **ClearString(HString \*S)** —— 清空字符串，释放内存。

3. **StrAssign(HString \*S, const char \*chars)** —— 将字符串常量 `chars` 赋给串变量 `S`。

4. **StrCompare(HString S, HString T)** —— 比较串 `S` 与 `T` 的值，若 `S > T`，则返回 `1`；若 `S = T`，则返回 `0`；否则返回 `-1`。

5. **StrLength(HString S)** —— 返回串 `S` 的长度。

6. **Concat(HString \*S, HString T1, HString T2)** —— 将串 `T1` 和 `T2` 连接在一起赋给串变量 `S`。

7. **SubString(HString \*Sub, HString S, int pos, int len)** —— 用 `Sub` 返回串 `S` 的第 `pos` 个字符起长度为 `len` 的子串。

8. **StrTraverse(HString S)** —— 遍历（输出）字符串 `S` 中的各个字符。

9. **StrCopy(HString \*S, HString T)** —— 将串 `T` 赋给串 `S`。

### 串其他操作函数的实现

调用上述串的基本操作函数，编制下列串的操作函数：

1. **Index(HString S, HString T, int pos)** —— 返回串 `T` 在串 `S` 中第 `pos` 个字符开始出现的位置，若串 `T` 不是串 `S` 的子串，则返回 `0`。

2. **Replace(HString \*S, HString T1, HString T2)** —— 将 `S` 串中所有的子串 `T1` 用串 `T2` 代替。

3. **StrInsert(HString \*S, int pos, HString T)** —— 在串 `S` 的第 `pos` 个字符之前插入串 `T`。

4. **StrDelete(HString \*S, int pos, int len)** —— 删除串 `S` 中从第 `pos` 个字符开始连续的 `len` 个字符。

## 编写一个主函数 main()，来检验上述操作函数是否正确，实现以下操作

### 数据准备

利用 `StrAssign()` 将设定的字符串常量（自行设定）赋给 3 个字符串变量 `T1`, `T2`, `T3`，然后利用 `StrTraverse()` 输出字符串字符。

### 字符串操作

1. 利用 `StrCopy()` 将串 `T1` 赋给串 `S`，并输出 `S` 的字符。

2. 利用 `StrInsert()` 将串 `T2` 插入到 `S` 的尾部，并输出 `S` 的字符。

3. 利用 `Replace()` 将串 `S` 中的 `T1` 替换成 `T3`，并输出 `S` 的字符。

4. 利用 `Index()` 查找串 `T2` 在 `S` 中的位置，并输出位置结果。

5. 利用 `StrDelete()` 删除 `S` 中子串 `T2`，并输出删除后 `S` 的结果。
