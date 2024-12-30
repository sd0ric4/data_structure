#include <iostream>
#include <stack>
#include <queue>
using namespace std;

const int MAX_VERTEX = 20;
const int INFINITY = 10000;

// 继承原有的图结构定义
// ...（省略已有的结构定义，使用原文件中的定义）
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

// 关键路径相关的新函数
class CriticalPath
{
private:
    ALGraph &G;
    int *ve;       // 事件最早发生时间
    int *vl;       // 事件最迟发生时间
    int *indegree; // 入度数组

public:
    CriticalPath(ALGraph &graph) : G(graph)
    {
        ve = new int[G.vexnum]();
        vl = new int[G.vexnum];
        indegree = new int[G.vexnum]();

        // 初始化vl数组为无穷大
        for (int i = 0; i < G.vexnum; i++)
        {
            vl[i] = INFINITY;
        }
    }

    ~CriticalPath()
    {
        delete[] ve;
        delete[] vl;
        delete[] indegree;
    }

    // 计算入度
    void computeIndegree()
    {
        for (int i = 0; i < G.vexnum; i++)
        {
            ArcNode *p = G.vertices[i].firstarc;
            while (p)
            {
                indegree[p->adjvex]++;
                p = p->nextarc;
            }
        }
    }

    // 拓扑排序，同时计算ve值
    bool topologicalSort(stack<int> &topologicalStack)
    {
        computeIndegree();
        queue<int> zeroInDegreeVertices;

        // 将入度为0的顶点入队
        for (int currentVertex = 0; currentVertex < G.vexnum; currentVertex++)
        {
            if (indegree[currentVertex] == 0)
            {
                zeroInDegreeVertices.push(currentVertex);
            }
        }

        int processedVertexCount = 0;
        while (!zeroInDegreeVertices.empty())
        {
            int currentVertex = zeroInDegreeVertices.front();
            zeroInDegreeVertices.pop();
            topologicalStack.push(currentVertex); // 将顶点入栈，用于后续逆序遍历
            processedVertexCount++;

            // 遍历当前顶点的所有邻接点
            ArcNode *adjacentArc = G.vertices[currentVertex].firstarc;
            while (adjacentArc)
            {
                int adjacentVertex = adjacentArc->adjvex;
                indegree[adjacentVertex]--;
                if (indegree[adjacentVertex] == 0)
                {
                    zeroInDegreeVertices.push(adjacentVertex);
                }
                // 计算最早发生时间
                if (ve[currentVertex] + adjacentArc->weight > ve[adjacentVertex])
                {
                    ve[adjacentVertex] = ve[currentVertex] + adjacentArc->weight;
                }
                adjacentArc = adjacentArc->nextarc;
            }
        }

        return processedVertexCount == G.vexnum;
    }
    bool criticalPath()
    {
        stack<int> topologicalStack;
        if (!topologicalSort(topologicalStack))
        {
            cout << "图中存在环，无法求关键路径！" << endl;
            return false;
        }

        // 初始化最迟发生时间
        int maxEarliestTime = 0;
        for (int vertex = 0; vertex < G.vexnum; vertex++)
        {
            if (ve[vertex] > maxEarliestTime)
                maxEarliestTime = ve[vertex];
        }
        for (int vertex = 0; vertex < G.vexnum; vertex++)
        {
            vl[vertex] = maxEarliestTime;
        }

        // 按拓扑排序的逆序求各顶点的最迟发生时间
        while (!topologicalStack.empty())
        {
            int currentVertex = topologicalStack.top();
            topologicalStack.pop();

            ArcNode *adjacentArc = G.vertices[currentVertex].firstarc;
            while (adjacentArc)
            {
                int adjacentVertex = adjacentArc->adjvex;
                if (vl[adjacentVertex] - adjacentArc->weight < vl[currentVertex])
                {
                    vl[currentVertex] = vl[adjacentVertex] - adjacentArc->weight;
                }
                adjacentArc = adjacentArc->nextarc;
            }
        }

        // 输出关键路径
        cout << "\n关键路径为：" << endl;
        // sourceVertex是起点，targetVertex是终点
        for (int sourceVertex = 0; sourceVertex < G.vexnum; sourceVertex++)
        {
            ArcNode *adjacentArc = G.vertices[sourceVertex].firstarc;
            while (adjacentArc)
            {
                int targetVertex = adjacentArc->adjvex;
                int earliestStartTime = ve[sourceVertex];                     // 活动最早开始时间
                int latestStartTime = vl[targetVertex] - adjacentArc->weight; // 活动最迟开始时间

                if (earliestStartTime == latestStartTime)
                { // 说明该活动是关键活动
                    cout << G.vertices[sourceVertex].data << " -> "
                         << G.vertices[targetVertex].data << " [权值="
                         << adjacentArc->weight << "]" << endl;
                }
                adjacentArc = adjacentArc->nextarc;
            }
        }
        return true;
    }
};

int main()
{
    ALGraph G;

    // 创建AOE网
    char vexs[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
    int arcs[] = {
        0, 1, 8,  // A->B (a0=8)
        0, 2, 6,  // A->C (a1=6)
        0, 4, 7,  // A->E (a2=7)
        1, 3, 3,  // B->D (a3=3)
        2, 3, 10, // C->D (a4=10)
        2, 6, 9,  // C->G (a5=9)
        4, 6, 9,  // E->G (a6=9)
        4, 7, 13, // E->H (a7=13)
        3, 5, 4,  // D->F (a8=4)
        3, 8, 19, // D->I (a9=19)
        6, 7, 2,  // G->H (a11=2)
        6, 8, 8,  // G->I (a10=8)
        7, 8, 6,  // H->I (a12=6)
        5, 9, 14, // F->J (a13=14)
        8, 9, 10  // I->J (a14=10)
    };

    CreateALGraph(DN, G, 10, 15, vexs, arcs);
    cout << "创建的AOE网：" << endl;
    OutALGraph(G);

    CriticalPath cp(G);
    cp.criticalPath();

    return 0;
}