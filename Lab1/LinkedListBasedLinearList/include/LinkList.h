#ifndef LIST_H
#define LIST_H

typedef int ElemType;

typedef struct LNode
{
    ElemType data;
    struct LNode *next;
} LNode, *LinkList;

typedef enum
{
    OK,
    ERROR
} Status;

Status InitList(LinkList *L);
Status ClearList(LinkList L);
int ListLength(LinkList L);
Status GetElem(LinkList L, int i, ElemType *e);
LNode *LocateElem(LinkList L, ElemType e);
Status ListInsert(LinkList *L, int i, ElemType e);
Status ListDelete(LinkList *L, int i, ElemType *e);
void ListTraverse(LinkList L);

Status Union(LinkList *La, LinkList Lb);
Status Intersection(LinkList *La, LinkList Lb);
Status Difference(LinkList *La, LinkList Lb);
Status MergeList(LinkList La, LinkList Lb, LinkList *Lc);
Status Purge(LinkList *Lc);

#endif