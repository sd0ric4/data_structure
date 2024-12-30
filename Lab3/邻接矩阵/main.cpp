/****************************************************************
 * 文件名: graph_matrix.cpp
 * 功能描述: 基于邻接矩阵实现的图结构及其基本操作
 * 主要实现:
 * 1. 图的四种类型：有向图(DG)、有向网(DN)、无向图(UDG)、无向网(UDN)
 * 2. 邻接矩阵创建
 * 3. 图的可视化输出
 * 4. 顶点度的计算
 ****************************************************************/

#include <iostream>
#include <iomanip> // 用于格式化输出
using namespace std;

// 常量定义
#define INFINITY 32767    // 定义无穷大值，用于网中不存在的边
#define MAX_VERTEX_NUM 20 // 最大顶点数量

typedef int Status; // 状态类型定义

/**
 * 图的类型枚举
 * DG: Directed Graph - 有向图
 * DN: Directed Network - 有向网(带权值的有向图)
 * UDG: Undirected Graph - 无向图
 * UDN: Undirected Network - 无向网(带权值的无向图)
 */
typedef enum
{
    DG,  // 有向图
    DN,  // 有向网
    UDG, // 无向图
    UDN  // 无向网
} GraphKind;

typedef int VRType;    // 顶点关系类型，用整型表示权值
typedef char InfoType; // 弧相关信息的指针类型

/**
 * 邻接矩阵中的单元结构
 * adj: 对于无权图，用0或1表示是否相邻；对于带权图，存储权值
 * info: 指向该弧的其他相关信息的指针
 */
typedef struct ArcCell
{
    VRType adj;     // 顶点关系，存储权值
    InfoType *info; // 弧的其他信息指针
} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef char VertexType; // 顶点数据类型

/**
 * 图的邻接矩阵存储结构
 * vexs: 顶点数组
 * arcs: 邻接矩阵
 * vexnum: 顶点数量
 * arcnum: 弧的数量
 * kind: 图的类型
 */
typedef struct
{
    VertexType vexs[MAX_VERTEX_NUM]; // 顶点向量
    AdjMatrix arcs;                  // 邻接矩阵
    int vexnum, arcnum;              // 图的顶点数和弧数
    GraphKind kind;                  // 图的种类
} MGraph;

/**
 * 创建图的函数
 * @param GKind: 图的类型
 * @param G: 图结构引用
 * @param vexnum: 顶点数量
 * @param arcnum: 边的数量
 * @param vexs: 顶点数组
 * @param arcs: 边的信息数组，每三个元素表示一条边：<起点,终点,权值>
 */
void CreateMGraph(GraphKind GKind, MGraph &G, int vexnum, int arcnum, char *vexs, int *arcs)
{
    // 初始化图的基本信息
    G.vexnum = vexnum;
    G.arcnum = arcnum;
    G.kind = GKind;

    // 初始化顶点信息
    for (int i = 0; i < vexnum; i++)
    {
        G.vexs[i] = vexs[i];
    }

    // 初始化邻接矩阵
    // 有向图和无向图初始化为0，有向网和无向网初始化为INFINITY
    for (int i = 0; i < vexnum; i++)
    {
        for (int j = 0; j < vexnum; j++)
        {
            if (GKind == DG || GKind == UDG)
                G.arcs[i][j].adj = 0; // 图初始化为0
            else
                G.arcs[i][j].adj = INFINITY; // 网初始化为INFINITY
            G.arcs[i][j].info = nullptr;     // 初始化弧的附加信息
        }
    }

    // 构建邻接矩阵
    for (int k = 0; k < arcnum; k++)
    {
        int i = arcs[k * 3];     // 获取起点索引
        int j = arcs[k * 3 + 1]; // 获取终点索引
        int w = arcs[k * 3 + 2]; // 获取权值

        G.arcs[i][j].adj = w; // 设置<i,j>边的权值
        // 如果是无向图或无向网，需要对称设置<j,i>的权值
        if (GKind == UDG || GKind == UDN)
        {
            G.arcs[j][i].adj = w;
        }
    }
}

/**
 * 输出图的邻接矩阵
 * @param G: 图结构
 * 输出格式：先输出顶点信息，再输出邻接矩阵
 */
