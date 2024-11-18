# 实验 1-1 线性表顺序存储结构的操作与应用案例的实现

## 函数文档

[函数文档](./doc/函数文档.md)

## 实验要求

设线性表采用顺序存储结构，数据元素为整数，利用 C/C++语言编制相应的程序（函数），实现以下操作：

### 顺序表存储结构和相关内容的定义

定义程序所需要的一些符号常量、线性表元素类型和顺序表存储结构，以及所需的库函数。

```c
#include <stdio.h>

#include <stdlib.h>

#define OK         1

#define ERROR   0

#define TRUE    1

#define FALSE    0

#define INFEASIBLE -1

#define OVERFLOW -2

#define MAXSIZE    100



typedef int ElemType;  //声明ElemType的类型


typedef int Status;  //声明函数类型名



typedef struct SqList {

        ElemType *elem;  //顺序表数据存储空间基址

        int length;

}SqList;// LIST为用户定义的线性表类型

```

### 顺序表基本操作函数的实现

根据定义的顺序表结构，编写下列基本操作函数的 C/C++ 源代码：

1. **InitList(SqList &L)** —— 利用 `new` 函数申请存储空间，构造一个空的顺序表 L，L 的初始大小为 `MAXSIZE`。

2. **ClearList(SqList &L)** —— 将顺序表的数据清空。只需将 `L.length` 清零，即 `L.length = 0` 即可。

3. **int Listlength(SqList L)** —— 返回 L 中数据元素个数。直接返回 `L.length` 的值即可。

4. **GetElem(SqList L, int i, ElemType &e)** —— 用 `e` 返回 L 中第 `i` 个数据元素的值，`1 ≤ i ≤ L.Length`。请注意函数参数 `i` 与数组下标之间的关系（以下同）。

5. **int LocateList(SqList L, ElemType e)** —— 返回 L 中第 1 个与 `e` 相等的数据元素位序，若 `e` 不存在，则返回 `0`。

6. **ListInsert(SqList &L, int i, ElemType e)** —— 在 L 中的第 `i` 个位置之前插入新的数据元素 `e`，`1 ≤ i ≤ L.length + 1`。

7. **ListDelete(SqList &L, int i, ElemType &e)** —— 删除 L 中的第 `i` 个数据元素，用 `e` 返回其值，`1 ≤ i ≤ L.length`。

8. **ListTraverse(SqList L)** —— 利用 `printf` 函数依次输出 L 的每个数据元素的值。

注：以上函数除了表明函数类型外，其他的函数类型名均采用 `Status`（以下同）。

## 应用案例的实现

### 应用案例 1——线性表元素的集合运算

将顺序表 $L_a$ 和 $L_b$ 看成 2 个集合，利用上述基本操作函数，分别编写出实现集合并、交、差运算的 3 个函数源码：

1. 并运算 $L_a = L_a \cup L_b$： `Union(SqList &La, SqList Lb)`

2. 交运算 $L_a = L_a \cap L_b$： `Intersection(SqList &La, SqList Lb)`

3. 差运算 $L_a = L_a - L_b$： `Difference(SqList &La, SqList Lb)`

### 应用案例 2——归并排序（两个有序表的合并）

设 $L_a$ 和 $L_b$ 中的元素是非递减有序的，编写一个归并排序，即 $L_c=L_a+L_b$ 的函数源码：

两个有序表的合并 $L_c=L_a+L_b$: `MergeList(SqList La, SqList Lb, SqList &Lc)`

### 应用案例 3——去掉线性表中的重复多余的元素

设 $L_c$ 是一个有重复元素的顺序表，编写一个去掉顺序表中重复多余元素的函数源码：

去重: `Purge(SqList &Lc)`

## 编写一个主函数 main()，来检验上述应用函数是否正确，实现以下操作

### 数据准备。生成 2 个顺序表 $L_A$、$L_B$

其数据元素分别为：

- $L_A = \{2,8,27,39,66,77,89\}$
- $L_B = \{6,18,27,59,65,77,89,120,140\}$

顺序表生成后分别输出它们各元素的值。

### 将顺序表 $L_A$、$L_B$ 看成 2 个集合，分别调用集合并、交、差运算的 3 个函数，并在每个函数调用后输出 LA 的元素值，观察函数执行结果是否正确。

注意：由于函数调用后将导致 LA 的元素发生变化，因此在做下一个函数调用前需要恢复 LA 的元素。

### 因为 $L_A$ 和 $L_B$ 中的元素是非递减有序的，调用有序表合并的函数 `MergeList(SqList La, SqList Lb, SqList &Lc)`，并在函数调用后输出 $L_C$ 的元素值。

### MergeList 函数合并后的有序表 $L_C$ 存在重复多余的元素，调用 `Purge(SqList &Lc)`去掉重复多余的元素，并在函数调用后输出 $L_C$ 的元素值。
