#include "ui.h"
#include <iostream>
#include <stack>

void NRPreOrderTraverse(BiTree T)
{
    if (!T)
        return;
    std::stack<BiTree> s;
    s.push(T);
    while (!s.empty())
    {
        BiTree node = s.top();
        s.pop();
        std::cout << "\033[32m访问结点: " << node->data << "\033[0m" << std::endl; // 绿色
        if (node->rchild)
        {
            s.push(node->rchild);
            std::cout << "\033[31m右孩子进栈: " << node->rchild->data << "\033[0m" << std::endl; // 红色
        }
        if (node->lchild)
        {
            s.push(node->lchild);
            std::cout << "\033[34m左孩子进栈: " << node->lchild->data << "\033[0m" << std::endl; // 蓝色
        }
    }
}

void NRInOrderTraverse(BiTree T)
{
    std::stack<BiTree> s;
    BiTree node = T;
    while (node || !s.empty())
    {
        while (node)
        {
            s.push(node);
            std::cout << "结点进栈: " << node->data << std::endl;
            node = node->lchild;
        }
        if (!s.empty())
        {
            node = s.top();
            s.pop();
            std::cout << "\033[32m访问结点: " << node->data << "\033[0m" << std::endl; // 绿色
            node = node->rchild;
        }
    }
}

void NRPostOrderTraverse(BiTree T)
{
    if (!T)
        return;
    std::stack<BiTree> s1, s2;
    s1.push(T);
    while (!s1.empty())
    {
        BiTree node = s1.top();
        s1.pop();
        s2.push(node);
        if (node->lchild)
        {
            s1.push(node->lchild);
            std::cout << "\033[34m左孩子进栈: " << node->lchild->data << "\033[0m" << std::endl; // 蓝色
        }
        if (node->rchild)
        {
            s1.push(node->rchild);
            std::cout << "\033[31m右孩子进栈: " << node->rchild->data << "\033[0m" << std::endl; // 红色
        }
    }
    while (!s2.empty())
    {
        BiTree node = s2.top();
        s2.pop();
        std::cout << "\033[32m访问结点: " << node->data << "\033[0m" << std::endl; // 绿色
    }
}