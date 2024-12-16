#include "binary_tree.h"
#include <iostream>
#include <fstream>

/**
 * 根据前序遍历序列构建二叉树
 * @param preorder 前序遍历序列
 * @param index 当前索引
 * @param nullSymbol 表示空节点的符号
 * @return 构建的二叉树根节点
 *
 * 算法实现:
 * 1. 如果索引超出序列范围或当前元素为空节点符号，返回空节点。
 * 2. 创建新节点并递归构建其左子树和右子树。
 */
BiTree buildTree(const std::vector<std::string> &preorder, int &index, const std::string &nullSymbol)
{
    if (static_cast<size_t>(index) >= preorder.size() || preorder[index] == nullSymbol)
    {
        index++;
        return nullptr;
    }
    BiTree node = new BiTNode(preorder[index++][0]);
    node->lchild = buildTree(preorder, index, nullSymbol);
    node->rchild = buildTree(preorder, index, nullSymbol);
    return node;
}

/**
 * 生成二叉树的DOT格式表示
 * @param node 当前节点
 * @param ofs 输出文件流
 * @return void
 *
 * 算法实现:
 * 1. 如果当前节点存在，递归生成其左子树和右子树的DOT表示。
 * 2. 如果左子树或右子树为空，生成不可见的边。
 */
void generateDot(BiTree node, std::ofstream &ofs)
{
    if (node)
    {
        if (node->lchild)
        {
            ofs << "    \"" << node->data << "\" -> \"" << node->lchild->data << "\" [label=\"L\", color=\"blue\"];\n";
            generateDot(node->lchild, ofs);
        }
        else
        {
            ofs << "    \"" << node->data << "\" -> \"" << node->data << "_left\" [style=invis];\n";
            ofs << "    \"" << node->data << "_left\" [style=invis, width=0, height=0];\n";
        }

        if (node->rchild)
        {
            ofs << "    \"" << node->data << "\" -> \"" << node->rchild->data << "\" [label=\"R\", color=\"red\"];\n";
            generateDot(node->rchild, ofs);
        }
        else
        {
            ofs << "    \"" << node->data << "\" -> \"" << node->data << "_right\" [style=invis];\n";
            ofs << "    \"" << node->data << "_right\" [style=invis, width=0, height=0];\n";
        }

        if (!node->lchild && !node->rchild)
        {
            ofs << "    \"" << node->data << "\";\n";
        }
    }
}

/**
 * 创建二叉树
 * @param T 二叉树根节点的引用
 * @param preorder 前序遍历序列
 * @param nullSymbol 表示空节点的符号
 * @return void
 *
 * 算法实现:
 * 1. 初始化索引为0。
 * 2. 调用buildTree函数构建二叉树。
 */
void CreateBiTree(BiTree &T, const std::vector<std::string> &preorder, const std::string &nullSymbol)
{
    int index = 0;
    T = buildTree(preorder, index, nullSymbol);
}

/**
 * 前序遍历二叉树
 * @param T 二叉树根节点
 * @return void
 *
 * 算法实现:
 * 1. 如果当前节点存在，输出其数据。
 * 2. 递归遍历其左子树和右子树。
 */
void PreOrderTraverse(BiTree T)
{
    if (T)
    {
        std::cout << T->data << " ";
        PreOrderTraverse(T->lchild);
        PreOrderTraverse(T->rchild);
    }
}

/**
 * 中序遍历二叉树
 * @param T 二叉树根节点
 * @return void
 *
 * 算法实现:
 * 1. 如果当前节点存在，递归遍历其左子树。
 * 2. 输出当前节点的数据。
 * 3. 递归遍历其右子树。
 */
void InOrderTraverse(BiTree T)
{
    if (T)
    {
        InOrderTraverse(T->lchild);
        std::cout << T->data << " ";
        InOrderTraverse(T->rchild);
    }
}

