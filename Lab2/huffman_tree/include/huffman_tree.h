#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <cstring>
#include <iostream>
using namespace std;

// 赫夫曼树结点结构
typedef struct
{
    int weight; // 权重
    int parent; // 父节点索引
    int lchild; // 左孩子索引
    int rchild; // 右孩子索引
} HTNode, *HTree;

// 动态分配字符串数组存储赫夫曼编码表
typedef char **HTCode;

// 函数声明
void InitHTree(HTree &HT, int *w, int n);
void CreateHTree(HTree &HT, int n);
void HTCoding(HTree HT, HTCode &HC, int n);

#endif
