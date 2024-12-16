#include "binary_tree.h"
#include "ui.h"
#include <iostream>
#include <vector>
#include <string>

int main()
{
    std::vector<std::string> preorder = {"A", "B", "D", "#", "F", "#", "#", "#", "C", "E", "#", "#", "#"};
    BiTree root;
    CreateBiTree(root, preorder, "#");

    std::cout << "递归先序遍历: ";
    PreOrderTraverse(root);
    std::cout << std::endl;

    std::cout << "递归中序遍历: ";
    InOrderTraverse(root);
    std::cout << std::endl;

    std::cout << "递归后序遍历: ";
    PostOrderTraverse(root);
    std::cout << std::endl;

    std::cout << "非递归先序遍历: " << std::endl;
    NRPreOrderTraverse(root);
    std::cout << std::endl;

    std::cout << "非递归中序遍历: " << std::endl;
    NRInOrderTraverse(root);
    std::cout << std::endl;

    std::cout << "非递归后序遍历: " << std::endl;
    NRPostOrderTraverse(root);
    std::cout << std::endl;
    // TNodes(T)函数，输出二叉树T度分别为0、1、2的结点数。
    std::cout << "\033[32mTNodes(T, 0) = " << TNodes(root, 0) << "\033[0m" << std::endl; // 绿色
    std::cout << "\033[33mTNodes(T, 1) = " << TNodes(root, 1) << "\033[0m" << std::endl; // 黄色
    std::cout << "\033[31mTNodes(T, 2) = " << TNodes(root, 2) << "\033[0m" << std::endl; // 红色
    // 调用CreateBST(T1,"DBFCAEG")，CreateBST(T2,"ABCDEFG")，创建两棵二叉树，对它们进行中序遍历，并调用High()函数输出其高度。分析比较其结果。
    BiTree T1 = nullptr, T2 = nullptr;
    CreateBST(T1, "DBFCAEG");
    CreateBST(T2, "ABCDEFG");
    std::cout << "递归中序遍历 T1: " << std::endl;
    InOrderTraverse(T1);
    std::cout << std::endl;
    std::cout << "T1的高度: " << High(T1) << std::endl;
    std::cout << "递归中序遍历 T2: " << std::endl;
    InOrderTraverse(T2);
    std::cout << std::endl;
    std::cout << "T2的高度: " << High(T2) << std::endl;

    generateDotAndPng(root, "root.dot", "root.png");
    generateDotAndPng(T1, "T1.dot", "T1.png");
    generateDotAndPng(T2, "T2.dot", "T2.png");

    return 0;
}