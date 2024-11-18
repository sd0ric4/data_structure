#include "SqList.h"

/**
 * @brief 初始化线性表
 * @details 利用new函数申请存储空间，构造一个空的顺序表表L，L的初始大小为MAXSIZE。
 * @param L 待初始化的线性表
 */
Status InitList(SqList *L)
{
    L->elem = (ElemType *)malloc(MAXSIZE * sizeof(ElemType));
    if (!L->elem)
        return OVERFLOW;
    L->length = 0;
    return OK;
}

/**
 * @brief 清空线性表
 * @details 将顺序表的数据清空。只需将L.length清零，即L.length=0即可。
 * @param L 待清空的线性表
 */
Status ClearList(SqList *L)
{
    L->length = 0;
    return OK;
}

/**
 * @brief 获取线性表长度
 * @details 返回L中数据元素个数。直接返回L.length的值即可。
 * @param L 待获取长度的线性表
 * @return int 线性表的长度
 */
int ListLength(SqList L)
{
    return L.length;
}

/**
 * @brief 获取线性表中第i个元素
 * @details 用e返回L中第i个数据元素的值，1≤i≤L.Length。请注意函数参数i与数组下标之间的关系（以下同）。
 * @param L 待获取元素的线性表
 * @param i 待获取元素的位置
 * @param e 获取到的元素值
 * @return Status 获取成功返回OK，否则返回ERROR
 */
Status GetElem(SqList L, int i, ElemType *e)
{
    if (i < 1 || i > L.length)
        return ERROR;
    *e = L.elem[i - 1];
    return OK;
}

/**
 * @brief 查找元素
 * @details 返回L中第一个与e满足关系compare()的数据元素的位序。若这样的数据元素不存在，则返回值为0。
 * @param L 待查找元素的线性表
 * @param e 待查找的元素
 * @return int 查找到的元素位置
 */
int LocateElem(SqList L, ElemType e)
{
    for (int i = 0; i < L.length; i++)
    {
        if (L.elem[i] == e)
            return i + 1;
    }
    return ERROR;
}

/**
 * @brief 插入元素
 * @details 在L的第i个位置之前插入新的数据元素e，L的长度加1。
 * @param L 待插入元素的线性表
 * @param i 待插入元素的位置
 * @param e 待插入的元素
 * @return Status 插入成功返回OK，否则返回ERROR
 */
Status ListInsert(SqList *L, int i, ElemType e)
{
    if (i < 1 || i > L->length + 1)
        return ERROR;
    if (L->length == MAXSIZE)
        return ERROR;
    for (int j = L->length - 1; j >= i - 1; j--)
    {
        L->elem[j + 1] = L->elem[j];
    }
    L->elem[i - 1] = e;
    L->length++;
    return OK;
}

/**
 * @brief 删除元素
 * @details 删除L中的第i个数据元素，用e返回其值，1≤i≤L.length。
 * @param L 待删除元素的线性表
 * @param i 待删除元素的位置
 * @param e 删除的元素值
 * @return Status 删除成功返回OK，否则返回ERROR
 */
Status ListDelete(SqList *L, int i, ElemType *e)
{
    // 如果i的值不在合法范围（1≤i≤L.length），则返回ERROR
    if (i < 1 || i > L->length)
        return ERROR;
    // 将第i个元素的值赋给e
    *e = L->elem[i - 1];
    for (int j = i; j < L->length; j++)
    {
        L->elem[j - 1] = L->elem[j];
    }
    L->length--;
    return OK;
}

/**
 * @brief 遍历线性表
 * @details 利用printf函数依次输出L的每个数据元素的值。
 * @param L 待遍历的线性表
 * @return void
 */
void ListTraverse(SqList L)
{
    for (int i = 0; i < L.length; i++)
    {
        printf("%d ", L.elem[i]);
    }
    printf("\n");
}

/**
 * @brief 并运算
 * @details 设有两个线性表La和Lb，编写一个求La和Lb的并集的函数源码。
 * @param La 待并运算的线性表
 * @param Lb 待并运算的线性表
 * @return void
 */
void Union(SqList *La, SqList Lb)
{
    ElemType e;
    for (int i = 1; i <= Lb.length; i++)
    {
        GetElem(Lb, i, &e);
        if (!LocateElem(*La, e))
        {
            ListInsert(La, La->length + 1, e);
        }
    }
}

/**
 * @brief 交运算
 * @details 设有两个线性表La和Lb，编写一个求La和Lb的交集的函数源码。
 * @param La 待交运算的线性表
 * @param Lb 待交运算的线性表
 * @return void
 */
void Intersection(SqList *La, SqList Lb)
{
    ElemType e;
    for (int i = 1; i <= La->length; i++)
    {
        GetElem(*La, i, &e);
        if (!LocateElem(Lb, e))
        {
            ListDelete(La, i, &e);
            i--;
        }
    }
}

/**
 * @brief 差运算
 * @details 设有两个线性表La和Lb，编写一个求La和Lb的差集的函数源码。
 * @param La 待差运算的线性表
 * @param Lb 待差运算的线性表
 * @return void
 */
void Difference(SqList *La, SqList Lb)
{
    ElemType e;
    for (int i = 1; i <= Lb.length; i++)
    {
        GetElem(Lb, i, &e);
        int pos = LocateElem(*La, e);
        if (pos)
        {
            ListDelete(La, pos, &e);
        }
    }
}

/**
 * @brief 合并线性表
 * @details 设La和Lb中的元素是非递减有序的，编写一个归并排序，即Lc=La+Lb的函数源码
 * @param La 待合并的线性表
 * @param Lb 待合并的线性表
 * @param Lc 合并后的线性表
 * @return void
 */
void MergeList(SqList La, SqList Lb, SqList *Lc)
{
    // 初始化Lc
    InitList(Lc);
    // 初始化i、j、k
    int i = 1, j = 1, k = 1;
    ElemType ai, bj;
    while (i <= La.length && j <= Lb.length)
    {
        GetElem(La, i, &ai);
        GetElem(Lb, j, &bj);
        if (ai <= bj)
        {
            ListInsert(Lc, k++, ai);
            i++;
        }
        else
        {
            ListInsert(Lc, k++, bj);
            j++;
        }
    }
    while (i <= La.length)
    {
        GetElem(La, i++, &ai);
        ListInsert(Lc, k++, ai);
    }
    while (j <= Lb.length)
    {
        GetElem(Lb, j++, &bj);
        ListInsert(Lc, k++, bj);
    }
}

/**
 * @brief 去重
 * @details 设Lc是一个有序顺序表，编写一个函数，删除线性表中所有重复的元素，使表中所有元素的值均不同。
 * @param Lc 待去重的线性表
 * @return void
 */
void Purge(SqList *Lc)
{
    ElemType e;
    for (int i = 1; i < Lc->length; i++)
    {
        GetElem(*Lc, i, &e);
        for (int j = i + 1; j <= Lc->length; j++)
        {
            ElemType ej;
            GetElem(*Lc, j, &ej);
            if (e == ej)
            {
                ListDelete(Lc, j, &ej);
                j--;
            }
        }
    }
}