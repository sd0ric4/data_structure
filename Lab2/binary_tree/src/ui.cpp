#include "ui.h"
#include <iostream>
#include <stack>

/**
 * 非递归前序遍历二叉树
 * @param T 二叉树根节点
 *
 * 算法实现:
 * 1. 如果树为空，直接返回。
 * 2. 使用栈保存节点，首先将根节点入栈。
 * 3. 当栈不为空时，弹出栈顶节点并访问它。
 * 4. 如果右孩子存在，将右孩子入栈。
 * 5. 如果左孩子存在，将左孩子入栈。
 */
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

/**
 * 非递归中序遍历二叉树
 * @param T 二叉树根节点
 *
 * 算法实现:
 * 1. 使用栈保存节点，从根节点开始。
 * 2. 当节点不为空或栈不为空时，执行以下操作：
 *    a. 将当前节点入栈，并移动到左孩子。
 *    b. 如果当前节点为空，弹出栈顶节点并访问它，然后移动到右孩子。
 */
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

/**
 * 非递归后序遍历二叉树
 * @param T 二叉树根节点
 *
 * 算法实现:
 * 1. 如果树为空，直接返回。
 * 2. 使用两个栈保存节点，首先将根节点入第一个栈。
 * 3. 当第一个栈不为空时，弹出栈顶节点并将其入第二个栈。
 * 4. 如果左孩子存在，将左孩子入第一个栈。
 * 5. 如果右孩子存在，将右孩子入第一个栈。
 * 6. 当第二个栈不为空时，弹出栈顶节点并访问它。
 */
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