// main.ts
import {
  MGraph,
  GraphKind,
  CreateMGraph,
  OutMGraph,
  VerDegree,
} from './MGraph';

// 主函数
function main() {
  // 定义四种图类型
  const kinds: GraphKind[] = [
    GraphKind.DG,
    GraphKind.DN,
    GraphKind.UDG,
    GraphKind.UDN,
  ];
  const kind_names: string[] = ['有向图', '有向网', '无向图', '无向网'];

  // 示例数据：顶点 A, B, C, D
  const vexnum: number = 4;
  const arcnum: number = 4;
  const vexs: string[] = ['A', 'B', 'C', 'D'];

  // 定义弧数组，每条弧采用三元组 (i, j, w)
  // 示例边：A->B, A->C, B->D, C->D
  const arcs_DG: number[] = [0, 1, 1, 0, 2, 1, 1, 3, 1, 2, 3, 1]; // 无权图，权值设为1
  const arcs_DN: number[] = [0, 1, 5, 0, 2, 3, 1, 3, 2, 2, 3, 4]; // 有向网
  const arcs_UDG: number[] = [0, 1, 1, 0, 2, 1, 1, 3, 1, 2, 3, 1]; // 无向图
  const arcs_UDN: number[] = [0, 1, 5, 0, 2, 3, 1, 3, 2, 2, 3, 4]; // 无向网

  // 将所有弧数组存入一个列表
  const arcsList: number[][] = [arcs_DG, arcs_DN, arcs_UDG, arcs_UDN];

  // 遍历每种图类型
  for (let k = 0; k < 4; k++) {
    const currentKind = kinds[k];
    const currentArcs = arcsList[k];

    console.log(`创建 ${kind_names[k]} ...`);
    const G = new MGraph(vexnum, arcnum, currentKind);
    CreateMGraph(currentKind, G, vexnum, arcnum, vexs, currentArcs);
    console.log('图创建完成。');

    console.log('输出图的信息:');
    OutMGraph(G);

    console.log('输出各顶点的度:');
    VerDegree(G);
    console.log('-----------------------------');
  }
}

main();
