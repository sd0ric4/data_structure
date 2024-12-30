#include "sstable.h"

int count = 0; // 用于递归版本的折半查找中计数

// 统一的查找接口
int Search(SSTable ST, KeyType key, int &c, SearchType type)
{
    c = 0;      // 重置计数器
    int result; // 提前声明结果变量

    switch (type)
    {
    case SEQUENTIAL:
        result = SearchSSTable_Seq(ST, key, c);
        break;
    case BINARY:
        result = SearchSSTable_Bin(ST, key, c);
        break;
    case BINARY_RECURSIVE:
        count = 0;
        result = SearchSSTable_Bin1(ST, key, 1, ST.length);
        c = count;
        break;
    default:
        result = -1;
        break;
    }
    return result;
}

void CreateSSTable(SSTable &ST, int n)
{
    ST.length = n;
    // 分配n+1个空间，0号位置作为哨兵
    ST.Record = (RecordType *)malloc(sizeof(RecordType) * (n + 1));

    // 初始化数据，从下标1开始
    ST.Record[1] = {56, "Zhang", "F", 19};
    ST.Record[2] = {19, "Wang", "F", 20};
    ST.Record[3] = {80, "Zhou", "F", 19};
    ST.Record[4] = {5, "Huang", "M", 20};
    ST.Record[5] = {21, "Zheng", "M", 20};
    ST.Record[6] = {64, "Li", "M", 19};
    ST.Record[7] = {88, "Liu", "F", 18};
    ST.Record[8] = {13, "Qian", "F", 19};
    ST.Record[9] = {37, "Sun", "M", 20};
    ST.Record[10] = {75, "Zhao", "M", 20};
}

void TraversSSTable(SSTable ST)
{
    printf("\n学号\t姓名\t性别\t年龄\n");
    for (int i = 1; i <= ST.length; i++)
    {
        printf("%d\t%s\t%s\t%d\n",
               ST.Record[i].key,
               ST.Record[i].name,
               ST.Record[i].sex,
               ST.Record[i].age);
    }
    printf("\n");
}

int SearchSSTable_Seq(SSTable ST, KeyType key, int &c)
{
    // 设置哨兵
    ST.Record[0].key = key;
    int i;
    c = 0;
    for (i = ST.length; i >= 0 && ST.Record[i].key != key; i--)
    {
        c++;
    }
    return i; // 若i=0则查找失败
}

void SortSSTable(SSTable &ST)
{
    // 使用冒泡排序
    for (int i = 1; i < ST.length; i++)
    {
        for (int j = 1; j <= ST.length - i; j++)
        {
            if (ST.Record[j].key > ST.Record[j + 1].key)
            {
                RecordType temp = ST.Record[j];
                ST.Record[j] = ST.Record[j + 1];
                ST.Record[j + 1] = temp;
            }
        }
    }
}

int SearchSSTable_Bin(SSTable ST, KeyType key, int &c)
{
    int low = 1, high = ST.length;
    c = 0;

    while (low <= high)
    {
        c++;
        int mid = (low + high) / 2;
        if (key == ST.Record[mid].key)
            return mid;
        else if (key < ST.Record[mid].key)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return -1; // 查找失败
}

int SearchSSTable_Bin1(SSTable ST, KeyType key, int low, int high)
{
    if (low > high)
        return -1;

    count++; // 增加比较次数
    int mid = (low + high) / 2;

    if (key == ST.Record[mid].key)
        return mid;
    else if (key < ST.Record[mid].key)
        return SearchSSTable_Bin1(ST, key, low, mid - 1);
    else
        return SearchSSTable_Bin1(ST, key, mid + 1, high);
}

void OutElem(SSTable ST, int i)
{
    printf("找到记录：学号=%d\t姓名=%s\t性别=%s\t年龄=%d\n",
           ST.Record[i].key,
           ST.Record[i].name,
           ST.Record[i].sex,
           ST.Record[i].age);
}