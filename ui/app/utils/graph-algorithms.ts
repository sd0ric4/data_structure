// 图的基本数据结构定义
export enum GraphKind {
  DG, // 有向图
  DN, // 有向网（带权值的有向图）
  UDG, // 无向图
  UDN, // 无向网（带权值的无向图）
}

// 边结点
export interface ArcNode {
  adjvex: number; // 该边指向的顶点的位置
  weight: number; // 边的权值
  nextArc: ArcNode | null; // 指向下一条边的指针
}

// 顶点结点
export interface VNode {
  data: string; // 顶点数据，使用字符串存储字母
  firstArc: ArcNode | null; // 指向第一条依附该顶点的边的指针
}

// 图的邻接表结构
export interface ALGraph {
  vertices: VNode[]; // 顶点数组
  vexnum: number; // 顶点数
  arcnum: number; // 边数
  kind: GraphKind; // 图的类型
}

// 创建图
export function createALGraph(
  vertices: string[],
  edges: [number, number, number][],
  kind: GraphKind
): ALGraph {
  // 创建顶点数组
  const vNodes: VNode[] = vertices.map((v) => ({
    data: v,
    firstArc: null,
  }));

  // 创建边
  edges.forEach(([from, to, weight]) => {
    // 创建边结点
    const arc: ArcNode = {
      adjvex: to,
      weight,
      nextArc: vNodes[from].firstArc,
    };
    vNodes[from].firstArc = arc;

    // 如果是无向图，需要创建反向边
    if (kind === GraphKind.UDG || kind === GraphKind.UDN) {
      const reverseArc: ArcNode = {
        adjvex: from,
        weight,
        nextArc: vNodes[to].firstArc,
      };
      vNodes[to].firstArc = reverseArc;
    }
  });

  return {
    vertices: vNodes,
    vexnum: vertices.length,
    arcnum:
      kind === GraphKind.UDG || kind === GraphKind.UDN
        ? edges.length * 2
        : edges.length,
    kind,
  };
}

// 输出图的邻接表
export function printALGraph(graph: ALGraph): string {
  let result = '';
  graph.vertices.forEach((vertex, i) => {
    result += `${vertex.data} -> `;
    let arc = vertex.firstArc;
    while (arc) {
      result += `${graph.vertices[arc.adjvex].data}`;
      if (arc.weight !== 1) {
        result += `(${arc.weight})`;
      }
      arc = arc.nextArc;
      if (arc) result += ' -> ';
    }
    result += '\n';
  });
  return result;
}

// 深度优先遍历
export function DFSTraverse(graph: ALGraph): string[] {
  const visited = new Array(graph.vexnum).fill(false);
  const result: string[] = [];

  function DFS(v: number) {
    visited[v] = true;
    result.push(graph.vertices[v].data);

    let arc = graph.vertices[v].firstArc;
    while (arc) {
      if (!visited[arc.adjvex]) {
        DFS(arc.adjvex);
      }
      arc = arc.nextArc;
    }
  }

  // 处理非连通图的情况
  for (let i = 0; i < graph.vexnum; i++) {
    if (!visited[i]) {
      DFS(i);
    }
  }

  return result;
}

// 广度优先遍历
export function BFSTraverse(graph: ALGraph): string[] {
  const visited = new Array(graph.vexnum).fill(false);
  const result: string[] = [];
  const queue: number[] = [];

  // 处理非连通图的情况
  for (let i = 0; i < graph.vexnum; i++) {
    if (!visited[i]) {
      // 访问当前顶点
      visited[i] = true;
      result.push(graph.vertices[i].data);
      queue.push(i);

      // 广度优先搜索
      while (queue.length > 0) {
        const v = queue.shift()!;
        let arc = graph.vertices[v].firstArc;

        while (arc) {
          if (!visited[arc.adjvex]) {
            visited[arc.adjvex] = true;
            result.push(graph.vertices[arc.adjvex].data);
            queue.push(arc.adjvex);
          }
          arc = arc.nextArc;
        }
      }
    }
  }

  return result;
}

// 将邻接表转换为边的数组形式
export function convertALGraphToEdges(
  graph: ALGraph
): [number, number, number][] {
  const edges: [number, number, number][] = [];
  const visited = new Set<string>();

  graph.vertices.forEach((vertex, from) => {
    let arc = vertex.firstArc;
    while (arc) {
      const to = arc.adjvex;
      const weight = arc.weight;

      // 对于无向图，只添加一次边
      const edgeKey =
        graph.kind === GraphKind.DG || graph.kind === GraphKind.DN
          ? `${from}-${to}`
          : `${Math.min(from, to)}-${Math.max(from, to)}`;

      if (!visited.has(edgeKey)) {
        edges.push([from, to, weight]);
        visited.add(edgeKey);
      }

      arc = arc.nextArc;
    }
  });

  return edges;
}

// 获取顶点的编号
export function getVertexIndex(graph: ALGraph, vertex: string): number {
  return graph.vertices.findIndex((v) => v.data === vertex);
}

// 示例图的创建函数
export function createExampleGraph(): ALGraph {
  const vertices = ['A', 'B', 'C', 'D', 'E'];
  const edges: [number, number, number][] = [
    [0, 1, 1], // A -> B
    [0, 3, 1], // A -> D
    [1, 2, 1], // B -> C
    [2, 4, 1], // C -> E
    [3, 4, 1], // D -> E
  ];
  return createALGraph(vertices, edges, GraphKind.DG);
}
