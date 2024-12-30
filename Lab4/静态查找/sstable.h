#ifndef SSTABLE_H
#define SSTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int KeyType; // 关键字的数据类型

// 数据元素（记录）的类型定义
typedef struct
{
    KeyType key;      // 学号(关键字)
    const char *name; // 姓名
    const char *sex;  // 性别
    int age;          // 年龄
} RecordType;

// 静态查找表的类型定义
typedef struct
{
    RecordType *Record; // 数据元素存储空间的基址(一维数组名)
    int length;         // 表的长度（元素个数）
} SSTable;

enum SearchType
{
    SEQUENTIAL,
    BINARY,
    BINARY_RECURSIVE
};

// 函数声明
void CreateSSTable(SSTable &ST, int n);
void TraversSSTable(SSTable ST);
int SearchSSTable_Seq(SSTable ST, KeyType key, int &c);
void SortSSTable(SSTable &ST);
int SearchSSTable_Bin(SSTable ST, KeyType key, int &c);
int SearchSSTable_Bin1(SSTable ST, KeyType key, int low, int high);
void OutElem(SSTable ST, int i);
int Search(SSTable ST, KeyType key, int &c, SearchType type);

extern int count; // 用于递归版本的折半查找中计数

#endif