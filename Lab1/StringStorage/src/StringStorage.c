#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "StringStorage.h"

/**
 * 初始化字符串
 * @param S 字符串
 * @return void
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