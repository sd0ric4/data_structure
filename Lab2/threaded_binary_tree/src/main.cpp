#include "threaded_binary_tree.h"
#include <iostream>
#include <vector>
#include <string>

int main()
{
    // A B D # F # # # C E # # #
    // 允许用户输入前序遍历序列
    std::vector<std::string> preorder;
    std::string input;
    std::cout << "请输入前序遍历序列（以空格分隔，# 表示空节点）：";
    while (std::cin >> input)
    {
        preorder.push_back(input);
        if (std::cin.peek() == '\n')
        {
            break;
        }
    }

    BiThrTree root;
    InitBiThrTree(root, preorder, "#");

    // 生成二叉树的图像
    std::string dotFilename = "binary_tree.dot";
    std::string pngFilename = "binary_tree.png";
    generateDotAndPng(root, dotFilename, pngFilename);

    std::cout << "二叉树的图像已生成：" << pngFilename << std::endl;
    std::cout << "中序遍历尚未线索化的二叉树：" << std::endl;
    // 中序遍历二叉树
    InOrderTraverse(root);

    BiThrTree Thrt;
    // 中序线索化
    InOrderThreading(Thrt, root);

    std::cout << "中序遍历线索化后的二叉树：" << std::endl;
    // 中序遍历线索化后的二叉树
    InOrderTraverse(Thrt);

    std::cout << "按线索和孩子中序遍历线索二叉树：" << std::endl;
    // 按线索和孩子中序遍历线索二叉树
    InOrderTraverse_Thr(Thrt);

    return 0;
}