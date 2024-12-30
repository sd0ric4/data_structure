#include <iostream>
#include <iomanip>
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

int main()
{
    ALGraph G;

    // 1. 测试有向图
    char vexs1[] = {'A', 'B', 'C', 'D'};
    int arcs1[] = {
        0, 1, 1, // A->B
        1, 0, 1, // B->A
        1, 2, 1, // B->C
        2, 0, 1, // C->A
        2, 1, 1, // C->B
        3, 1, 1  // D->B
    };
    cout << "\n=== 创建有向图 ===";
    CreateALGraph(DG, G, 4, 6, vexs1, arcs1);
    OutALGraph(G);
    VerDegree(G);

    // 2. 测试有向网
    char vexs2[] = {'A', 'B', 'C'};
    int arcs2[] = {
        0, 2, 50,
        1, 2, 45,
        2, 0, 64};
    cout << "\n=== 创建有向网 ===";
    CreateALGraph(DN, G, 3, 3, vexs2, arcs2);
    OutALGraph(G);
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
    CreateALGraph(UDN, G, 4, 4, vexs3, arcs3); // 4个顶点，4条边
    OutALGraph(G);
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
    CreateALGraph(UDG, G, 4, 5, vexs4, arcs4); // 注意这里改成了5条边
    OutALGraph(G);
    VerDegree(G);

    return 0;
}