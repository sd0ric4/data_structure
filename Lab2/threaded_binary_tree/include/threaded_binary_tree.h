#ifndef THREADED_BINARY_TREE_H
#define THREADED_BINARY_TREE_H

#include <vector>
#include <string>

typedef int Status; // 声明函数类型名

typedef char TElemType; // 声明结点元素值得类型

typedef struct BiThrNode
{ // 定义线索二叉链表的结点结构和类型

    TElemType data;
    struct BiThrNode *lchild, *rchild;
    int LTag, RTag;
    BiThrNode(TElemType x) : data(x), lchild(nullptr), rchild(nullptr), LTag(0), RTag(0) {}
} BiThrNode, *BiThrTree;

BiThrTree buildTree(const std::vector<std::string> &preorder, int &index, const std::string &nullSymbol);
void InitBiThrTree(BiThrTree &T, const std::vector<std::string> &preorder, const std::string &nullSymbol);
void InOrderThreading(BiThrTree &Thrt, BiThrTree T);
void InOrderTraverse(BiThrTree T);
void InOrderTraverse_Thr(BiThrTree T);
void generateDotAndPng(BiThrTree root, const std::string &dotFilename, const std::string &pngFilename);

#endif // THREADED_BINARY_TREE_H