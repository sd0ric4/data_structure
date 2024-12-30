#include <iostream>
#include <queue>
#include <vector>
using namespace std;

const int MAX_VERTEX = 20;
const int INFINITY = 10000;

// 图的类型枚举
typedef enum
{
    DG,
    DN,
    UDG,
    UDN
} GraphKind;

// 边节点结构
struct ArcNode
{
    int targetVertex; // 目标顶点的索引
    ArcNode *next;    // 下一条边
    int duration;     // 活动持续时间（权值）
};

// 顶点节点结构
struct Vertex
{
    char name;         // 顶点名称
    ArcNode *firstArc; // 第一条边

    // 前向传播相关
    int earliestStart; // 最早开始时间
    int inDegree;      // 入度（用于前向传播）

    // 反向传播相关
    int latestStart; // 最迟开始时间
};

// 图结构
class ActivityNetwork
{
private:
    vector<Vertex> vertices;
    int vertexCount, arcCount;
    GraphKind kind;

public:
    // 构造函数
    ActivityNetwork(int vCount) : vertexCount(vCount)
    {
        vertices.resize(vCount);
        for (auto &vertex : vertices)
        {
            vertex.firstArc = nullptr;
            vertex.earliestStart = 0;
            vertex.latestStart = INFINITY;
            vertex.inDegree = 0;
        }
    }

    // 创建活动网络
    void createNetwork(char *names, int *arcs, int arcCount)
    {
        // 初始化顶点
        for (int i = 0; i < vertexCount; i++)
        {
            vertices[i].name = names[i];
        }

        // 创建边并计算入度
        for (int i = 0; i < arcCount; i++)
        {
            int from = arcs[i * 3];
            int to = arcs[i * 3 + 1];
            int duration = arcs[i * 3 + 2];

            // 创建新边
            ArcNode *newArc = new ArcNode{to, vertices[from].firstArc, duration};
            vertices[from].firstArc = newArc;

            // 更新入度
            vertices[to].inDegree++;
        }
    }

    // 前向传播 - 计算最早开始时间
    void forwardPropagation()
    {
        cout << "\n开始前向传播 - 计算最早开始时间：" << endl;
        queue<int> zeroInDegree;
        vector<int> inDegree = getInDegrees();

        // 找出所有入度为0的顶点
        for (int i = 0; i < vertexCount; i++)
        {
            if (inDegree[i] == 0)
            {
                zeroInDegree.push(i);
            }
        }

        // 前向传播过程
        while (!zeroInDegree.empty())
        {
            int current = zeroInDegree.front();
            zeroInDegree.pop();

            cout << "处理顶点 " << vertices[current].name
                 << " 最早开始时间: " << vertices[current].earliestStart << endl;

            // 更新所有后继顶点
            for (ArcNode *arc = vertices[current].firstArc; arc; arc = arc->next)
            {
                int next = arc->targetVertex;
                // 更新最早开始时间（类似于神经网络中的前向传播）
                vertices[next].earliestStart = max(
                    vertices[next].earliestStart,
                    vertices[current].earliestStart + arc->duration);

                // 减少入度，当入度为0时加入队列
                if (--inDegree[next] == 0)
                {
                    zeroInDegree.push(next);
                }
            }
        }
    }

    // 反向传播 - 计算最迟开始时间
    void backwardPropagation()
    {
        cout << "\n开始反向传播 - 计算最迟开始时间：" << endl;

        // 找到最大的最早开始时间（项目总工期）
        int maxEarliestStart = 0;
        for (const auto &vertex : vertices)
        {
            maxEarliestStart = max(maxEarliestStart, vertex.earliestStart);
        }

        // 初始化所有顶点的最迟开始时间
        for (auto &vertex : vertices)
        {
            vertex.latestStart = maxEarliestStart;
        }

        // 反向遍历所有顶点（从后向前）
        for (int i = vertexCount - 1; i >= 0; i--)
        {
            cout << "处理顶点 " << vertices[i].name
                 << " 最迟开始时间: " << vertices[i].latestStart << endl;

            // 更新所有前驱顶点的最迟开始时间
            for (int j = 0; j < vertexCount; j++)
            {
                for (ArcNode *arc = vertices[j].firstArc; arc; arc = arc->next)
                {
                    if (arc->targetVertex == i)
                    {
                        // 反向更新最迟开始时间（类似于神经网络中的反向传播）
                        vertices[j].latestStart = min(
                            vertices[j].latestStart,
                            vertices[i].latestStart - arc->duration);
                    }
                }
            }
        }
    }

    // 输出关键路径
    void findCriticalPath()
    {
        cout << "\n关键路径（关键活动）：" << endl;
        for (int i = 0; i < vertexCount; i++)
        {
            for (ArcNode *arc = vertices[i].firstArc; arc; arc = arc->next)
            {
                // 如果活动的最早开始时间等于最迟开始时间，则为关键活动
                if (vertices[i].earliestStart ==
                    vertices[arc->targetVertex].latestStart - arc->duration)
                {
                    cout << vertices[i].name << " -> "
                         << vertices[arc->targetVertex].name
                         << " (持续时间: " << arc->duration << ")" << endl;
                }
            }
        }
    }

private:
    // 获取所有顶点的入度
    vector<int> getInDegrees()
    {
        vector<int> inDegree(vertexCount, 0);
        for (int i = 0; i < vertexCount; i++)
        {
            for (ArcNode *arc = vertices[i].firstArc; arc; arc = arc->next)
            {
                inDegree[arc->targetVertex]++;
            }
        }
        return inDegree;
    }
};

int main()
{
    // 创建示例网络
    char vexs[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
    int arcs[] = {
        0, 1, 8,  // A->B (8天)
        0, 2, 6,  // A->C (6天)
        0, 4, 7,  // A->E (7天)
        1, 3, 3,  // B->D (3天)
        2, 3, 10, // C->D (10天)
        2, 6, 9,  // C->G (9天)
        4, 6, 9,  // E->G (9天)
        4, 7, 13, // E->H (13天)
        3, 5, 4,  // D->F (4天)
        3, 8, 19, // D->I (19天)
        6, 7, 2,  // G->H (2天)
        6, 8, 8,  // G->I (8天)
        7, 8, 6,  // H->I (6天)
        5, 9, 14, // F->J (14天)
        8, 9, 10  // I->J (10天)
    };

    ActivityNetwork network(10);
    network.createNetwork(vexs, arcs, 15);

    // 执行前向传播和反向传播
    network.forwardPropagation();
    network.backwardPropagation();
    network.findCriticalPath();

    return 0;
}