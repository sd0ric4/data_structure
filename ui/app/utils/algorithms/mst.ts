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

  // 初始化第一个顶点
  used[0] = true;
  lowcost[0] = 0; // 将起始顶点的 lowcost 设为 0
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

    // 寻找最小边
    for (let j = 0; j < n; j++) {
      if (!used[j] && lowcost[j] < minCost) {
        minCost = lowcost[j];
        minJ = j;
      }
    }

    if (minJ === -1) break;

    // 将顶点加入生成树
    used[minJ] = true;
    lowcost[minJ] = 0; // 将加入生成树的顶点的 lowcost 设为 0

    steps.push({
      type: 'add',
      edge: {
        start: closest[minJ],
        end: minJ,
        weight: minCost,
      },
      message: `选择边 (${graph.vexs[closest[minJ]]},${
        graph.vexs[minJ]
      },${minCost})`,
      closedge: [...lowcost],
    });

    // 更新其他顶点的最小权值
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
// 创建一个并查集类
class UnionFind {
  private parent: number[];
  private size: number;

  constructor(n: number) {
    this.size = n;
    this.parent = new Array(n).fill(0).map((_, i) => i);
  }

  // 查找根节点（带路径压缩）
  find(x: number): number {
    if (this.parent[x] !== x) {
      this.parent[x] = this.find(this.parent[x]);
    }
    return this.parent[x];
  }

  // 合并两个集合
  unite(x: number, y: number): void {
    const rootX = this.find(x);
    const rootY = this.find(y);
    if (rootX !== rootY) {
      // 总是让大编号指向小编号
      if (rootX < rootY) {
        this.parent[rootY] = rootX;
      } else {
        this.parent[rootX] = rootY;
      }
    }
  }

  // 获取当前连通分量标识数组
  getCnvx(): number[] {
    const cnvx = new Array(this.size);
    for (let i = 0; i < this.size; i++) {
      cnvx[i] = this.find(i);
    }
    return cnvx;
  }
}

export const kruskalMST = (graph: MGraph): AnimationStep[] => {
  const steps: AnimationStep[] = [];
  const edges: Edge[] = [];

  // 收集所有边
  for (let i = 0; i < graph.vexnum; i++) {
    for (let j = i + 1; j < graph.vexnum; j++) {
      if (graph.arcs[i][j] !== INFINITY) {
        edges.push({ start: i, end: j, weight: graph.arcs[i][j] });
      }
    }
  }

  // 按权值排序
  edges.sort((a, b) => a.weight - b.weight);

  // 创建并查集
  const uf = new UnionFind(graph.vexnum);

  steps.push({
    type: 'compare',
    message: '对所有边按权值进行排序',
    cnvx: uf.getCnvx(),
  });

  let selectedEdges = 0;
  for (const edge of edges) {
    const startRoot = uf.find(edge.start);
    const endRoot = uf.find(edge.end);

    steps.push({
      type: 'compare',
      edge,
      message: `检查边 (${graph.vexs[edge.start]},${graph.vexs[edge.end]},${
        edge.weight
      })`,
      cnvx: uf.getCnvx(),
    });

    if (startRoot !== endRoot) {
      uf.unite(edge.start, edge.end);
      steps.push({
        type: 'add',
        edge,
        message: `选中边 (${graph.vexs[edge.start]},${graph.vexs[edge.end]},${
          edge.weight
        })`,
        cnvx: uf.getCnvx(),
      });

      selectedEdges++;
      if (selectedEdges === graph.vexnum - 1) {
        steps.push({
          type: 'compare',
          message: '最小生成树构建完成',
          cnvx: uf.getCnvx(),
        });
        break;
      }
    }
  }

  return steps;
};
