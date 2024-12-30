#include "sstable.h"

int main()
{
    int i, key;
    int n = 10;
    int c;
    SSTable ST1;
    int searchType;
    bool isSorted = false;

    // 创建静态查找表ST1
    CreateSSTable(ST1, n);
    printf("\n当前的静态查找表\n");
    TraversSSTable(ST1);

    while (1)
    {
        printf("\n请选择操作：\n");
        printf("1. 顺序查找\n");
        printf("2. 折半查找(非递归)\n");
        printf("3. 折半查找(递归)\n");
        printf("4. 排序表\n");
        printf("5. 显示表\n");
        printf("0. 退出\n");
        printf("请输入选择：");
        scanf("%d", &searchType);

        if (searchType == 0)
            break;

        switch (searchType)
        {
        case 1:
        { // 顺序查找
            printf("\n---顺序查找---\n");
            printf("请输入要查找的学号(-1退出)：");
            scanf("%d", &key);
            while (key != -1)
            {
                i = Search(ST1, key, c, SEQUENTIAL);
                if (i == 0)
                    printf("当前要查找的记录不存在！\n");
                else
                    OutElem(ST1, i);
                printf("查找时比较的次数=%d\n", c);
                printf("\n请输入要查找的学号(-1退出)：");
                scanf("%d", &key);
            }
            break;
        }
        case 2:
        { // 折半查找(非递归)
            if (!isSorted)
            {
                printf("执行折半查找前需要先对表进行排序！\n");
                break;
            }
            printf("\n---折半查找(非递归)---\n");
            printf("请输入要查找的学号(-1退出)：");
            scanf("%d", &key);
            while (key != -1)
            {
                i = Search(ST1, key, c, BINARY);
                if (i == -1)
                    printf("当前要查找的记录不存在！\n");
                else
                    OutElem(ST1, i);
                printf("查找时比较的次数=%d\n", c);
                printf("\n请输入要查找的学号(-1退出)：");
                scanf("%d", &key);
            }
            break;
        }
        case 3:
        { // 折半查找(递归)
            if (!isSorted)
            {
                printf("执行折半查找前需要先对表进行排序！\n");
                break;
            }
            printf("\n---折半查找(递归)---\n");
            printf("请输入要查找的学号(-1退出)：");
            scanf("%d", &key);
            while (key != -1)
            {
                i = Search(ST1, key, c, BINARY_RECURSIVE);
                if (i == -1)
                    printf("当前要查找的记录不存在！\n");
                else
                    OutElem(ST1, i);
                printf("查找时比较的次数=%d\n", c);
                printf("\n请输入要查找的学号(-1退出)：");
                scanf("%d", &key);
            }
            break;
        }
        case 4:
        { // 排序
            SortSSTable(ST1);
            isSorted = true;
            printf("\n排序后的表：\n");
            TraversSSTable(ST1);
            break;
        }
        case 5:
        { // 显示表
            printf("\n当前的表：\n");
            TraversSSTable(ST1);
            break;
        }
        default:
            printf("无效的选择！\n");
            break;
        }
    }

    // 释放内存
    free(ST1.Record);
    return 0;
}