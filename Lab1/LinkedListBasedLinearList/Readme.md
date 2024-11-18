# 实验 1-2 链表存储结构的操作与应用案例的实现

## 函数文档

[函数文档](./doc/函数文档.md)

## 实验要求

设线性表采用链表存储结构，数据元素为整数，利用 C 语言编制相应的程序（函数），实现以下操作：

### 链表存储结构和相关内容的定义

定义程序所需要的一些符号常量、链表元素类型和链表存储结构，以及所需的库函数。

```c
#include <stdio.h>
#include <stdlib.h>

#define OK         1
#define ERROR      0
#define TRUE       1
#define FALSE      0
#define INFEASIBLE -1
#define OVERFLOW   -2

typedef int ElemType;  // 声明 ElemType 的类型
typedef int Status;    // 声明函数类型名

typedef struct LNode {
    ElemType data;     // 数据域
    struct LNode *next; // 指针域
} LNode, *LinkList;    // 链表类型
```

### 链表基本操作函数的实现

根据定义的链表结构，编写下列基本操作函数的 C 源代码：

1. **InitList(LinkList \*L)** —— 利用 `malloc` 函数申请存储空间，构造一个空的链表 L。

2. **ClearList(LinkList \*L)** —— 将链表的数据清空。只需将链表头节点的 `next` 指针设为 `NULL`。

3. **int ListLength(LinkList L)** —— 返回 L 中数据元素个数。遍历链表，计数节点个数。

4. **GetElem(LinkList L, int i, ElemType \*e)** —— 用 `e` 返回 L 中第 `i` 个数据元素的值，`1 ≤ i ≤ ListLength(L)`。

5. **LNode\* LocateElem(LinkList L, ElemType e)** —— 返回 L 中第一个与 `e` 相等的数据元素的指针，若 `e` 不存在，则返回 `NULL`。

6. **ListInsert(LinkList \*L, int i, ElemType e)** —— 在 L 中的第 `i` 个位置之前插入新的数据元素 `e`，`1 ≤ i ≤ ListLength(L) + 1`。

7. **ListDelete(LinkList \*L, int i, ElemType e)** —— 删除 L 中的第 `i` 个数据元素，用 `e` 返回其值，`1 ≤ i ≤ ListLength(L)`。

8. **ListTraverse(LinkList L)** —— 利用 `printf` 函数依次输出 L 的每个数据元素的值。

注：以上函数除了表明函数类型外，其他的函数类型名均采用 `Status`（以下同）。

## 应用案例的实现

### 应用案例 1——链表元素的集合运算

将链表 $L_a$ 和 $L_b$ 看成 2 个集合，利用上述基本操作函数，分别编写出实现集合并、交、差运算的 3 个函数源码：

1. 并运算 $L_a = L_a \cup L_b$： `Union(LinkList *La, LinkList Lb)`

2. 交运算 $L_a = L_a \cap L_b$： `Intersection(LinkList *La, LinkList Lb)`

3. 差运算 $L_a = L_a - L_b$： `Difference(LinkList *La, LinkList Lb)`

### 应用案例 2——归并排序（两个有序表的合并）

设 $L_a$ 和 $L_b$ 中的元素是非递减有序的，编写一个归并排序，即 $L_c=L_a+L_b$ 的函数源码：

两个有序表的合并 $L_c=L_a+L_b$: `MergeList(LinkList La, LinkList Lb, LinkList *Lc)`

### 应用案例 3——去掉链表中的重复多余的元素

设 $L_c$ 是一个有重复元素的链表，编写一个去掉链表中重复多余元素的函数源码：

去重: `Purge(LinkList *Lc)`

## 编写一个主函数 main()，来检验上述应用函数是否正确，实现以下操作

### 数据准备。生成 2 个链表 $L_A$、$L_B$

其数据元素分别为：

- $L_A = 2 \rightarrow 8 \rightarrow 27 \rightarrow 39 \rightarrow 66 \rightarrow 77 \rightarrow 89 \rightarrow ∧$
- $L_B = 6 \rightarrow 18 \rightarrow 27 \rightarrow 59 \rightarrow 65 \rightarrow 77 \rightarrow 89 \rightarrow 120 \rightarrow 140 \rightarrow ∧$

链表生成后分别输出它们各元素的值。

### 将链表 $L_A$、$L_B$ 看成 2 个集合，分别调用集合并、交、差运算的 3 个函数，并在每个函数调用后输出 LA 的元素值，观察函数执行结果是否正确。

注意：由于函数调用后将导致 LA 的元素发生变化，因此在做下一个函数调用前需要恢复 LA 的元素。

### 因为 $L_A$ 和 $L_B$ 中的元素是非递减有序的，调用有序表合并的函数 `MergeList(LinkList La, LinkList Lb, LinkList *Lc)`，并在函数调用后输出 $L_C$ 的元素值。

### MergeList 函数合并后的有序表 $L_C$ 存在重复多余的元素，调用 `Purge(LinkList *Lc)`去掉重复多余的元素，并在函数调用后输出 $L_C$ 的元素值。
