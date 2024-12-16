#include "threaded_binary_tree.h"
#include <iostream>
#include <fstream>
#include <functional>
#include <tabulate/table.hpp>
using namespace tabulate;
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

// 初始化线索二叉树
void InitBiThrTree(BiThrTree &T, const std::vector<std::string> &preorder, const std::string &nullSymbol)
{
    int index = 0;
    T = buildTree(preorder, index, nullSymbol);
}

// 中序线索化
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

// 非递归中序遍历线索二叉树
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