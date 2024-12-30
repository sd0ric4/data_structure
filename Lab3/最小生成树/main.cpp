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
    int begin;
    int end;
    int weight;
} Edge;

// 并查集数据结构
class UnionFind
{
private:
    int *parent;
    int size;

public:
    UnionFind(int n)
    {
        size = n;
        parent = new int[n];
        for (int i = 0; i < n; i++)
        {
            parent[i] = i;
        }
    }

    ~UnionFind()
    {
        delete[] parent;
    }

    // 查找根节点（带路径压缩）
    int find(int x)
    {
        if (parent[x] != x)
        {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    // 合并两个集合
    void unite(int x, int y)
    {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY)
        {
            // 总是让大编号指向小编号
            if (rootX < rootY)
            {
                parent[rootY] = rootX;
            }
            else
            {
                parent[rootX] = rootY;
            }
        }
    }

    // 获取当前连通分量标识数组
    void getCnvx(int *cnvx)
    {
        for (int i = 0; i < size; i++)
        {
            cnvx[i] = find(i);
        }
    }
};
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
            G.arcs[i][j].adj = (GKind == DG || GKind == UDG) ? 0 : INFINITY;
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

    struct
    {
        VertexType adjvex;
        int lowcost;
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
    closedge[k].lowcost = 0;

    cout << "初始closedge数组：";
    for (int i = 0; i < G.vexnum; i++)
    {
        if (closedge[i].lowcost == INFINITY)
            cout << "∞ ";
        else
            cout << closedge[i].lowcost << " ";
    }
    cout << endl;

    // 选择其余顶点
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

        cout << "(" << closedge[j].adjvex << "," << G.vexs[j] << ","
             << closedge[j].lowcost << ")" << endl;

        closedge[j].lowcost = 0;

        // 更新最小边
        for (int w = 0; w < G.vexnum; w++)
        {
            if (G.arcs[j][w].adj < closedge[w].lowcost)
            {
                closedge[w].adjvex = G.vexs[j];
                closedge[w].lowcost = G.arcs[j][w].adj;
            }
        }

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

void MiniSpanTree_Kruskal(MGraph G)
{
    cout << "\nKruskal算法生成最小生成树：\n";
    Edge edges[MAX_EDGE_NUM];
    int edgeCount = 0;
    int *cnvx = new int[G.vexnum];

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

    sort(edges, edges + edgeCount,
         [](Edge a, Edge b)
         { return a.weight < b.weight; });

    cout << "排序后的边：\n";
    for (int i = 0; i < edgeCount; i++)
    {
        cout << "(" << G.vexs[edges[i].begin] << ","
             << G.vexs[edges[i].end] << ","
             << edges[i].weight << ")" << endl;
    }

    UnionFind uf(G.vexnum);

    int selectedEdges = 0;
    for (int i = 0; i < edgeCount && selectedEdges < G.vexnum - 1; i++)
    {
        int begin = edges[i].begin;
        int end = edges[i].end;

        if (uf.find(begin) != uf.find(end))
        {
            uf.unite(begin, end);
            cout << "选中边：(" << G.vexs[begin] << ","
                 << G.vexs[end] << ","
                 << edges[i].weight << ")" << endl;

            // 获取并显示连通分量数组
            uf.getCnvx(cnvx);
            cout << "连通分量数组 cnvx[]: ";
            for (int j = 0; j < G.vexnum; j++)
            {
                cout << cnvx[j] << " ";
            }
            cout << endl;

            selectedEdges++;
        }
    }

    delete[] cnvx;
}
int main()
{
    MGraph G;
    char vexs[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    int arcs[] = {
        0, 1, 10, // A-B,10
        0, 2, 12, // A-C,12
        0, 4, 15, // A-E,15
        1, 2, 7,  // B-C,7
        1, 3, 5,  // B-D,5
        1, 5, 6,  // B-F,6
        2, 4, 12, // C-E,12
        2, 5, 8,  // C-F,8
        3, 5, 6,  // D-F,6
        4, 5, 10  // E-F,10
    };

    CreateMGraph(UDN, G, 6, 10, vexs, arcs);
    OutMGraph(G);
    MiniSpanTree_Prim(G, 'A');
    MiniSpanTree_Kruskal(G);

    return 0;
}