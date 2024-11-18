#include <stdio.h>
#include "LinkList.h"

void PrintList(LinkList L)
{
    LNode *p = L->next;
    while (p)
    {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

int main()
{
    LinkList LA, LB, LC;

    InitList(&LA);
    InitList(&LB);

    int dataA[] = {2, 8, 27, 39, 66, 77, 89};
    int dataB[] = {6, 18, 27, 59, 65, 77, 89, 120, 140};

    for (int i = 0; i < sizeof(dataA) / sizeof(dataA[0]); i++)
    {
        ListInsert(&LA, i + 1, dataA[i]);
    }

    for (int i = 0; i < sizeof(dataB) / sizeof(dataB[0]); i++)
    {
        ListInsert(&LB, i + 1, dataB[i]);
    }

    printf("LA: ");
    PrintList(LA);
    printf("LB: ");
    PrintList(LB);

    // 并运算
    Union(&LA, LB);
    printf("Union LA: ");
    PrintList(LA);

    // 恢复LA
    ClearList(LA);
    for (int i = 0; i < sizeof(dataA) / sizeof(dataA[0]); i++)
    {
        ListInsert(&LA, i + 1, dataA[i]);
    }

    // 交运算
    Intersection(&LA, LB);
    printf("Intersection LA: ");
    PrintList(LA);

    // 恢复LA
    ClearList(LA);
    for (int i = 0; i < sizeof(dataA) / sizeof(dataA[0]); i++)
    {
        ListInsert(&LA, i + 1, dataA[i]);
    }

    // 差运算
    Difference(&LA, LB);
    printf("Difference LA: ");
    PrintList(LA);

    // 恢复LA
    ClearList(LA);
    for (int i = 0; i < sizeof(dataA) / sizeof(dataA[0]); i++)
    {
        ListInsert(&LA, i + 1, dataA[i]);
    }

    // 归并排序
    MergeList(LA, LB, &LC);
    printf("Merged LC: ");
    PrintList(LC);

    // 去重
    Purge(&LC);
    printf("Purged LC: ");
    PrintList(LC);

    // 清理内存
    ClearList(LA);
    ClearList(LB);
    ClearList(LC);

    return 0;
}