void OutMGraph(MGraph G)
{
    cout << "\n图的邻接矩阵：\n";
    // 输出顶点信息
    cout << "顶点：";
    for (int i = 0; i < G.vexnum; i++)
    {
        cout << G.vexs[i] << " ";
    }
    cout << "\n邻接矩阵：\n";

    // 输出邻接矩阵
    for (int i = 0; i < G.vexnum; i++)
    {
        for (int j = 0; j < G.vexnum; j++)
        {
            if (G.arcs[i][j].adj == INFINITY)
                cout << setw(4) << "∞"; // 输出无穷大符号
            else
                cout << setw(4) << G.arcs[i][j].adj; // 输出权值
        }
        cout << endl;
    }
}

/**
 * 计算图中各顶点的度
 * @param G: 图结构
 * 对于有向图：分别计算入度、出度和总度数
 * 对于无向图：计算每个顶点的度
 */
void VerDegree(MGraph G)
{
    cout << "\n各顶点的度：\n";

    if (G.kind == DG || G.kind == DN)
    {
        // 有向图或有向网的度计算
        for (int i = 0; i < G.vexnum; i++)
        {
            int in = 0, out = 0; // 入度和出度计数器
            for (int j = 0; j < G.vexnum; j++)
            {
                // 计算入度：统计第i列非零（非INFINITY）的个数
                if (G.arcs[j][i].adj != 0 && G.arcs[j][i].adj != INFINITY)
                    in++;
                // 计算出度：统计第i行非零（非INFINITY）的个数
                if (G.arcs[i][j].adj != 0 && G.arcs[i][j].adj != INFINITY)
                    out++;
            }
            cout << "顶点" << G.vexs[i] << "的入度为" << in
                 << "，出度为" << out << "，总度数为" << (in + out) << endl;
        }
    }
    else
    {
        // 无向图或无向网的度计算
        for (int i = 0; i < G.vexnum; i++)
        {
            int degree = 0; // 度计数器
            for (int j = 0; j < G.vexnum; j++)
            {
                // 统计第i行非零（非INFINITY）的个数
                if (G.arcs[i][j].adj != 0 && G.arcs[i][j].adj != INFINITY)
                    degree++;
            }
            cout << "顶点" << G.vexs[i] << "的度为" << degree << endl;
        }
    }
}

/**
 * 主函数
 * 测试四种类型的图的创建、显示和度的计算
 */
int main()
{
    MGraph G;

    // 1. 测试有向图 - 演示顶点间的单向连接关系
    char vexs1[] = {'A', 'B', 'C', 'D'};
    int arcs1[] = {
        0, 1, 1, // A->B 权值1
        1, 0, 1, // B->A 权值1
        1, 2, 1, // B->C 权值1
        2, 0, 1, // C->A 权值1
        2, 1, 1, // C->B 权值1
        3, 1, 1  // D->B 权值1
    };

    cout << "\n=== 创建有向图 ===";
    CreateMGraph(DG, G, 4, 6, vexs1, arcs1);
    OutMGraph(G);
    VerDegree(G);

    // 2. 有向网
    char vexs2[] = {'A', 'B', 'C'};
    int arcs2[] = {
        0, 2, 50,
        1, 2, 45,
        2, 0, 64};
    cout << "\n=== 创建有向网 ===";
    CreateMGraph(DN, G, 3, 3, vexs2, arcs2);
    OutMGraph(G);
    VerDegree(G);

    // 3. 无向网
    char vexs3[] = {'A', 'B', 'C', 'D'};
    int arcs3[] = {
        0, 1, 56, // A-B权值56
        0, 2, 34, // A-C权值34
        0, 3, 78, // A-D权值78
        2, 3, 25  // C-D权值25
    };

    // 创建无向网
    cout << "\n=== 创建无向网 ===";
    CreateMGraph(UDN, G, 4, 4, vexs3, arcs3); // 4个顶点，4条边
    OutMGraph(G);
    VerDegree(G);

    // 4. 无向图
    char vexs4[] = {'A', 'B', 'C', 'D'};
    int arcs4[] = {
        0, 1, 1, // A-B
        0, 2, 1, // A-C
        0, 3, 1, // A-D
        1, 2, 1, // B-C
        2, 3, 1  // C-D
    };
    cout << "\n=== 创建无向图 ===";
    CreateMGraph(UDG, G, 4, 5, vexs4, arcs4); // 注意这里改成了5条边
    OutMGraph(G);
    VerDegree(G);

    return 0;
}