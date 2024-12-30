#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 1
#define UNSUCCESS 0

typedef int KeyType;

// Linear probing hash table structure
typedef struct
{
    KeyType *key;
    int count;
    int size;
} HashTable;

// Chain addressing node structure
typedef struct KeyNode
{
    KeyType key;
    struct KeyNode *next;
} KeyNode, *KeyLink;

// Chain addressing hash table structure
typedef struct
{
    KeyLink *head;
    int count;
    int size;
} HashLink;

// Hash function
int Hash(KeyType key)
{
    return key % 13;
}

// Create hash table with linear probing
void CreateHashTable(HashTable &HT, int HT_Length, KeyType key[], int KeyNum)
{
    int i, addr;

    // Initialize hash table
    HT.size = HT_Length;
    HT.count = KeyNum;
    HT.key = (KeyType *)malloc(HT_Length * sizeof(KeyType));

    // Initialize all positions with -1
    for (i = 0; i < HT_Length; i++)
    {
        HT.key[i] = -1;
    }

    // Insert all keys
    for (i = 0; i < KeyNum; i++)
    {
        addr = Hash(key[i]); // Get initial address

        // If collision occurs, use linear probing
        while (HT.key[addr] != -1)
        {
            addr = (addr + 1) % HT_Length;
        }

        HT.key[addr] = key[i];
    }
}

// Search in hash table with linear probing
int SearchHashTable(HashTable HT, KeyType key, int &p, int &c)
{
    p = Hash(key); // Initial position
    c = 1;         // Initialize comparison counter

    while (HT.key[p] != key && HT.key[p] != -1)
    {
        c++;
        p = (p + 1) % HT.size;
    }

    if (HT.key[p] == key)
    {
        return SUCCESS;
    }

    p = -1;
    return UNSUCCESS;
}

// Create hash table with chaining
void CreateHashLink(HashLink &HL, int HL_Length, KeyType key[], int KeyNum)
{
    int i, addr;
    KeyLink p;

    // Initialize hash table
    HL.size = HL_Length;
    HL.count = KeyNum;
    HL.head = (KeyLink *)malloc(HL_Length * sizeof(KeyLink));

    // Initialize all head pointers to NULL
    for (i = 0; i < HL_Length; i++)
    {
        HL.head[i] = NULL;
    }

    // Insert all keys
    for (i = 0; i < KeyNum; i++)
    {
        addr = Hash(key[i]);

        // Create new node
        p = (KeyLink)malloc(sizeof(KeyNode));
        p->key = key[i];

        // Insert at beginning of list (head insertion)
        p->next = HL.head[addr];
        HL.head[addr] = p;
    }
}

// Search in hash table with chaining
int SearchHashLink(HashLink HL, KeyType key, KeyLink &p, int &c)
{
    int addr = Hash(key);
    p = HL.head[addr];
    c = 0;

    // Search in the linked list
    while (p)
    {
        c++;
        if (p->key == key)
        {
            return SUCCESS;
        }
        p = p->next;
    }

    return UNSUCCESS;
}

// Output hash table with chaining
void OutHashLink(HashLink HL)
{
    KeyLink p;

    for (int i = 0; i < HL.size; i++)
    {
        printf("%d:", i);
        p = HL.head[i];

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
    int keys[12] = {19, 14, 23, 1, 68, 20, 84, 27, 55, 11, 10, 79};
    int keys1[13] = {26, 40, 15, 29, 30, 18, 32, 46, 60, 74, 36, 24, 38};
    int n = 12, n1 = 13;
    int HT_Length = 16;
    HashTable HT;

    printf("关键字表:\n");
    for (i = 0; i < n; i++)
        printf("%2d ", keys[i]);

    CreateHashTable(HT, HT_Length, keys, n);
    printf("\n\n线性探测再散列哈希表:\n");
    for (i = 0; i < HT_Length; i++)
        printf("%d ", HT.key[i]);
    printf("\n");

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

    HashLink HL;
    KeyLink p;
    CreateHashLink(HL, 13, keys, n);
    printf("\n链地址法哈希表:\n");
    OutHashLink(HL);

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