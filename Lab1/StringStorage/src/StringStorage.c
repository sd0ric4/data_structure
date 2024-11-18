#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "StringStorage.h"

/**
 * 初始化字符串
 * @param S 字符串
 * @return void
 *
 * 算法实现:
 * 1. 将字符串指针 `ch` 置为 `NULL`。
 * 2. 将字符串长度 `length` 置为 `0`。
 */
void InitString(HString *S)
{
    S->ch = NULL;
    S->length = 0;
}

/**
 * 清空字符串
 * @param S 字符串
 * @return void
 *
 * 算法实现:
 * 1. 检查字符串指针 `ch` 是否为空。
 * 2. 若不为空，释放内存并将 `ch` 置为 `NULL`。
 * 3. 将字符串长度 `length` 置为 `0`。
 */
void ClearString(HString *S)
{
    if (S->ch)
    {
        free(S->ch);
        S->ch = NULL;
    }
    S->length = 0;
}

/**
 * 赋值操作：生成一个其值等于字符串常量 chars 的串 S
 * @param S 字符串
 * @param chars 字符串常量
 *
 * 算法实现:
 * 1. 获取 `chars` 的长度 `len`。
 * 2. 若 `len` 为 `0`，将 `S` 的 `ch` 置为 `NULL`，`length` 置为 `0`。
 * 3. 否则，分配 `len` 大小的内存，将 `chars` 复制到 `S` 的 `ch` 中，并设置 `length` 为 `len`。
 */
void StrAssign(HString *S, const char *chars)
{
    int len = strlen(chars);
    if (len == 0)
    {
        S->ch = NULL;
        S->length = 0;
    }
    else
    {
        S->ch = (char *)malloc(len * sizeof(char));
        memcpy(S->ch, chars, len);
        S->length = len;
    }
}

/**
 * 比较操作：若 S > T，则返回值 > 0；若 S = T，则返回值 = 0；若 S < T，则返回值 < 0
 * @param S 字符串 S
 * @param T 字符串 T
 * @return int
 *
 * 算法实现:
 * 1. 获取 `S` 和 `T` 的最小长度 `minLen`。
 * 2. 逐字符比较 `S` 和 `T`，若不相等则返回比较结果。
 * 3. 若相等，比较 `S` 和 `T` 的长度，返回比较结果。
 */
int StrCompare(HString S, HString T)
{
    int minLen = S.length < T.length ? S.length : T.length;
    for (int i = 0; i < minLen; i++)
    {
        if (S.ch[i] != T.ch[i])
        {
            return S.ch[i] - T.ch[i] > 0 ? 1 : -1;
        }
    }
    if (S.length == T.length)
        return 0;
    return S.length > T.length ? 1 : -1;
}

/**
 * 返回字符串长度
 * @param S 字符串
 * @return int
 *
 * 算法实现:
 * 1. 返回 `S` 的 `length`。
 */
int StrLength(HString S)
{
    return S.length;
}

/**
 * 串联接：用 S 返回由 T1 和 T2 联接而成的新串
 * @param S 新串
 * @param T1 字符串 T1
 * @param T2 字符串 T2
 *
 * 算法实现:
 * 1. 若 `S` 的 `ch` 不为空，释放内存。
 * 2. 分配 `T1` 和 `T2` 长度之和的内存。
 * 3. 将 `T1` 和 `T2` 的内容复制到 `S` 的 `ch` 中，并设置 `length`。
 */
void Concat(HString *S, HString T1, HString T2)
{
    if (S->ch)
        free(S->ch);
    S->length = T1.length + T2.length;
    S->ch = (char *)malloc(S->length * sizeof(char));
    memcpy(S->ch, T1.ch, T1.length);
    memcpy(S->ch + T1.length, T2.ch, T2.length);
}

/**
 * 返回串 S 的第 pos 个字符起长度为 len 的子串
 * @param Sub 子串
 * @param S 字符串 S
 * @param pos 起始位置
 * @param len 子串长度
 *
 * 算法实现:
 * 1. 检查 `pos` 和 `len` 的合法性。
 * 2. 若 `Sub` 的 `ch` 不为空，释放内存。
 * 3. 分配 `len` 大小的内存，将 `S` 的子串复制到 `Sub` 的 `ch` 中，并设置 `length`。
 */
void SubString(HString *Sub, HString S, int pos, int len)
{
    if (pos < 1 || pos > S.length || len < 0 || len > S.length - pos + 1)
    {
        printf("参数不合法\n");
        return;
    }
    if (Sub->ch)
        free(Sub->ch);
    Sub->length = len;
    Sub->ch = (char *)malloc(len * sizeof(char));
    memcpy(Sub->ch, S.ch + pos - 1, len);
}

