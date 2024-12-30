// MGraph.h
#ifndef MGGRAPH_H
#define MGGRAPH_H

#define INFINITY 32767    // 表示最大值∞
#define MAX_VERTEX_NUM 20 // 最大顶点数

typedef int Status;

typedef enum
{
    DG,
    DN,
    UDG,
    UDN
} GraphKind; // 图的枚举类型

typedef int VRType;

typedef char InfoType;

typedef struct ArcCell
{
    VRType adj;     // 对无权图用1或0表示；对带权图，则为权值类型
    InfoType *info; // 该边或弧相关信息的指针（字符串）
} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef char VertexType;

typedef struct
{
    VertexType vexs[MAX_VERTEX_NUM]; // 顶点数组，存放顶点信息
    AdjMatrix arcs;                  // 邻接矩阵
    int vexnum, arcnum;              // 图中顶点总数与弧数
    GraphKind kind;                  // 图的种类标志
} MGraph;

// 函数声明
Status CreateMGraph(GraphKind GKind, MGraph &G, int vexnum, int arcnum, char *vexs, int *arcs);
void OutMGraph(MGraph G);
void VerDegree(MGraph G);

#endif // MGGRAPH_H
