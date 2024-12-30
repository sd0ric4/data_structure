#include "huffman_tree.h"

/**
 * 初始化赫夫曼树
 * @param HT 赫夫曼树
 * @param w 权重数组
 * @param n 叶子节点数
 *
 * 算法实现:
 * 1. 计算赫夫曼树的总节点数。
 * 2. 初始化前n个节点为叶子节点，设置权重和父子关系。
 * 3. 初始化剩余节点为非叶子节点，设置权重和父子关系。
 */
void InitHTree(HTree &HT, int *w, int n)
{
    int m = 2 * n - 1; // 总结点数
    HT = new HTNode[m];

    for (int i = 0; i < n; i++)
    {
        HT[i].weight = w[i];
        HT[i].parent = -1;
        HT[i].lchild = -1;
        HT[i].rchild = -1;
    }
    for (int i = n; i < m; i++)
    {
        HT[i].weight = -1;
        HT[i].parent = -1;
        HT[i].lchild = -1;
        HT[i].rchild = -1;
    }
}

/**
 * 从赫夫曼树中选择两个最小权重且父节点为空的结点
 * @param HT 赫夫曼树
 * @param n 当前节点数
 * @param s1 最小权重节点索引
 * @param s2 次小权重节点索引
 *
 * 算法实现:
 * 1. 初始化s1和s2为-1。
 * 2. 遍历所有节点，找到两个最小权重且父节点为空的节点。
 */
void Select(HTree HT, int n, int &s1, int &s2)
{
    s1 = s2 = -1;
    for (int i = 0; i < n; i++)
    {
        // 在循环中，检查每个节点的父节点是否为空，如果为空，则检查权重是否小于s1和s2
        if (HT[i].parent == -1)
        {
            // 如果s1为空或者当前节点的权重小于s1，则将s1赋值为当前节点
            if (s1 == -1 || HT[i].weight < HT[s1].weight)
            {
                s2 = s1;
                s1 = i;
            }
            // 如果s2为空或者当前节点的权重小于s2，则将s2赋值为当前节点
            else if (s2 == -1 || HT[i].weight < HT[s2].weight)
            {
                s2 = i;
            }
        }
    }
}

/**
 * 构造赫夫曼树
 * @param HT 赫夫曼树
 * @param n 叶子节点数
 *
 * 算法实现:
 * 1. 计算赫夫曼树的总节点数。
 * 2. 依次选择两个最小权重的节点，构造新的父节点。
 * 3. 更新父子关系和权重。
 */
void CreateHTree(HTree &HT, int n)
{
    int m = 2 * n - 1; // 总结点数
    for (int i = n; i < m; i++)
    {
        int s1, s2;
        Select(HT, i, s1, s2); // 选择两个最小权重的结点
        HT[i].weight = HT[s1].weight + HT[s2].weight;
        HT[i].lchild = s1;
        HT[i].rchild = s2;
        HT[s1].parent = i;
        HT[s2].parent = i;
    }
}

/**
 * 生成赫夫曼编码
 * @param HT 赫夫曼树
 * @param HC 赫夫曼编码
 * @param n 叶子节点数
 *
 * 算法实现:
 * 1. 初始化编码数组和临时存放编码的数组。
 * 2. 对每个叶子节点，逆向追踪其父节点，生成编码。
 * 3. 将生成的编码存入编码数组。
 * 4. 释放临时存放编码的数组。
 */
void HTCoding(HTree HT, HTCode &HC, int n)
{
    HC = new char *[n];
    char *cd = new char[n]; // 临时存放编码
    cd[n - 1] = '\0';

    for (int i = 0; i < n; i++)
    {
        int start = n - 1;
        int c = i;
        int p = HT[i].parent;

        // 逆向追踪父节点，生成编码
        while (p != -1)
        {
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
