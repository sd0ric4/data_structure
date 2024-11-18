#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <cmath>
#include <cstring>
#include <iostream>
#include <queue>
#include <stack>

using namespace std;

typedef int Status;
typedef char TElemType;

typedef struct BiTNode {
    TElemType data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

// 二叉树的基本操作函数声明
void CreateBiTree(BiTree &T);
void PreOrderTraverse(BiTree T);
void InOrderTraverse(BiTree T);
void PostOrderTraverse(BiTree T);
void NRPreOrderTraverse(BiTree T);
void NRInOrderTraverse(BiTree T);
void NRPostOrderTraverse(BiTree T);
int TNodes(BiTree T, int d);
int High(BiTree T);
void CreateBST(BiTree &T, const char *chars);

#endif
