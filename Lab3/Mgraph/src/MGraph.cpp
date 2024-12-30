// MGraph.cpp
#include "MGraph.h"
#include <iostream>
#include <cstring>
using namespace std;

// 创建图的邻接矩阵存储结构
Status CreateMGraph(GraphKind GKind, MGraph &G, int vexnum, int arcnum, char *vexs, int *arcs)
{
    if (vexnum > MAX_VERTEX_NUM)
        return -1;
    G.vexnum = vexnum;
    G.arcnum = arcnum;
    G.kind = GKind;

    // 初始化顶点
    for (int i = 0; i < vexnum; i++)
    {
        G.vexs[i] = vexs[i];
    }

    // 初始化邻接矩阵
    for (int i = 0; i < vexnum; i++)
    {
        for (int j = 0; j < vexnum; j++)
        {
            G.arcs[i][j].adj = 0;
            G.arcs[i][j].info = nullptr;
        }
    }

    // 插入弧
    for (int k = 0; k < arcnum; k++)
    {
        int i = arcs[3 * k];
        int j = arcs[3 * k + 1];
        int w = arcs[3 * k + 2];
        if (GKind == DG || GKind == DN)
        {
            G.arcs[i][j].adj = (GKind == DG || GKind == UDG) ? 1 : w;
        }
        else
        {
            G.arcs[i][j].adj = (GKind == UDG || GKind == UDN) ? 1 : w;
            G.arcs[j][i].adj = G.arcs[i][j].adj;
        }
        if (GKind == DN || GKind == UDN)
        {
            G.arcs[i][j].adj = w;
            if (GKind == UDN)
            {
                G.arcs[j][i].adj = w;
            }
        }
    }

    // 对于无权图，设置权值为1
    if (GKind == DG || GKind == UDG)
    {
        for (int k = 0; k < arcnum; k++)
        {
            int i = arcs[3 * k];
            int j = arcs[3 * k + 1];
            G.arcs[i][j].adj = 1;
            if (GKind == UDG)
            {
                G.arcs[j][i].adj = 1;
            }
        }
    }

    return 1;
}

// 输出图的邻接矩阵
void OutMGraph(MGraph G)
{
    cout << "顶点数组: ";
    for (int i = 0; i < G.vexnum; i++)
    {
        cout << G.vexs[i] << " ";
    }
    cout << endl;

    cout << "邻接矩阵:" << endl;
    for (int i = 0; i < G.vexnum; i++)
    {
        for (int j = 0; j < G.vexnum; j++)
        {
            if (G.arcs[i][j].adj == 0)
                cout << "0 ";
            else if (G.arcs[i][j].adj == 1 && (G.kind == UDG || G.kind == DG)) // 无权图
                cout << "1 ";
            else if (G.arcs[i][j].adj < INFINITY)
                cout << G.arcs[i][j].adj << " ";
            else
                cout << "∞ ";
        }
        cout << endl;
    }
}

// 输出图中各顶点的度
void VerDegree(MGraph G)
{
    cout << "顶点的度:" << endl;
    if (G.kind == DG || G.kind == DN)
    {
        // 有向图：计算入度和出度
        for (int i = 0; i < G.vexnum; i++)
        {
            int in = 0, out = 0;
            for (int j = 0; j < G.vexnum; j++)
            {
                if (G.arcs[j][i].adj > 0 && G.arcs[j][i].adj < INFINITY)
                    in++;
                if (G.arcs[i][j].adj > 0 && G.arcs[i][j].adj < INFINITY)
                    out++;
            }
            cout << G.vexs[i] << ": in-degree = " << in << ", out-degree = " << out << endl;
        }
    }
    else
    {
        // 无向图：度为相邻边数
        for (int i = 0; i < G.vexnum; i++)
        {
            int degree = 0;
            for (int j = 0; j < G.vexnum; j++)
            {
                if (G.arcs[i][j].adj > 0 && G.arcs[i][j].adj < INFINITY)
                    degree++;
            }
            cout << G.vexs[i] << ": degree = " << degree << endl;
        }
    }
}
