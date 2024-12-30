// MGraph.ts

export enum GraphKind {
  DG, // 有向图
  DN, // 有向网
  UDG, // 无向图
  UDN, // 无向网
}

export const INFINITY = 32767;
export const MAX_VERTEX_NUM = 20;

export type VertexType = string;

export class ArcCell {
  adj: number; // 对无权图用1或0表示；对带权图，则为权值类型
  info?: string; // 该边或弧相关信息的指针（字符串）

  constructor(adj: number, info?: string) {
    this.adj = adj;
    this.info = info;
  }
}

export class MGraph {
  vexs: VertexType[];
  arcs: ArcCell[][];
  vexnum: number;
  arcnum: number;
  kind: GraphKind;

  constructor(vexnum: number, arcnum: number, kind: GraphKind) {
    this.vexs = new Array<VertexType>(vexnum);
    this.arcs = Array.from({ length: vexnum }, () => Array<ArcCell>(vexnum));
    this.vexnum = vexnum;
    this.arcnum = arcnum;
    this.kind = kind;

    // 初始化邻接矩阵
    for (let i = 0; i < vexnum; i++) {
      for (let j = 0; j < vexnum; j++) {
        this.arcs[i][j] = new ArcCell(0);
      }
    }
  }
}

// 创建图的邻接矩阵存储结构
export function CreateMGraph(
  GKind: GraphKind,
  G: MGraph,
  vexnum: number,
  arcnum: number,
  vexs: string[],
  arcs: number[]
): void {
  G.vexnum = vexnum;
  G.arcnum = arcnum;
  G.kind = GKind;

  // 初始化顶点
  for (let i = 0; i < vexnum; i++) {
    G.vexs[i] = vexs[i];
  }

  // 初始化邻接矩阵
  for (let i = 0; i < vexnum; i++) {
    for (let j = 0; j < vexnum; j++) {
      G.arcs[i][j].adj = 0;
      G.arcs[i][j].info = undefined;
    }
  }

  // 插入弧
  for (let k = 0; k < arcnum; k++) {
    let i = arcs[3 * k];
    let j = arcs[3 * k + 1];
    let w = arcs[3 * k + 2];
    if (GKind === GraphKind.DG || GKind === GraphKind.UDG) {
      G.arcs[i][j].adj = 1;
      if (GKind === GraphKind.UDG) {
        G.arcs[j][i].adj = 1;
      }
    } else {
      G.arcs[i][j].adj = w;
      if (GKind === GraphKind.UDN) {
        G.arcs[j][i].adj = w;
      }
    }
  }

  // 对于无权图，设置权值为1
  if (GKind === GraphKind.DG || GKind === GraphKind.UDG) {
    for (let k = 0; k < arcnum; k++) {
      let i = arcs[3 * k];
      let j = arcs[3 * k + 1];
      G.arcs[i][j].adj = 1;
      if (GKind === GraphKind.UDG) {
        G.arcs[j][i].adj = 1;
      }
    }
  }
}

// 输出图的邻接矩阵
export function OutMGraph(G: MGraph): void {
  console.log('顶点数组:', G.vexs.join(' '));

  console.log('邻接矩阵:');
  for (let i = 0; i < G.vexnum; i++) {
    let row = '';
    for (let j = 0; j < G.vexnum; j++) {
      if (G.arcs[i][j].adj === 0) row += '0 ';
      else if (
        (G.kind === GraphKind.UDG || G.kind === GraphKind.DG) &&
        G.arcs[i][j].adj === 1
      )
        row += '1 ';
      else if (G.arcs[i][j].adj < INFINITY) row += G.arcs[i][j].adj + ' ';
      else row += '∞ ';
    }
    console.log(row.trim());
  }
}

// 输出图中各顶点的度
export function VerDegree(G: MGraph): void {
  console.log('顶点的度:');
  if (G.kind === GraphKind.DG || G.kind === GraphKind.DN) {
    // 有向图：计算入度和出度
    for (let i = 0; i < G.vexnum; i++) {
      let inDegree = 0,
        outDegree = 0;
      for (let j = 0; j < G.vexnum; j++) {
        if (G.arcs[j][i].adj > 0 && G.arcs[j][i].adj < INFINITY) inDegree++;
        if (G.arcs[i][j].adj > 0 && G.arcs[i][j].adj < INFINITY) outDegree++;
      }
      console.log(
        `${G.vexs[i]}: in-degree = ${inDegree}, out-degree = ${outDegree}`
      );
    }
  } else {
    // 无向图：度为相邻边数
    for (let i = 0; i < G.vexnum; i++) {
      let degree = 0;
      for (let j = 0; j < G.vexnum; j++) {
        if (G.arcs[i][j].adj > 0 && G.arcs[i][j].adj < INFINITY) degree++;
      }
      console.log(`${G.vexs[i]}: degree = ${degree}`);
    }
  }
}
