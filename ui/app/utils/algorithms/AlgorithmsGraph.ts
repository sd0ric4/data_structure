export enum GraphKind {
  DG, // 有向图
  DN, // 有向网
  UDG, // 无向图
  UDN, // 无向网
}

export interface ArcNode {
  adjvex: number; // 邻接点的位置
  nextarc: ArcNode | null; // 指向下一个邻接点的指针
  weight: number; // 边的权重
}

export interface VNode {
  data: string; // 顶点的数据
  firstarc: ArcNode | null; // 指向第一个邻接点的指针
}

export interface ALGraph {
  vertices: VNode[]; // 顶点数组
  vexnum: number; // 顶点数
  arcnum: number; // 边数
  kind: GraphKind; // 图的类型
}

export interface AnimationStep {
  type: 'visit' | 'edge'; // 动画步骤类型
  from?: number; // 边的起点
  to?: number; // 边的终点
  node?: number; // 访问的节点
}

export const DFSTraverse = (G: ALGraph): AnimationStep[] => {
  const visited = new Array(G.vexnum).fill(false);
  const steps: AnimationStep[] = [];

  const DFS = (v: number) => {
    visited[v] = true;
    steps.push({ type: 'visit', node: v });

    let p = G.vertices[v].firstarc;
    while (p) {
      if (!visited[p.adjvex]) {
        steps.push({ type: 'edge', from: v, to: p.adjvex });
        DFS(p.adjvex);
      }
      p = p.nextarc;
    }
  };

  for (let v = 0; v < G.vexnum; v++) {
    if (!visited[v]) {
      DFS(v);
    }
  }

  return steps;
};

export const BFSTraverse = (G: ALGraph): AnimationStep[] => {
  const visited = new Array(G.vexnum).fill(false);
  const steps: AnimationStep[] = [];
  const queue: number[] = [];

  for (let v = 0; v < G.vexnum; v++) {
    if (!visited[v]) {
      visited[v] = true;
      steps.push({ type: 'visit', node: v });
      queue.push(v);

      while (queue.length > 0) {
        const u = queue.shift()!;
        let p = G.vertices[u].firstarc;

        while (p) {
          if (!visited[p.adjvex]) {
            visited[p.adjvex] = true;
            steps.push({ type: 'edge', from: u, to: p.adjvex });
            steps.push({ type: 'visit', node: p.adjvex });
            queue.push(p.adjvex);
          }
          p = p.nextarc;
        }
      }
    }
  }

  return steps;
};
