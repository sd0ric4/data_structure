#include "threaded_binary_tree.h"

int main() {
    BiThrTree T, Thrt;

    // (1) 初始化尚未线索化的二叉树
    cout << "请输入先序序列（#表示空节点）: ";
    InitBiThrTree(T);

    // (2) 输出中序遍历的结果（尚未线索化）
    cout << "中序遍历二叉树（递归，尚未线索化）:" << endl;
    InOrderTraverse(T);

    // (3) 中序线索化
    InOrderThreading(Thrt, T);

    // (4) 输出线索化后的中序遍历结果（递归）
    cout << "线索化后的中序遍历（递归）:" << endl;
    InOrderTraverse(Thrt->lchild);

    // (5) 输出线索化后的中序遍历结果（非递归）
    cout << "线索化后的中序遍历（非递归）:" << endl;
    InOrderTraverse_Thr(Thrt);

    return 0;
}
