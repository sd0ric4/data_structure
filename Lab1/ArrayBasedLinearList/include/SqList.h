#ifndef SQLIST_H
#define SQLIST_H

#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define MAXSIZE 100

#define LIST_INIT_SIZE 10
#define LISTINCREMENT 5

typedef int ElemType; // 定义ElemType类型
typedef int Status;   // 定义函数类型名

typedef struct SqList
{
    ElemType *elem; // 顺序表数据存储空间基址
    int length;     // 长度
} SqList;           // LIST为用户定义的线性表类型

Status InitList(SqList *L);
Status ClearList(SqList *L);
int ListLength(SqList L);
Status GetElem(SqList L, int i, ElemType *e);
int LocateElem(SqList L, ElemType e);
Status ListInsert(SqList *L, int i, ElemType e);
Status ListDelete(SqList *L, int i, ElemType *e);
void ListTraverse(SqList L);
void Union(SqList *La, SqList Lb);
void Intersection(SqList *La, SqList Lb);
void Difference(SqList *La, SqList Lb);
void MergeList(SqList La, SqList Lb, SqList *Lc);
void Purge(SqList *Lc);

#endif // SQLIST_H