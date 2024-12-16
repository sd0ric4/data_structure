#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <vector>
#include <string>

typedef char TElemType;
/**
 * @brief 二叉树结点
 * @param data 数据
 * @param lchild 左孩子
 * @param rchild 右孩子
 *
 */
typedef struct BiTNode
{
    TElemType data;
    struct BiTNode *lchild, *rchild;
    BiTNode(TElemType x) : data(x), lchild(nullptr), rchild(nullptr) {}
} BiTNode, *BiTree;

BiTree buildTree(const std::vector<std::string> &preorder, int &index, const std::string &nullSymbol);
void generateDot(BiTree node, std::ofstream &ofs);
void CreateBiTree(BiTree &T, const std::vector<std::string> &preorder, const std::string &nullSymbol);
void PreOrderTraverse(BiTree T);
void InOrderTraverse(BiTree T);
void PostOrderTraverse(BiTree T);
void generateDotAndPng(BiTree root, const std::string &dotFilename, const std::string &pngFilename);
int TNodes(BiTree T, int d);
int High(BiTree T);
void CreateBST(BiTree &T, const char *chars);

#endif // BINARY_TREE_H