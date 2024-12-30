#include <iostream>
#include <iomanip>
#include <queue> // 用于广度优先遍历
using namespace std;

#define MAX_VERTEX_NUM 20
typedef int Status;
typedef enum
{
    DG,
    DN,
    UDG,
    UDN
} GraphKind;
typedef char VertexType;

// 边(弧)结点
typedef struct ArcNode
{
    int adjvex;              // 邻接点在顶点数组中的下标
    struct ArcNode *nextarc; // 指向下一条边的指针
    int weight;              // 边的权值
} ArcNode;

// 顶点结点
typedef struct VNode
{
    VertexType data;   // 顶点数据
    ArcNode *firstarc; // 指向第一条依附该顶点的边的指针
} VNode, AdjList[MAX_VERTEX_NUM];

// 图的邻接表结构
typedef struct
{
    AdjList vertices;   // 顶点数组
    int vexnum, arcnum; // 顶点数和边数
    GraphKind kind;     // 图的类型
} ALGraph;

// 创建图的邻接表
void CreateALGraph(GraphKind GKind, ALGraph &G, int vexnum, int arcnum, char *vexs, int *arcs)
{
    G.vexnum = vexnum;
    G.arcnum = arcnum;
    G.kind = GKind;

    // 初始化顶点数组
    for (int i = 0; i < vexnum; i++)
    {
        G.vertices[i].data = vexs[i];
        G.vertices[i].firstarc = nullptr;
    }

    // 创建边结点
    for (int k = 0; k < arcnum; k++)
    {
        int i = arcs[k * 3];     // 起点下标
        int j = arcs[k * 3 + 1]; // 终点下标
        int w = arcs[k * 3 + 2]; // 权值

        // 创建边结点i->j
        ArcNode *p = new ArcNode;
        p->adjvex = j;
        p->weight = w;
        p->nextarc = G.vertices[i].firstarc;
        G.vertices[i].firstarc = p;

        // 如果是无向图/网，需要增加边结点j->i
        if (GKind == UDG || GKind == UDN)
        {
            ArcNode *q = new ArcNode;
            q->adjvex = i;
            q->weight = w;
            q->nextarc = G.vertices[j].firstarc;
            G.vertices[j].firstarc = q;
        }
    }
}

// 输出图的邻接表
// 修改输出图的邻接表函数
void OutALGraph(ALGraph G)
{
    cout << "\n图的邻接表：" << endl;
    for (int i = 0; i < G.vexnum; i++)
    {
        cout << G.vertices[i].data << ": "; // 打印顶点
        ArcNode *p = G.vertices[i].firstarc;
        while (p)
        {
            if (G.kind == DN || G.kind == UDN)
            {
                cout << G.vertices[p->adjvex].data << "(" << p->weight << ")";
            }
            else
            {
                cout << G.vertices[p->adjvex].data;
            }

            if (p->nextarc)
            {
                cout << ", "; // 如果不是最后一个邻接点，加逗号分隔
            }
            p = p->nextarc;
        }
        cout << endl; // 每个顶点的邻接表占一行
    }
}
// 计算顶点的度
void VerDegree(ALGraph G)
{
    cout << "\n各顶点的度：" << endl;

    if (G.kind == DG || G.kind == DN)
    {
        // 计算有向图的入度和出度
        int *inDegree = new int[G.vexnum]();

        // 计算各顶点的入度
        for (int i = 0; i < G.vexnum; i++)
        {
            ArcNode *p = G.vertices[i].firstarc;
            while (p)
            {
                inDegree[p->adjvex]++;
                p = p->nextarc;
            }
        }

        // 统计并输出入度和出度
        for (int i = 0; i < G.vexnum; i++)
        {
            int outDegree = 0;
            ArcNode *p = G.vertices[i].firstarc;
            while (p)
            {
                outDegree++;
                p = p->nextarc;
            }
            cout << "顶点" << G.vertices[i].data
                 << "的入度为" << inDegree[i]
                 << "，出度为" << outDegree
                 << "，总度数为" << (inDegree[i] + outDegree) << endl;
        }
        delete[] inDegree;
    }
    else
    {
        // 计算无向图的度
        for (int i = 0; i < G.vexnum; i++)
        {
            int degree = 0;
            ArcNode *p = G.vertices[i].firstarc;
            while (p)
            {
                degree++;
                p = p->nextarc;
            }
            cout << "顶点" << G.vertices[i].data << "的度为" << degree << endl;
        }
    }
}
// 深度优先遍历的递归函数
void DFS(ALGraph G, int v, bool visited[])
{
    // 访问当前顶点
    cout << G.vertices[v].data << " ";
    visited[v] = true;

    // 遍历当前顶点的所有邻接点
    ArcNode *p = G.vertices[v].firstarc;
    while (p)
    {
        // 如果邻接点未被访问，则递归访问它
        if (!visited[p->adjvex])
        {
            DFS(G, p->adjvex, visited);
        }
        p = p->nextarc;
    }
}

// 深度优先遍历的主函数
void DFSTraverse(ALGraph G)
{
    cout << "\n深度优先遍历序列：";
    // 初始化访问标记数组
    bool *visited = new bool[G.vexnum]();

    // 从每个未被访问的顶点开始遍历
    for (int i = 0; i < G.vexnum; i++)
    {
        if (!visited[i])
        {
            DFS(G, i, visited);
        }
    }
    cout << endl;
    delete[] visited;
}

// 广度优先遍历
void BFSTraverse(ALGraph G)
{
    cout << "\n广度优先遍历序列：";
    // 初始化访问标记数组
    bool *visited = new bool[G.vexnum]();
    queue<int> Q; // 用于存储待访问的顶点

    // 从每个未被访问的顶点开始遍历
    for (int i = 0; i < G.vexnum; i++)
    {
        if (!visited[i])
        {
            // 访问当前顶点
            cout << G.vertices[i].data << " ";
            visited[i] = true;
            Q.push(i);

            // 访问当前顶点的所有邻接点
            while (!Q.empty())
            {
                int v = Q.front();
                Q.pop();
                ArcNode *p = G.vertices[v].firstarc;
                while (p)
                {
                    if (!visited[p->adjvex])
                    {
                        cout << G.vertices[p->adjvex].data << " ";
                        visited[p->adjvex] = true;
                        Q.push(p->adjvex);
                    }
                    p = p->nextarc;
                }
            }
        }
    }
    cout << endl;
    delete[] visited;
}
int main()
{
    ALGraph G;

    // 创建图的顶点和边
    char vexs[] = {'0', '1', '2', '3', '4', '5'}; // 6个顶点
    int arcs[] = {
        0, 1, 1, // 0-1
        0, 2, 1, // 0-2
        1, 3, 1, // 1-3
        1, 4, 1, // 1-4
        2, 5, 1, // 2-5
        3, 4, 1, // 3-4
        4, 5, 1  // 4-5
    };

    // 创建无向图
    cout << "=== 创建无向图 ===\n";
    CreateALGraph(UDG, G, 6, 7, vexs, arcs); // 6个顶点，7条边

    // 输出邻接表
    OutALGraph(G);

    // 深度优先遍历
    DFSTraverse(G);

    // 广度优先遍历
    BFSTraverse(G);

    return 0;
}