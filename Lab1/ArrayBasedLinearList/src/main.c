#include "SqList.h"

int main()
{
    SqList LA, LB, LC;
    InitList(&LA);
    InitList(&LB);

    // 数据准备
    int dataA[] = {2, 8, 27, 39, 66, 77, 89};
    int dataB[] = {6, 18, 27, 59, 65, 77, 89, 120, 140};
    for (int i = 0; i < 7; i++)
        ListInsert(&LA, i + 1, dataA[i]);
    for (int i = 0; i < 9; i++)
        ListInsert(&LB, i + 1, dataB[i]);

    printf("LA: ");
    ListTraverse(LA);
    printf("LB: ");
    ListTraverse(LB);

    // 并运算
    Union(&LA, LB);
    printf("Union LA: ");
    ListTraverse(LA);

    // 恢复LA
    ClearList(&LA);
    for (int i = 0; i < 7; i++)
        ListInsert(&LA, i + 1, dataA[i]);

    // 交运算
    Intersection(&LA, LB);
    printf("Intersection LA: ");
    ListTraverse(LA);

    // 恢复LA
    ClearList(&LA);
    for (int i = 0; i < 7; i++)
        ListInsert(&LA, i + 1, dataA[i]);

    // 差运算
    Difference(&LA, LB);
    printf("Difference LA: ");
    ListTraverse(LA);

    // 恢复LA
    ClearList(&LA);
    for (int i = 0; i < 7; i++)
        ListInsert(&LA, i + 1, dataA[i]);

    // 归并排序
    MergeList(LA, LB, &LC);
    printf("Merged LC: ");
    ListTraverse(LC);

    // 去重
    Purge(&LC);
    printf("Purged LC: ");
    ListTraverse(LC);

    return 0;
}