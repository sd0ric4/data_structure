#include "threaded_binary_tree.h"
#include <iostream>
#include <vector>
#include <string>

int main()
{
    std::vector<std::string> preorder = {"A", "B", "D", "#", "F", "#", "#", "#", "C", "E", "#", "#", "#"};
    BiThrTree root;
    InitBiThrTree(root, preorder, "#");

    // 生成二叉树的图像
    std::string dotFilename = "binary_tree.dot";
    std::string pngFilename = "binary_tree.png";
    generateDotAndPng(root, dotFilename, pngFilename);

    std::cout << "二叉树的图像已生成：" << pngFilename << std::endl;
    std::cout << "中序遍历尚未线索化的二叉树：" << std::endl;
    InOrderTraverse(root);

    BiThrTree Thrt;
    InOrderThreading(Thrt, root);

    std::cout << "中序遍历线索化后的二叉树：" << std::endl;
    InOrderTraverse(Thrt);

    std::cout << "按线索和孩子中序遍历线索二叉树：" << std::endl;
    InOrderTraverse_Thr(Thrt);

    return 0;
}