#include <stdio.h>
#include <stdlib.h>

// 定义函数返回值状态码
#define SUCCESS 1   // 查找成功
#define UNSUCCESS 0 // 查找失败

// 关键字类型定义
typedef int KeyType;

// 线性探测法哈希表结构
typedef struct
{
    KeyType *key; // 存储关键字的数组
    int count;    // 当前表中的关键字个数
    int size;     // 哈希表的大小
} HashTable;

// 链地址法结点结构
typedef struct KeyNode
{
    KeyType key;          // 关键字值
    struct KeyNode *next; // 指向下一个结点的指针
} KeyNode, *KeyLink;

// 链地址法哈希表结构
typedef struct
{
    KeyLink *head; // 指向链表头节点的指针数组
    int count;     // 当前表中的关键字个数
    int size;      // 哈希表的大小
} HashLink;

// 哈希函数：除留余数法
int Hash(KeyType key)
{
    return key % 13; // 选择13作为除数（应选择质数以减少冲突）
}

// 创建线性探测法哈希表
void CreateHashTable(HashTable &HT, int HT_Length, KeyType key[], int KeyNum)
{
    int i, addr;

    // 初始化哈希表
    HT.size = HT_Length;                                     // 设置表长
    HT.count = KeyNum;                                       // 设置关键字个数
    HT.key = (KeyType *)malloc(HT_Length * sizeof(KeyType)); // 分配内存空间

    // 将所有位置初始化为-1，表示空位
    for (i = 0; i < HT_Length; i++)
    {
        HT.key[i] = -1;
    }

    // 插入所有关键字
    for (i = 0; i < KeyNum; i++)
    {
        addr = Hash(key[i]); // 计算初始地址

        // 如果发生冲突，使用线性探测找下一个空位
        while (HT.key[addr] != -1)
        {
            addr = (addr + 1) % HT_Length; // 线性探测的下一个位置
        }

        HT.key[addr] = key[i]; // 将关键字插入到找到的空位
    }
}

// 在线性探测法哈希表中查找关键字
int SearchHashTable(HashTable HT, KeyType key, int &p, int &c)
{
    p = Hash(key); // 计算初始查找位置
    c = 1;         // 初始化比较次数

    // 当前位置不是要找的关键字且不是空位时，继续查找
    while (HT.key[p] != key && HT.key[p] != -1)
    {
        c++;                   // 比较次数加1
        p = (p + 1) % HT.size; // 探测下一个位置
    }

    // 找到关键字，返回成功
    if (HT.key[p] == key)
    {
        return SUCCESS;
    }

    // 未找到，返回失败
    p = -1;
    return UNSUCCESS;
}

// 创建链地址法哈希表
void CreateHashLink(HashLink &HL, int HL_Length, KeyType key[], int KeyNum)
{
    int i, addr;
    KeyLink p;

    // 初始化哈希表
    HL.size = HL_Length;
    HL.count = KeyNum;
    HL.head = (KeyLink *)malloc(HL_Length * sizeof(KeyLink));

    // 初始化所有头指针为NULL
    for (i = 0; i < HL_Length; i++)
    {
        HL.head[i] = NULL;
    }

    // 插入所有关键字
    for (i = 0; i < KeyNum; i++)
    {
        addr = Hash(key[i]); // 计算哈希地址

        // 创建新结点
        p = (KeyLink)malloc(sizeof(KeyNode));
        p->key = key[i];

        // 头插法插入新结点
        p->next = HL.head[addr];
        HL.head[addr] = p;
    }
}

// 在链地址法哈希表中查找关键字
int SearchHashLink(HashLink HL, KeyType key, KeyLink &p, int &c)
{
    int addr = Hash(key); // 计算哈希地址
    p = HL.head[addr];    // 获取对应链表的头指针
    c = 1;                // 初始化比较次数（包括计算哈希地址）

    // 在链表中查找关键字
    while (p)
    {
        c++; // 每比较一次，计数加1
        if (p->key == key)
        {
            return SUCCESS; // 找到关键字，返回成功
        }
        p = p->next; // 继续查找下一个结点
    }

    return UNSUCCESS; // 未找到关键字，返回失败
}

// 输出链地址法哈希表
void OutHashLink(HashLink HL)
{
    KeyLink p;

    // 遍历每个哈希地址
    for (int i = 0; i < HL.size; i++)
    {
        printf("%d:", i); // 打印哈希地址
        p = HL.head[i];   // 获取该地址的链表头指针

        // 遍历并打印该地址下的所有关键字
        while (p)
        {
            printf("->%d", p->key);
            p = p->next;
        }
        printf("\n");
    }
}

int main()
{
    int i, j, k, total;
    // 待查找的关键字数组
    int keys[12] = {19, 14, 23, 1, 68, 20, 84, 27, 55, 11, 10, 79};
    // 用于测试查找不成功的关键字数组
    int keys1[13] = {26, 40, 15, 29, 30, 18, 32, 46, 60, 74, 36, 24, 38};
    int n = 12, n1 = 13; // 数组长度
    int HT_Length = 16;  // 哈希表长度
    HashTable HT;        // 线性探测法哈希表

    // 打印原始关键字
    printf("关键字表:\n");
    for (i = 0; i < n; i++)
        printf("%2d ", keys[i]);

    // 创建并显示线性探测法哈希表
    CreateHashTable(HT, HT_Length, keys, n);
    printf("\n\n线性探测再散列哈希表:\n");
    for (i = 0; i < HT_Length; i++)
        printf("%d ", HT.key[i]);
    printf("\n");

    // 测试线性探测法查找成功的平均查找长度(ASL)
    total = 0;
    for (i = 0; i < n; i++)
    {
        printf("\n查找关键字=%2d", keys[i]);
        SearchHashTable(HT, keys[i], j, k);
        total = total + k;
        printf("  所在哈希表下标=%2d", j);
        printf("  关键字比较次数=%2d", k);
    }
    printf("\n\n查找成功ASL=%f\n", (float)total / n);

    // 测试线性探测法查找不成功的平均查找长度
    total = 0;
    for (i = 0; i < n1; i++)
    {
        printf("\n查找关键字=%2d", keys1[i]);
        SearchHashTable(HT, keys1[i], j, k);
        total = total + k;
        printf("  所在哈希表下标=%2d", j);
        printf("  关键字比较次数=%2d", k);
    }
    printf("\n\n查找不成功ASL=%f\n", (float)total / n1);

    // 创建并显示链地址法哈希表
    HashLink HL;
    KeyLink p;
    CreateHashLink(HL, 13, keys, n);
    printf("\n链地址法哈希表:\n");
    OutHashLink(HL);

    // 测试链地址法查找成功的平均查找长度
    total = 0;
    for (i = 0; i < n; i++)
    {
        printf("\n查找关键字=%2d", keys[i]);
        SearchHashLink(HL, keys[i], p, k);
        total = total + k;
        printf("  所在哈希表下标=%2d", keys[i] % 13);
        printf("  关键字比较次数=%2d", k);
    }
    printf("\n\n查找成功ASL=%f\n", (float)total / n);

    // 测试链地址法查找不成功的平均查找长度
    total = 0;
    for (i = 0; i < n1; i++)
    {
        printf("\n查找关键字=%2d", keys1[i]);
        SearchHashLink(HL, keys1[i], p, k);
        total = total + k;
        printf("  所在哈希表下标=%2d", keys1[i] % 13);
        printf("  关键字比较次数=%2d", k);
    }
    printf("\n\n查找不成功ASL=%f\n", (float)total / n1);

    return 1;
}