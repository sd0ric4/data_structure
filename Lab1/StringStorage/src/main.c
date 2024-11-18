#include <stdio.h>
#include "StringStorage.h"

int main()
{
    HString T1, T2, T3, S;
    InitString(&T1);
    InitString(&T2);
    InitString(&T3);
    InitString(&S);

    // （1）数据准备
    StrAssign(&T1, "Hello");
    StrAssign(&T2, "World");
    StrAssign(&T3, "C Programming");
    printf("T1: ");
    StrTraverse(T1);
    printf("T2: ");
    StrTraverse(T2);
    printf("T3: ");
    StrTraverse(T3);

    // （2）将串 T1 赋给串 S
    StrCopy(&S, T1);
    printf("S: ");
    StrTraverse(S);

    // （3）将串 T2 插入到 S 的尾部
    StrInsert(&S, StrLength(S) + 1, T2);
    printf("After inserting T2 at the end of S: ");
    StrTraverse(S);

    // （4）将 S 中的 T1 替换成 T3
    Replace(&S, T1, T3);
    printf("After replacing T1 with T3 in S: ");
    StrTraverse(S);

    // （5）查找串 T2 在 S 中的位置
    int pos = Index(S, T2, 1);
    printf("Position of T2 in S: %d\n", pos);

    // （6）删除 S 中子串 T2
    StrDelete(&S, pos, StrLength(T2));
    printf("After deleting T2 from S: ");
    StrTraverse(S);

    // 清理内存
    ClearString(&T1);
    ClearString(&T2);
    ClearString(&T3);
    ClearString(&S);

    return 0;
}