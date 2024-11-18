#include <stdio.h>
#include <stdlib.h>
#include "LinkList.h"

/**
 * @brief 初始化链表
 * 创建一个空链表
 * @param L 链表
 * @return Status 状态码
 *
 * 算法实现:
 * 1. 分配一个头结点的内存空间。
 * 2. 将头结点的 `next` 指针域置为 `NULL`，表示链表为空。
 */
Status InitList(LinkList *L)
{
    *L = (LNode *)malloc(sizeof(LNode));
    if (*L == NULL)
        return ERROR;
    (*L)->next = NULL;
    return OK;
}

/**
 * @brief 清空链表
 * 将链表置为空表
 * @param L 链表
 * @return Status 状态码
 *
 * 算法实现:
 * 1. 遍历链表，释放每个节点的内存空间。
 * 2. 将头结点的 `next` 指针域置为 `NULL`。
 */
Status ClearList(LinkList L)
{
    LNode *p, *q;
    p = L->next;
    while (p)
    {
        q = p->next;
        free(p);
        p = q;
    }
    L->next = NULL;
    return OK;
}

/**
 * @brief 获取链表长度
 * 返回链表的长度
 * @param L 链表
 * @return int 链表长度
 *
 * 算法实现:
 * 1. 遍历链表，计数节点个数。
 */
int ListLength(LinkList L)
{
    int length = 0;
    LNode *p = L->next;
    while (p)
    {
        length++;
        p = p->next;
    }
    return length;
}

/**
 * @brief 获取链表元素
 * 获取链表中第i个元素
 * @param L 链表
 * @param i 位置
 * @param e 元素
 * @return Status 状态码
 *
 * 算法实现:
 * 1. 遍历链表，找到第 `i` 个节点。
 * 2. 返回该节点的数据域的值。
 */
Status GetElem(LinkList L, int i, ElemType *e)
{
    int j = 1;
    LNode *p = L->next;
    while (p && j < i)
    {
        p = p->next;
        j++;
    }
    if (!p || j > i)
        return ERROR;
    *e = p->data;
    return OK;
}

/**
 * @brief 查找元素
 * 在链表中查找元素e
 * @param L 链表
 * @param e 元素
 * @return LNode* 元素位置
 *
 * 算法实现:
 * 1. 遍历链表，找到第一个与 `e` 相等的节点。
 */
LNode *LocateElem(LinkList L, ElemType e)
{
    LNode *p = L->next;
    while (p && p->data != e)
    {
        p = p->next;
    }
    return p;
}

/**
 * @brief 插入元素
 * 在链表的第i个位置插入元素e
 * @param L 链表
 * @param i 位置
 * @param e 元素
 * @return Status 状态码
 *
 * 算法实现:
 * 1. 遍历链表，找到第 `i-1` 个节点。
 * 2. 分配一个新节点，将数据域置为 `e`。
 * 3. 将新节点插入链表。
 */
Status ListInsert(LinkList *L, int i, ElemType e)
{
    int j = 0;
    LNode *p = *L, *s;
    while (p && j < i - 1)
    {
        p = p->next;
        j++;
    }
    if (!p || j > i - 1)
        return ERROR;
    s = (LNode *)malloc(sizeof(LNode));
    if (!s)
        return ERROR;
    s->data = e;
    s->next = p->next;
    p->next = s;
    return OK;
}

/**
 * @brief 删除元素
 * 删除链表的第i个元素
 * @param L 链表
 * @param i 位置
 * @param e 元素
 * @return Status 状态码
 *
 * 算法实现:
 * 1. 遍历链表，找到第 `i-1` 个节点。
 * 2. 删除第 `i` 个节点，并返回其数据域的值。
 */
Status ListDelete(LinkList *L, int i, ElemType *e)
{
    int j = 0;
    LNode *p = *L, *q;
    while (p->next && j < i - 1)
    {
        p = p->next;
        j++;
    }
    if (!(p->next) || j > i - 1)
        return ERROR;
    q = p->next;
    p->next = q->next;
    *e = q->data;
    free(q);
    return OK;
}

/**
 * @brief 遍历链表
 * 遍历链表并输出元素
 * @param L 链表
 *
 * 算法实现:
 * 1. 遍历链表，打印每个节点的数据域。
 */
void ListTraverse(LinkList L)
{
    LNode *p = L->next;
    while (p)
    {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

/**
 * @brief 并运算
 * 将链表La和Lb合并为新链表La
 * @param La 链表La
 * @param Lb 链表Lb
 * @return Status 状态码
 *
 * 算法实现:
 * 1. 遍历第二个链表，将不在第一个链表中的元素插入第一个链表。
 */
Status Union(LinkList *La, LinkList Lb)
{
    LNode *pa = (*La)->next;
    LNode *pb = Lb->next;
    while (pb)
    {
        if (!LocateElem(*La, pb->data))
        {
            ListInsert(La, ListLength(*La) + 1, pb->data);
        }
        pb = pb->next;
    }
    return OK;
}

/**
 * @brief 交运算
 * 将链表La和Lb中相同的元素保留在La中
 * @param La 链表La
 * @param Lb 链表Lb
 * @return Status 状态码
 *
 * 算法实现:
 * 1. 遍历第一个链表，删除不在第二个链表中的元素。
 */
Status Intersection(LinkList *La, LinkList Lb)
{
    LNode *pa = (*La)->next;
    LNode *pre = *La;
    while (pa)
    {
        if (!LocateElem(Lb, pa->data))
        {
            pre->next = pa->next;
            free(pa);
            pa = pre->next;
        }
        else
        {
            pre = pa;
            pa = pa->next;
        }
    }
    return OK;
}

/**
 * @brief 差运算
 * 将链表La中与Lb中相同的元素删除
 * @param La 链表La
 * @param Lb 链表Lb
 * @return Status 状态码
 *
 * 算法实现:
 * 1. 遍历第一个链表，删除在第二个链表中的元素。
 */
Status Difference(LinkList *La, LinkList Lb)
{
    LNode *pa = (*La)->next;
    LNode *pre = *La;
    while (pa)
    {
        if (LocateElem(Lb, pa->data))
        {
            pre->next = pa->next;
            free(pa);
            pa = pre->next;
        }
        else
        {
            pre = pa;
            pa = pa->next;
        }
    }
    return OK;
}

/**
 * @brief 合并链表
 * 将链表La和Lb合并为新链表Lc
 * @param La 链表La
 * @param Lb 链表Lb
 * @param Lc 新链表Lc
 * @return Status 状态码
 *
 * 算法实现:
 * 1. 初始化一个新的链表。
 * 2. 依次比较两个链表的元素，将较小的元素插入新链表。
 * 3. 将剩余的元素插入新链表。
 */
Status MergeList(LinkList La, LinkList Lb, LinkList *Lc)
{
    InitList(Lc);
    LNode *pa = La->next;
    LNode *pb = Lb->next;
    LNode *pc = *Lc;
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
    return OK;
}

/**
 * @brief 清除重复元素
 * 清除链表中的重复元素
 * @param Lc 链表
 * @return Status 状态码
 *
 * 算法实现:
 * 1. 遍历链表，对于每个元素，删除其后续节点中与其相等的元素。
 */
Status Purge(LinkList *Lc)
{
    LNode *p = (*Lc)->next;
    LNode *q, *pre;
    while (p && p->next)
    {
        pre = p;
        q = p->next;
        while (q)
        {
            if (q->data == p->data)
            {
                pre->next = q->next;
                free(q);
                q = pre->next;
            }
            else
            {
                pre = q;
                q = q->next;
            }
        }
        p = p->next;
    }
    return OK;
}