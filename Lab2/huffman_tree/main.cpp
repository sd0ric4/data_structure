#include "huffman_tree.h"

int main() {
    HTree HT;                               // 赫夫曼树
    HTCode HC;                              // 赫夫曼编码
    int w[] = {5, 29, 7, 8, 14, 23, 3, 11}; // 权重数组
    int n = sizeof(w) / sizeof(w[0]);       // 叶子结点数

    // (1) 初始化赫夫曼树
    InitHTree(HT, w, n);
    cout << "初始化的赫夫曼树：" << endl;
    for (int i = 0; i < n; i++) {
        cout << "结点 " << i << ": weight=" << HT[i].weight
             << ", parent=" << HT[i].parent << ", lchild=" << HT[i].lchild
             << ", rchild=" << HT[i].rchild << endl;
    }

    // (2) 构造赫夫曼树
    CreateHTree(HT, n);
    cout << "\n构造后的赫夫曼树：" << endl;
    for (int i = 0; i < 2 * n - 1; i++) {
        cout << "结点 " << i << ": weight=" << HT[i].weight
             << ", parent=" << HT[i].parent << ", lchild=" << HT[i].lchild
             << ", rchild=" << HT[i].rchild << endl;
    }

    // (3) 生成赫夫曼编码
    HTCoding(HT, HC, n);
    cout << "\n赫夫曼编码：" << endl;
    for (int i = 0; i < n; i++) {
        cout << "结点 " << i << " (权重 " << w[i] << "): " << HC[i] << endl;
    }

    // 释放内存
    for (int i = 0; i < n; i++) {
        delete[] HC[i];
    }
    delete[] HC;
    delete[] HT;

    return 0;
}
