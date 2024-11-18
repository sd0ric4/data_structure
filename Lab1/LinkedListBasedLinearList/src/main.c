#include <stdio.h>
#include "LinkList.h"

int main()
{
    LinkList LA, LB, LC;
    InitList(&LA);
    InitList(&LB);
    InitList(&LC);

    int la_data[] = {2, 8, 27, 39, 66, 77, 89};
    int lb_data[] = {6, 18, 27, 59, 65, 77, 89, 120, 140};

    for (int i = 0; i < sizeof(la_data) / sizeof(la_data[0]); i++)
    {
        ListInsert(&LA, i + 1, la_data[i]);
    }

    for (int i = 0; i < sizeof(lb_data) / sizeof(lb_data[0]); i++)
    {
        ListInsert(&LB, i + 1, lb_data[i]);
    }

    printf("LA: ");
    ListTraverse(LA);
    printf("LB: ");
    ListTraverse(LB);

    Union(&LA, LB);
    printf("Union LA: ");
    ListTraverse(LA);

    ClearList(&LA);
    for (int i = 0; i < sizeof(la_data) / sizeof(la_data[0]); i++)
    {
        ListInsert(&LA, i + 1, la_data[i]);
    }

    Intersection(&LA, LB);
    printf("Intersection LA: ");
    ListTraverse(LA);

    ClearList(&LA);
    for (int i = 0; i < sizeof(la_data) / sizeof(la_data[0]); i++)
    {
        ListInsert(&LA, i + 1, la_data[i]);
    }

    Difference(&LA, LB);
    printf("Difference LA: ");
    ListTraverse(LA);

    MergeList(LA, LB, &LC);
    printf("Merged LC: ");
    ListTraverse(LC);

    Purge(&LC);
    printf("Purged LC: ");
    ListTraverse(LC);

    return 0;
}