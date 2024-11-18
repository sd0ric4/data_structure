#include <stdio.h>
#include <stdlib.h>
#include "LinkList.h"

int InitList(LinkList *L)
{
    *L = (LinkList)malloc(sizeof(LNode));
    if (*L == NULL)
        return 0;
    (*L)->next = NULL;
    return 1;
}

int ClearList(LinkList *L)
{
    LinkList p, q;
    p = (*L)->next;
    while (p)
    {
        q = p->next;
        free(p);
        p = q;
    }
    (*L)->next = NULL;
    return 1;
}

int ListLength(LinkList L)
{
    int length = 0;
    LinkList p = L->next;
    while (p)
    {
        length++;
        p = p->next;
    }
    return length;
}

int GetElem(LinkList L, int i, ElemType *e)
{
    int j = 1;
    LinkList p = L->next;
    while (p && j < i)
    {
        p = p->next;
        j++;
    }
    if (!p || j > i)
        return 0;
    *e = p->data;
    return 1;
}

LNode *LocateElem(LinkList L, ElemType e)
{
    LinkList p = L->next;
    while (p && p->data != e)
    {
        p = p->next;
    }
    return p;
}

int ListInsert(LinkList *L, int i, ElemType e)
{
    int j = 0;
    LinkList p = *L, s;
    while (p && j < i - 1)
    {
        p = p->next;
        j++;
    }
    if (!p || j > i - 1)
        return 0;
    s = (LinkList)malloc(sizeof(LNode));
    s->data = e;
    s->next = p->next;
    p->next = s;
    return 1;
}

int ListDelete(LinkList *L, int i, ElemType *e)
{
    int j = 0;
    LinkList p = *L, q;
    while (p->next && j < i - 1)
    {
        p = p->next;
        j++;
    }
    if (!(p->next) || j > i - 1)
        return 0;
    q = p->next;
    p->next = q->next;
    *e = q->data;
    free(q);
    return 1;
}

void ListTraverse(LinkList L)
{
    LinkList p = L->next;
    while (p)
    {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

void Union(LinkList *La, LinkList Lb)
{
    LinkList pb = Lb->next;
    while (pb)
    {
        if (!LocateElem(*La, pb->data))
        {
            ListInsert(La, ListLength(*La) + 1, pb->data);
        }
        pb = pb->next;
    }
}

void Intersection(LinkList *La, LinkList Lb)
{
    LinkList pa = (*La)->next;
    LinkList pb;
    ElemType e;
    while (pa)
    {
        pb = Lb->next;
        while (pb && pb->data != pa->data)
        {
            pb = pb->next;
        }
        if (!pb)
        {
            ListDelete(La, 1, &e);
        }
        else
        {
            pa = pa->next;
        }
    }
}

void Difference(LinkList *La, LinkList Lb)
{
    LinkList pa = (*La)->next;
    LinkList pb;
    ElemType e;
    while (pa)
    {
        pb = Lb->next;
        while (pb && pb->data != pa->data)
        {
            pb = pb->next;
        }
        if (pb)
        {
            ListDelete(La, 1, &e);
        }
        else
        {
            pa = pa->next;
        }
    }
}

void MergeList(LinkList La, LinkList Lb, LinkList *Lc)
{
    LinkList pa = La->next;
    LinkList pb = Lb->next;
    LinkList pc;
    *Lc = (LinkList)malloc(sizeof(LNode));
    pc = *Lc;
    pc->next = NULL;
    while (pa && pb)
    {
        if (pa->data <= pb->data)
        {
            ListInsert(Lc, ListLength(*Lc) + 1, pa->data);
            pa = pa->next;
        }
        else
        {
            ListInsert(Lc, ListLength(*Lc) + 1, pb->data);
            pb = pb->next;
        }
    }
    while (pa)
    {
        ListInsert(Lc, ListLength(*Lc) + 1, pa->data);
        pa = pa->next;
    }
    while (pb)
    {
        ListInsert(Lc, ListLength(*Lc) + 1, pb->data);
        pb = pb->next;
    }
}

void Purge(LinkList *Lc)
{
    LinkList p = (*Lc)->next;
    LinkList q, r;
    while (p && p->next)
    {
        q = p;
        while (q->next)
        {
            if (q->next->data == p->data)
            {
                r = q->next;
                q->next = r->next;
                free(r);
            }
            else
            {
                q = q->next;
            }
        }
        p = p->next;
    }
}