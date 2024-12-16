#include "huffman_tree.h"

// 初始化赫夫曼树
void InitHTree(HTree &HT, int *w, int n) {
    int m = 2 * n - 1; // 总结点数
    HT = new HTNode[m];

    for (int i = 0; i < n; i++) {
        HT[i].weight = w[i];
        HT[i].parent = -1;
        HT[i].lchild = -1;
        HT[i].rchild = -1;
    }
    for (int i = n; i < m; i++) {
        HT[i].weight = -1;
        HT[i].parent = -1;
        HT[i].lchild = -1;
        HT[i].rchild = -1;
    }
}

// 从赫夫曼树中选择两个最小权重且父节点为空的结点
void Select(HTree HT, int n, int &s1, int &s2) {
    s1 = s2 = -1;
    for (int i = 0; i < n; i++) {
        if (HT[i].parent == -1) { // 未被选中的结点
            if (s1 == -1 || HT[i].weight < HT[s1].weight) {
                s2 = s1;
                s1 = i;
            } else if (s2 == -1 || HT[i].weight < HT[s2].weight) {
                s2 = i;
            }
        }
    }
}

// 构造赫夫曼树
void CreateHTree(HTree &HT, int n) {
    int m = 2 * n - 1; // 总结点数
    for (int i = n; i < m; i++) {
        int s1, s2;
        Select(HT, i, s1, s2); // 选择两个最小权重的结点
        HT[i].weight = HT[s1].weight + HT[s2].weight;
        HT[i].lchild = s1;
        HT[i].rchild = s2;
        HT[s1].parent = i;
        HT[s2].parent = i;
    }
}

// 生成赫夫曼编码
void HTCoding(HTree HT, HTCode &HC, int n) {
    HC = new char *[n];
    char *cd = new char[n]; // 临时存放编码
    cd[n - 1] = '\0';

    for (int i = 0; i < n; i++) {
        int start = n - 1;
        int c = i;
        int p = HT[i].parent;

        while (p != -1) {
            if (HT[p].lchild == c)
                cd[--start] = '0';
            else
                cd[--start] = '1';

            c = p;
            p = HT[p].parent;
        }

        HC[i] = new char[n - start];
        strcpy(HC[i], &cd[start]);
    }

    delete[] cd;
}
