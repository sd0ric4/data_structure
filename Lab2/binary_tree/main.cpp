#include "binary_tree.h"

int main() {
    BiTree T, T1, T2;

    // (1) 创建二叉树
    cout << "请输入先序序列（#表示空节点）: ";
    CreateBiTree(T);

    // (2) 递归遍历
    cout << "递归先序遍历: ";
    PreOrderTraverse(T);
    cout << "\n递归中序遍历: ";
    InOrderTraverse(T);
    cout << "\n递归后序遍历: ";
    PostOrderTraverse(T);
    cout << endl;

    // (3) 非递归遍历
    cout << "非递归先序遍历: \n";
    NRPreOrderTraverse(T);
    cout << "非递归中序遍历: \n";
    NRInOrderTraverse(T);
    cout << "非递归后序遍历: \n";
    NRPostOrderTraverse(T);

    // (4) 统计节点
    cout << "叶子节点数量: " << TNodes(T, 0) << endl;
    cout << "度为1的节点数量: " << TNodes(T, 1) << endl;
    cout << "度为2的节点数量: " << TNodes(T, 2) << endl;

    // (5) 创建二叉搜索树并计算高度
    CreateBST(T1, "DBFCAEG");
    CreateBST(T2, "ABCDEFG");

    cout << "T1的中序遍历: ";
    InOrderTraverse(T1);
    cout << "\nT1的高度: " << High(T1) << endl;

    cout << "T2的中序遍历: ";
    InOrderTraverse(T2);
    cout << "\nT2的高度: " << High(T2) << endl;

    return 0;
}
