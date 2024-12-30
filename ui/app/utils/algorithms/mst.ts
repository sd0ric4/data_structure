// 常量定义
export const INFINITY = 32767;
export const MAX_VERTEX_NUM = 20;

// 图的类型枚举
export enum GraphKind {
  DG, // 有向图
  DN, // 有向网
  UDG, // 无向图
  UDN, // 无向网
}

// 边的结构
export interface Edge {
  start: number;
  end: number;
  weight: number;
}

// 图的数据结构
export interface MGraph {
  vexs: string[]; // 顶点数组
  arcs: number[][]; // 邻接矩阵
  vexnum: number; // 顶点数
  arcnum: number; // 边数
  kind: GraphKind; // 图类型
}

// 动画步骤结构
export interface AnimationStep {
  type: 'select' | 'add' | 'compare';
  edge?: Edge;
  message: string;
  closedge?: number[]; // Prim算法的辅助数组
  cnvx?: number[]; // Kruskal算法的连通分量数组
}

// Prim算法实现
export const primMST = (graph: MGraph): AnimationStep[] => {
  const steps: AnimationStep[] = [];
  const n = graph.vexnum;
  const lowcost = new Array(n).fill(INFINITY);
  const closest = new Array(n).fill(0);
  const used = new Array(n).fill(false);

  used[0] = true;
  for (let i = 1; i < n; i++) {
    lowcost[i] = graph.arcs[0][i];
    closest[i] = 0;
  }

  steps.push({
    type: 'select',
    message: `从顶点 ${graph.vexs[0]} 开始构建最小生成树`,
    closedge: [...lowcost],
  });

  for (let i = 1; i < n; i++) {
    let minCost = INFINITY;
    let minJ = -1;

    for (let j = 0; j < n; j++) {
      if (!used[j] && lowcost[j] < minCost) {
        minCost = lowcost[j];
        minJ = j;
      }
    }

    if (minJ === -1) break;

    used[minJ] = true;
    steps.push({
      type: 'add',
      edge: {
        start: closest[minJ],
        end: minJ,
        weight: lowcost[minJ],
      },
      message: `选择边 (${graph.vexs[closest[minJ]]},${graph.vexs[minJ]},${
        lowcost[minJ]
      })`,
      closedge: [...lowcost],
    });

    for (let j = 0; j < n; j++) {
      if (!used[j] && graph.arcs[minJ][j] < lowcost[j]) {
        lowcost[j] = graph.arcs[minJ][j];
        closest[j] = minJ;
        steps.push({
          type: 'compare',
          message: `更新顶点 ${graph.vexs[j]} 的最小权值为 ${graph.arcs[minJ][j]}`,
          closedge: [...lowcost],
        });
      }
    }
  }

  return steps;
};

// Kruskal算法实现
export const kruskalMST = (graph: MGraph): AnimationStep[] => {
  const steps: AnimationStep[] = [];
  const edges: Edge[] = [];
  const parent = new Array(graph.vexnum).fill(-1);

  for (let i = 0; i < graph.vexnum; i++) {
    for (let j = i + 1; j < graph.vexnum; j++) {
      if (graph.arcs[i][j] !== INFINITY) {
        edges.push({ start: i, end: j, weight: graph.arcs[i][j] });
      }
    }
  }

  edges.sort((a, b) => a.weight - b.weight);
  steps.push({
    type: 'compare',
    message: '对所有边按权值进行排序',
    cnvx: [...parent],
  });

  const find = (x: number): number => {
    if (parent[x] === -1) return x;
    return (parent[x] = find(parent[x]));
  };

  const union = (x: number, y: number): void => {
    const px = find(x);
    const py = find(y);
    if (px !== py) {
      parent[px] = py;
    }
  };

  for (const edge of edges) {
    const x = find(edge.start);
    const y = find(edge.end);

    steps.push({
      type: 'compare',
      edge,
      message: `检查边 (${graph.vexs[edge.start]},${graph.vexs[edge.end]},${
        edge.weight
      })`,
      cnvx: [...parent],
    });

    if (x !== y) {
      union(edge.start, edge.end);
      steps.push({
        type: 'add',
        edge,
        message: `添加边 (${graph.vexs[edge.start]},${graph.vexs[edge.end]},${
          edge.weight
        })`,
        cnvx: [...parent],
      });
    }
  }

  return steps;
};