/**
 * 后序遍历二叉树
 * @param T 二叉树根节点
 * @return void
 *
 * 算法实现:
 * 1. 如果当前节点存在，递归遍历其左子树和右子树。
 * 2. 输出当前节点的数据。
 */
void PostOrderTraverse(BiTree T)
{
    if (T)
    {
        PostOrderTraverse(T->lchild);
        PostOrderTraverse(T->rchild);
        std::cout << T->data << " ";
    }
}

/**
 * 生成二叉树的DOT文件并转换为PNG图像
 * @param root 二叉树根节点
 * @param dotFilename DOT文件名
 * @param pngFilename PNG文件名
 * @return void
 *
 * 算法实现:
 * 1. 打开DOT文件并写入DOT格式的二叉树表示。
 * 2. 关闭DOT文件。
 * 3. 使用系统命令将DOT文件转换为PNG图像。
 */
void generateDotAndPng(BiTree root, const std::string &dotFilename, const std::string &pngFilename)
{
    std::ofstream ofs(dotFilename);
    ofs << "digraph G {\n";
    ofs << "    node [shape=circle];\n";
    generateDot(root, ofs);
    ofs << "}\n";
    ofs.close();

    std::string command = "dot -Tpng " + dotFilename + " -o " + pngFilename;
    system(command.c_str());
}

/**
 * 统计二叉树中度为d的节点数
 * @param T 二叉树根节点
 * @param d 节点的度
 * @return 节点数
 *
 * 算法实现:
 * 1. 如果当前节点为空，返回0。
 * 2. 根据节点的度统计符合条件的节点数。
 * 3. 递归统计左子树和右子树中符合条件的节点数。
 */
int TNodes(BiTree T, int d)
{
    if (!T)
        return 0;
    int count = 0;
    // 判断规则：度为0且无左右孩子，或者度为1且只有左孩子或右孩子，或者度为2，说明是度为d的节点
    if ((d == 0 && !T->lchild && !T->rchild) ||
        (d == 1 && ((T->lchild && !T->rchild) || (!T->lchild && T->rchild))) ||
        (d == 2 && T->lchild && T->rchild))
    {
        count = 1;
    }
    // 递归统计左子树和右子树中符合条件的节点数
    return count + TNodes(T->lchild, d) + TNodes(T->rchild, d);
}

/**
 * 计算二叉树的高度
 * @param T 二叉树根节点
 * @return 二叉树的高度
 *
 * 算法实现:
 * 1. 如果当前节点为空，返回0。
 * 2. 递归计算左子树和右子树的高度。
 * 3. 返回左右子树高度的最大值加1。
 */
int High(BiTree T)
{
    if (!T)
        return 0;
    int leftHeight = High(T->lchild);
    int rightHeight = High(T->rchild);
    // 返回左右子树高度的最大值加1
    return std::max(leftHeight, rightHeight) + 1;
}

/**
 * 插入节点到二叉搜索树
 * @param T 二叉搜索树根节点的引用
 * @param data 插入的数据
 * @return void
 *
 * 算法实现:
 * 1. 如果当前节点为空，创建新节点并插入。
 * 2. 如果数据小于当前节点的数据，递归插入到左子树。
 * 3. 如果数据大于当前节点的数据，递归插入到右子树。
 */
void InsertBST(BiTree &T, TElemType data)
{
    if (!T)
    {
        T = new BiTNode(data);
        return;
    }
    if (data < T->data)
        InsertBST(T->lchild, data);
    else if (data > T->data)
        InsertBST(T->rchild, data);
}

/**
 * 创建二叉搜索树
 * @param T 二叉搜索树根节点的引用
 * @param chars 插入的字符数组
 * @return void
 *
 * 算法实现:
 * 1. 初始化二叉搜索树为空。
 * 2. 遍历字符数组，依次插入每个字符到二叉搜索树。
 */
void CreateBST(BiTree &T, const char *chars)
{
    T = nullptr;
    for (int i = 0; chars[i] != '\0'; ++i)
    {
        InsertBST(T, chars[i]);
    }
}