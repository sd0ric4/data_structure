#include <iostream>
#include <vector>
#include <tabulate/table.hpp>
#include "huffman_tree.h"

using namespace std;
using namespace tabulate;

int main()
{
    HTree HT;                               // 赫夫曼树
    HTCode HC;                              // 赫夫曼编码
    int w[] = {5, 29, 7, 8, 14, 23, 3, 11}; // 权重数组
    int n = sizeof(w) / sizeof(w[0]);       // 叶子结点数

    // (1) 初始化赫夫曼树
    InitHTree(HT, w, n);
    cout << "初始化的赫夫曼树：" << endl;
    Table init_huffman_tree_table;
    init_huffman_tree_table.add_row({"node", "weight", "parent", "lchild", "rchild"});
    for (int i = 0; i < n; i++)
    {
        init_huffman_tree_table.add_row({to_string(i), to_string(HT[i].weight), to_string(HT[i].parent), to_string(HT[i].lchild), to_string(HT[i].rchild)});
    }
    init_huffman_tree_table[0].format().font_color(Color::yellow); // 表头颜色
    for (int i = 1; i <= n; i++)
    {
        init_huffman_tree_table[i].format().font_color(Color::cyan); // 内容颜色
    }
    cout << init_huffman_tree_table << endl;

    // (2) 构造赫夫曼树
    CreateHTree(HT, n);
    cout << "\n构造后的赫夫曼树：" << endl;
    Table huffman_tree_table;
    huffman_tree_table.add_row({"node", "weight", "parent", "lchild", "rchild"});
    for (int i = 0; i < 2 * n - 1; i++)
    {
        huffman_tree_table.add_row({to_string(i), to_string(HT[i].weight), to_string(HT[i].parent), to_string(HT[i].lchild), to_string(HT[i].rchild)});
    }
    huffman_tree_table[0].format().font_color(Color::yellow); // 表头颜色
    for (int i = 1; i < 2 * n; i++)
    {
        huffman_tree_table[i].format().font_color(Color::cyan); // 内容颜色
    }
    cout << huffman_tree_table << endl;

    // (3) 生成赫夫曼编码
    HTCoding(HT, HC, n);
    cout << "\n赫夫曼编码：" << endl;
    Table huffman_code_table;
    huffman_code_table.add_row({"node", "weight", "code"});
    for (int i = 0; i < n; i++)
    {
        huffman_code_table.add_row({to_string(i), to_string(w[i]), HC[i]});
    }
    huffman_code_table[0].format().font_color(Color::yellow); // 表头颜色
    for (int i = 1; i <= n; i++)
    {
        huffman_code_table[i].format().font_color(Color::cyan); // 内容颜色
    }
    cout << huffman_code_table << endl;

    // 释放内存
    for (int i = 0; i < n; i++)
    {
        delete[] HC[i];
    }
    delete[] HC;
    delete[] HT;

    return 0;
}