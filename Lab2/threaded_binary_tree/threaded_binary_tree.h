#ifndef THREADED_BINARY_TREE_H
#define THREADED_BINARY_TREE_H

#include <iostream>
#include <stack>
using namespace std;

typedef int Status;
typedef char TElemType;

typedef struct BiThrNode {
    TElemType data;           // 结点数据
    struct BiThrNode *lchild; // 左孩子或前驱
    struct BiThrNode *rchild; // 右孩子或后继
    int LTag, RTag;           // 左右标志，0表示孩子，1表示线索
} BiThrNode, *BiThrTree;

// 函数声明
void InitBiThrTree(BiThrTree &T);
void InOrderThreading(BiThrTree &Thrt, BiThrTree T);
void InOrderTraverse(BiThrTree T);
void InOrderTraverse_Thr(BiThrTree T);

#endif
