#ifndef LINKLIST_H
#define LINKLIST_H

typedef int ElemType;

typedef struct LNode
{
    ElemType data;
    struct LNode *next;
} LNode, *LinkList;

int InitList(LinkList *L);
int ClearList(LinkList *L);
int ListLength(LinkList L);
int GetElem(LinkList L, int i, ElemType *e);
LNode *LocateElem(LinkList L, ElemType e);
int ListInsert(LinkList *L, int i, ElemType e);
int ListDelete(LinkList *L, int i, ElemType *e);
void ListTraverse(LinkList L);

void Union(LinkList *La, LinkList Lb);
void Intersection(LinkList *La, LinkList Lb);
void Difference(LinkList *La, LinkList Lb);
void MergeList(LinkList La, LinkList Lb, LinkList *Lc);
void Purge(LinkList *Lc);

#endif