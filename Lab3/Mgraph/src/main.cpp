// main.cpp
#include "MGraph.h"
#include <iostream>
using namespace std;

int main()
{
    // 定义四种图类型
    GraphKind kinds[4] = {DG, DN, UDG, UDN};
    const char *kind_names[4] = {"有向图", "有向网", "无向图", "无向网"};

    // 示例数据：顶点 A, B, C, D
    int vexnum = 4;
    int arcnum = 4;
    char vexs[] = {'A', 'B', 'C', 'D'};

    // 定义弧数组，每条弧采用三元组 (i, j, w)
    // 示例边：A->B, A->C, B->D, C->D
    int arcs_DG[] = {0, 1, 1, 0, 2, 1, 1, 3, 1, 2, 3, 1};  // 无权图，权值设为1
    int arcs_DN[] = {0, 1, 5, 0, 2, 3, 1, 3, 2, 2, 3, 4};  // 有权网
    int arcs_UDG[] = {0, 1, 1, 0, 2, 1, 1, 3, 1, 2, 3, 1}; // 无向无权图
    int arcs_UDN[] = {0, 1, 5, 0, 2, 3, 1, 3, 2, 2, 3, 4}; // 无向有权网

    // 创建和处理每种图
    for (int k = 0; k < 4; k++)
    {
        MGraph G;
        GraphKind currentKind = kinds[k];
        int currentArcnum = arcnum;
        char *currentVexs = const_cast<char *>(vexs); // 转换为非const指针
        int *currentArcs;
        switch (k)
        {
        case 0:
            currentArcs = arcs_DG;
            break;
        case 1:
            currentArcs = arcs_DN;
            break;
        case 2:
            currentArcs = arcs_UDG;
            break;
        case 3:
            currentArcs = arcs_UDN;
            break;
        }

        cout << "创建 " << kind_names[k] << " ..." << endl;
        CreateMGraph(currentKind, G, vexnum, arcnum, currentVexs, currentArcs);
        cout << "图创建完成。" << endl;

        cout << "输出图的信息:" << endl;
        OutMGraph(G);

        cout << "输出各顶点的度:" << endl;
        VerDegree(G);
        cout << "-----------------------------" << endl;
    }

    return 0;
}
