#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

#define INFINITY 32767
#define MAX_VERTEX_NUM 20
#define MAX_EDGE_NUM 20

typedef int Status;
typedef enum
{
    DG,
    DN,
    UDG,
    UDN
} GraphKind;
typedef int VRType;
typedef char InfoType;

// 邻接矩阵中的单元结构
typedef struct ArcCell
{
    VRType adj;
    InfoType *info;
} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

// 图的邻接矩阵结构
typedef char VertexType;
typedef struct
{
    VertexType vexs[MAX_VERTEX_NUM];
    AdjMatrix arcs;
    int vexnum, arcnum;
    GraphKind kind;
} MGraph;

// 边的结构，用于Kruskal算法
typedef struct
{
    int begin;  // 起点下标
    int end;    // 终点下标
    int weight; // 权重
} Edge;

// 创建图
void CreateMGraph(GraphKind GKind, MGraph &G, int vexnum, int arcnum, char *vexs, int *arcs)
{
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
            if (GKind == DG || GKind == UDG)
                G.arcs[i][j].adj = 0;
            else
                G.arcs[i][j].adj = INFINITY;
            G.arcs[i][j].info = nullptr;
        }
    }

    // 构建邻接矩阵
    for (int k = 0; k < arcnum; k++)
    {
        int i = arcs[k * 3];
        int j = arcs[k * 3 + 1];
        int w = arcs[k * 3 + 2];
        G.arcs[i][j].adj = w;
        if (GKind == UDG || GKind == UDN)
        {
            G.arcs[j][i].adj = w;
        }
    }
}

// 输出图的邻接矩阵
void OutMGraph(MGraph G)
{
    cout << "\n邻接矩阵：\n";
    for (int i = 0; i < G.vexnum; i++)
    {
        for (int j = 0; j < G.vexnum; j++)
        {
            if (G.arcs[i][j].adj == INFINITY)
                cout << setw(4) << "∞";
            else
                cout << setw(4) << G.arcs[i][j].adj;
        }
        cout << endl;
    }
}

// Prim算法实现最小生成树
void MiniSpanTree_Prim(MGraph G, VertexType u)
{
    cout << "\nPrim算法生成最小生成树：\n";
    int k = 0;
    // 查找起始顶点的下标
    for (k = 0; k < G.vexnum; k++)
    {
        if (G.vexs[k] == u)
            break;
    }

    // 辅助数组，记录当前生成树到其余顶点的最小边
    struct
    {
        VertexType adjvex; // 最小边在生成树上的顶点
        int lowcost;       // 最小边上的权值
    } closedge[MAX_VERTEX_NUM];

    // 初始化辅助数组
    for (int i = 0; i < G.vexnum; i++)
    {
        if (i != k)
        {
            closedge[i].adjvex = u;
            closedge[i].lowcost = G.arcs[k][i].adj;
        }
    }
    closedge[k].lowcost = 0; // 起始顶点放入树中

    // 输出数组初始状态
    cout << "初始closedge数组：";
    for (int i = 0; i < G.vexnum; i++)
    {
        if (closedge[i].lowcost == INFINITY)
            cout << "∞ ";
        else
            cout << closedge[i].lowcost << " ";
    }
    cout << endl;

    // 选择其余n-1个顶点
    for (int i = 1; i < G.vexnum; i++)
    {
        int min = INFINITY;
        int j = 0;

        // 寻找最小边
        for (int w = 0; w < G.vexnum; w++)
        {
            if (closedge[w].lowcost != 0 && closedge[w].lowcost < min)
            {
                min = closedge[w].lowcost;
                j = w;
            }
        }

        // 输出找到的最小生成边
        cout << "(" << closedge[j].adjvex << "," << G.vexs[j] << "," << closedge[j].lowcost << ")" << endl;

        closedge[j].lowcost = 0; // 标记j号顶点已经加入生成树

        // 更新其他顶点的最小边
        for (int w = 0; w < G.vexnum; w++)
        {
            if (G.arcs[j][w].adj < closedge[w].lowcost)
            {
                closedge[w].adjvex = G.vexs[j];
                closedge[w].lowcost = G.arcs[j][w].adj;
            }
        }

        // 输出更新后的数组状态
        cout << "更新后的closedge数组：";
        for (int w = 0; w < G.vexnum; w++)
        {
            if (closedge[w].lowcost == INFINITY)
                cout << "∞ ";
            else
                cout << closedge[w].lowcost << " ";
        }
        cout << endl;
    }
}

// 查找连通分量的根节点
int Find(int *parent, int f)
{
    while (parent[f] > 0)
    {
        f = parent[f];
    }
    return f;
}

// Kruskal算法实现最小生成树
void MiniSpanTree_Kruskal(MGraph G)
{
    cout << "\nKruskal算法生成最小生成树：\n";
    Edge edges[MAX_EDGE_NUM];
    int edgeCount = 0;

    // 收集所有边
    for (int i = 0; i < G.vexnum; i++)
    {
        for (int j = i + 1; j < G.vexnum; j++)
        {
            if (G.arcs[i][j].adj != INFINITY)
            {
                edges[edgeCount].begin = i;
                edges[edgeCount].end = j;
                edges[edgeCount].weight = G.arcs[i][j].adj;
                edgeCount++;
            }
        }
    }

    // 对边按权重排序
    sort(edges, edges + edgeCount,
         [](Edge a, Edge b)
         { return a.weight < b.weight; });

    // 输出排序后的边
    cout << "排序后的边：\n";
    for (int i = 0; i < edgeCount; i++)
    {
        cout << "(" << G.vexs[edges[i].begin] << ","
             << G.vexs[edges[i].end] << ","
             << edges[i].weight << ")" << endl;
    }

    int *parent = new int[G.vexnum](); // 连通分量数组
    for (int i = 0; i < G.vexnum; i++)
    {
        parent[i] = 0;
    }

    // 从小到大选择边
    int num = 0; // 当前选中的边数
    int i = 0;   // 边的指针
    while (num < G.vexnum - 1)
    {
        int n = Find(parent, edges[i].begin);
        int m = Find(parent, edges[i].end);
        if (n != m)
        {                  // 不会形成回路
            parent[n] = m; // 合并连通分量
            cout << "选中边：(" << G.vexs[edges[i].begin] << ","
                 << G.vexs[edges[i].end] << ","
                 << edges[i].weight << ")" << endl;

            // 输出连通分量数组
            cout << "连通分量数组：";
            for (int j = 0; j < G.vexnum; j++)
            {
                cout << parent[j] << " ";
            }
            cout << endl;

            num++;
        }
        i++;
    }

    delete[] parent;
}

int main()
{
    MGraph G;

    // 创建图的顶点和边
    char vexs[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    int arcs[] = {
        0, 1, 6, // A-B,6
        0, 2, 1, // A-C,1
        0, 3, 5, // A-D,5
        1, 2, 5, // B-C,5
        1, 4, 3, // B-E,3
        2, 3, 5, // C-D,5
        2, 4, 6, // C-E,6
        2, 5, 4, // C-F,4
        3, 5, 2, // D-F,2
        4, 5, 6  // E-F,6
    };

    // 创建无向网
    CreateMGraph(UDN, G, 6, 10, vexs, arcs);

    // 输出邻接矩阵
    OutMGraph(G);

    // 使用Prim算法求最小生成树
    MiniSpanTree_Prim(G, 'A');

    // 使用Kruskal算法求最小生成树
    MiniSpanTree_Kruskal(G);

    return 0;
}