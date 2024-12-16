#include "threaded_binary_tree.h"
#include <iostream>
#include <fstream>
#include <functional>
#include <tabulate/table.hpp>
using namespace tabulate;

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
BiThrTree buildTree(const std::vector<std::string> &preorder, int &index, const std::string &nullSymbol)
{
    if (static_cast<size_t>(index) >= preorder.size() || preorder[index] == nullSymbol)
    {
        index++;
        return nullptr;
    }
    BiThrTree node = new BiThrNode(preorder[index++][0]);
    node->lchild = buildTree(preorder, index, nullSymbol);
    node->rchild = buildTree(preorder, index, nullSymbol);
    node->LTag = (node->lchild == nullptr) ? 1 : 0;
    node->RTag = (node->rchild == nullptr) ? 1 : 0;
    return node;
}

/**
 * 初始化线索二叉树
 * @param T 线索二叉树根节点
 * @param preorder 前序遍历序列
 * @param nullSymbol 表示空节点的符号
 *
 * 算法实现:
 * 1. 调用buildTree函数根据前序遍历序列构建二叉树。
 */
void InitBiThrTree(BiThrTree &T, const std::vector<std::string> &preorder, const std::string &nullSymbol)
{
    int index = 0;
    T = buildTree(preorder, index, nullSymbol);
}

/**
 * 中序线索化
 * @param Thrt 线索二叉树头节点
 * @param T 线索二叉树根节点
 *
 * 算法实现:
 * 1. 创建头节点并初始化。
 * 2. 如果树为空，头节点的左孩子指向自己。
 * 3. 否则，头节点的左孩子指向根节点，进行中序线索化。
 * 4. 更新最后一个节点的右孩子指向头节点。
 */
void InOrderThreading(BiThrTree &Thrt, BiThrTree T)
{
    Thrt = new BiThrNode('@');
    Thrt->LTag = 0;
    Thrt->RTag = 1;
    Thrt->rchild = Thrt;

    if (!T)
    {
        Thrt->lchild = Thrt;
    }
    else
    {
        Thrt->lchild = T;
        BiThrTree pre = Thrt;
        // 递归中序线索化
        std::function<void(BiThrTree)> InThreading = [&](BiThrTree p)
        {
            if (p)
            {
                InThreading(p->lchild);
                if (!p->lchild)
                {
                    p->LTag = 1;
                    p->lchild = pre;
                }
                if (!pre->rchild)
                {
                    pre->RTag = 1;
                    pre->rchild = p;
                }
                pre = p;
                InThreading(p->rchild);
            }
        };
        InThreading(T);
        pre->rchild = Thrt;
        pre->RTag = 1;
        Thrt->rchild = pre;
    }
}

/**
 * 中序遍历线索二叉树
 * @param T 线索二叉树根节点
 *
 * 算法实现:
 * 1. 创建表格并添加表头。
 * 2. 递归遍历树的每个节点，添加节点信息到表格中。
 * 3. 打印表格。
 */
void InOrderTraverse(BiThrTree T)
{
    Table table;
    table.add_row({"LTag", "LChild", "Data", "RChild", "RTag"});
    table[0].format().font_color(Color::yellow).font_style({FontStyle::bold});
    std::function<void(BiThrTree)> traverse = [&](BiThrTree node)
    {
        if (node)
        {
            if (node->LTag == 0)
                traverse(node->lchild);
            auto row = table.add_row({std::to_string(node->LTag), (node->lchild ? std::string(1, node->lchild->data) : "^"), std::string(1, node->data), (node->rchild ? std::string(1, node->rchild->data) : "^"), std::to_string(node->RTag)});
            row.format().font_color(Color::cyan);
            if (node->RTag == 0)
                traverse(node->rchild);
        }
    };
    traverse(T);
    std::cout << table << std::endl;
}

/**
 * 非递归中序遍历线索二叉树
 * @param T 线索二叉树根节点
 *
 * 算法实现:
 * 1. 创建表格并添加表头。
 * 2. 从根节点开始，找到最左下节点。
 * 3. 遍历节点并添加节点信息到表格中。
 * 4. 打印表格。
 */
void InOrderTraverse_Thr(BiThrTree T)
{
    Table table;
    table.add_row({"LTag", "LChild", "Data", "RChild", "RTag"});
    table[0].format().font_color(Color::yellow).font_style({FontStyle::bold});
    BiThrTree p = T->lchild;
    while (p != T)
    {
        while (p->LTag == 0)
            p = p->lchild;
        auto row = table.add_row({std::to_string(p->LTag), (p->lchild ? std::string(1, p->lchild->data) : "^"), std::string(1, p->data), (p->rchild ? std::string(1, p->rchild->data) : "^"), std::to_string(p->RTag)});
        row.format().font_color(Color::cyan);
        while (p->RTag == 1 && p->rchild != T)
        {
            p = p->rchild;
            row = table.add_row({std::to_string(p->LTag), (p->lchild ? std::string(1, p->lchild->data) : "^"), std::string(1, p->data), (p->rchild ? std::string(1, p->rchild->data) : "^"), std::to_string(p->RTag)});
            row.format().font_color(Color::cyan);
        }
        p = p->rchild;
    }
    std::cout << table << std::endl;
}

/**
 * 生成dot文件
 * @param node 当前节点
 * @param ofs 输出文件流
 *
 * 算法实现:
 * 1. 如果节点存在，递归生成左子树和右子树的dot文件内容。
 * 2. 如果节点不存在，生成不可见节点。
 */
void generateDot(BiThrTree node, std::ofstream &ofs)
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
 * 生成dot文件和png图片
 * @param root 线索二叉树根节点
 * @param dotFilename dot文件名
 * @param pngFilename png文件名
 *
 * 算法实现:
 * 1. 打开dot文件并写入dot文件头。
 * 2. 调用generateDot函数生成dot文件内容。
 * 3. 写入dot文件尾并关闭文件。
 * 4. 调用系统命令生成png图片。
 */
void generateDotAndPng(BiThrTree root, const std::string &dotFilename, const std::string &pngFilename)
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