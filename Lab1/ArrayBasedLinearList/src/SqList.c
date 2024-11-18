#include "SqList.h"

/**
 * @brief 初始化线性表
 * @details 利用new函数申请存储空间，构造一个空的顺序表表L，L的初始大小为MAXSIZE。
 * @param L 待初始化的线性表
 * @return Status 初始化成功返回OK，否则返回OVERFLOW
 * @note 算法实现:
 * 1. 使用 malloc 分配存储空间。
 * 2. 检查分配是否成功，若不成功则返回 OVERFLOW。
 * 3. 设置L.length为 0。
 * 4. 返回 OK。
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
 * @return Status 清空操作成功返回OK
 * @note 算法实现:
 * 1. 将L.length设为 0。
 * 2. 返回 OK。
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
 * @note 算法实现:
 * 1. 直接返回L.length的值。
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
 * @note 算法实现:
 * 1. 检查i是否在有效范围 [1..L.length]。
 * 2. 若不在范围内，返回 ERROR。
 * 3. 通过i-1作为数组下标获取元素值并赋值给e。
 * 4. 返回 OK。
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
 * @note 算法实现:
 * 1. 遍历 L 的元素。
 * 2. 对每个元素，检查其值是否等于e。
 * 3. 若相等，则返回当前位序（i+1）。
 * 4. 若遍历结束未找到，返回 0。
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
 * @note 算法实现:
 * 1. 检查i是否在有效范围 [1..L.length+1]。
 * 2. 若不在范围内，或 L 已满，返回 ERROR。
 * 3. 从L.length-1至i-1位置的元素后移一位。
 * 4. 在i-1位置插入新元素e。
 * 5. 将L.length加 1。
 * 6. 返回 OK。
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
 * @note 算法实现:
 * 1. 检查i是否在有效范围 [1..L.length]。
 * 2. 若不在范围内，返回 ERROR。
 * 3. 获取i-1位置的元素值并赋值给e。
 * 4. 从i位置至L.length-1位置的元素前移一位。
 * 5. 将L.length减 1。
 * 6. 返回 OK。
 */
Status ListDelete(SqList *L, int i, ElemType *e)
{
    if (i < 1 || i > L->length)
        return ERROR;
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
 * @note 算法实现:
 * 1. 遍历 L 的元素，通过 printf 输出每个元素。
 * 2. 输出完成后换行。
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
 * @note 算法实现:
 * 1. 遍历 Lb 的元素。
 * 2. 对每一个元素 e，调用 LocateElem 检查是否在 La 中存在。
 * 3. 若不存在，调用 ListInsert 将 e 插入到 La 的末尾。
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
 * @note 算法实现:
 * 1. 遍历 La 的元素。
 * 2. 对每一个元素 e，调用 LocateElem 检查是否在 Lb 中存在。
 * 3. 若不存在，调用 ListDelete 删除该元素。
 * 4. 从i减 1 的位置继续遍历。
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
 * @note 算法实现:
 * 1. 遍历 Lb 的元素。
 * 2. 对每一个元素 e，调用 LocateElem 检查在 La 中的位置 pos。
 * 3. 若存在，调用 ListDelete 删除 La 中位置为 pos 的元素。
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
 * @note 算法实现:
 * 1. 初始化Lc。
 * 2. 设置初始索引 i, j, k 为 1。
 * 3. 在 La 和 Lb 非空时，逐一比较当前元素，较小者插入到Lc，并移动对应指针。
 * 4. 完成后，将剩余元素插入到Lc。
 */
void MergeList(SqList La, SqList Lb, SqList *Lc)
{
    InitList(Lc);
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
 * @note 算法实现:
 * 1. 遍历 Lc，获取当前元素e。
 * 2. 对于每个元素e，从当前位置 i+1 查找其余部分。
 * 3. 若发现与 e 相同的元素，调用 ListDelete 删除之，并调整遍历索引。
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