/**
 * 遍历字符串
 * @param S 字符串
 * @return void
 *
 * 算法实现:
 * 1. 逐字符输出 `S` 的 `ch` 中的内容。
 */
void StrTraverse(HString S)
{
    for (int i = 0; i < S.length; i++)
    {
        putchar(S.ch[i]);
    }
    putchar('\n');
}

/**
 * 复制操作：由串 T 复制得串 S
 * @param S 字符串 S
 * @param T 字符串 T
 *
 * 算法实现:
 * 1. 若 `S` 的 `ch` 不为空，释放内存。
 * 2. 分配 `T` 长度的内存，将 `T` 的内容复制到 `S` 的 `ch` 中，并设置 `length`。
 */
void StrCopy(HString *S, HString T)
{
    if (S->ch)
        free(S->ch);
    S->length = T.length;
    S->ch = (char *)malloc(S->length * sizeof(char));
    memcpy(S->ch, T.ch, S->length);
}

/**
 * 返回子串 T 在主串 S 中第 pos 个字符之后的位置。若不存在，则函数值为 0
 * @param S 主串 S
 * @param T 子串 T
 * @param pos 起始位置
 * @return int
 *
 * 算法实现:
 * 1. 检查 `pos` 的合法性。
 * 2. 从 `pos` 开始逐字符比较 `S` 和 `T`，若匹配则返回位置。
 * 3. 若未匹配，返回 `0`。
 */
int Index(HString S, HString T, int pos)
{
    if (pos < 1 || pos > S.length)
        return 0;
    int i = pos - 1;
    while (i <= S.length - T.length)
    {
        int j = 0;
        while (j < T.length && S.ch[i + j] == T.ch[j])
            j++;
        if (j == T.length)
            return i + 1;
        i++;
    }
    return 0;
}

/**
 * 用 T2 替换主串 S 中出现的所有与 T1 相等的不重叠的子串
 * @param S 主串 S
 * @param T1 子串 T1
 * @param T2 子串 T2
 *
 * 算法实现:
 * 1. 初始化 `pos` 为 `1`。
 * 2. 循环查找 `T1` 在 `S` 中的位置 `idx`。
 * 3. 若找到，删除 `T1` 并插入 `T2`，更新 `pos`。
 * 4. 若未找到，退出循环。
 */
void Replace(HString *S, HString T1, HString T2)
{
    int pos = 1;
    int idx;
    while ((idx = Index(*S, T1, pos)) != 0)
    {
        StrDelete(S, idx, T1.length);
        StrInsert(S, idx, T2);
        pos = idx + T2.length;
    }
}

/**
 * 在串 S 的第 pos 个字符之前插入串 T
 * @param S 字符串 S
 * @param pos 插入位置
 * @param T 字符串 T
 * @return void
 *
 * 算法实现:
 * 1. 检查 `pos` 的合法性。
 * 2. 重新分配 `S` 的内存，使其能容纳插入后的内容。
 * 3. 移动 `S` 的内容，为 `T` 腾出空间。
 * 4. 将 `T` 的内容复制到 `S` 中，并更新 `length`。
 */
void StrInsert(HString *S, int pos, HString T)
{
    if (pos < 1 || pos > S->length + 1)
    {
        printf("参数不合法\n");
        return;
    }
    S->ch = (char *)realloc(S->ch, (S->length + T.length) * sizeof(char));
    memmove(S->ch + pos - 1 + T.length, S->ch + pos - 1, S->length - pos + 1);
    memcpy(S->ch + pos - 1, T.ch, T.length);
    S->length += T.length;
}

/**
 * 从串 S 中删除第 pos 个字符起长度为 len 的子串
 * @param S 字符串 S
 * @param pos 删除位置
 * @param len 删除长度
 * @return void
 *
 * 算法实现:
 * 1. 检查 `pos` 和 `len` 的合法性。
 * 2. 移动 `S` 的内容，覆盖被删除的部分。
 * 3. 重新分配 `S` 的内存，并更新 `length`。
 */
void StrDelete(HString *S, int pos, int len)
{
    if (pos < 1 || pos > S->length - len + 1 || len < 0)
    {
        printf("参数不合法\n");
        return;
    }
    memmove(S->ch + pos - 1, S->ch + pos - 1 + len, S->length - pos - len + 1);
    S->length -= len;
    S->ch = (char *)realloc(S->ch, S->length * sizeof(char));
}