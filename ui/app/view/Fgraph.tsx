import React, { useState, useEffect } from 'react';
import { Card } from '~/components/ui/card';

// 类型定义
interface GraphType {
  isDirected: boolean;
  isWeighted: boolean;
}

interface Position {
  x: number;
  y: number;
}

interface Edge {
  to: number;
  weight: number;
}

interface Degree {
  in: number;
  out: number;
}
const COLORS = [
  'rose-500',
  'purple-500',
  'indigo-500',
  'blue-500',
  'cyan-500',
  'teal-500',
  'emerald-500',
  'green-500',
  'amber-500',
  'orange-500',
];
// 颜色配置
const EDGE_COLORS = [
  'stroke-rose-500',
  'stroke-purple-500',
  'stroke-indigo-500',
  'stroke-blue-500',
  'stroke-cyan-500',
  'stroke-teal-500',
  'stroke-emerald-500',
  'stroke-green-500',
  'stroke-amber-500',
  'stroke-orange-500',
];

const TEXT_COLORS = [
  'fill-rose-500',
  'fill-purple-500',
  'fill-indigo-500',
  'fill-blue-500',
  'fill-cyan-500',
  'fill-teal-500',
  'fill-emerald-500',
  'fill-green-500',
  'fill-amber-500',
  'fill-orange-500',
];
const getColor = (index: number): string => COLORS[index % COLORS.length];

const getEdgeColor = (index: number): string =>
  EDGE_COLORS[index % EDGE_COLORS.length];
const getTextColor = (index: number): string =>
  TEXT_COLORS[index % TEXT_COLORS.length];

// 辅助函数
const calculateDegrees = (adjList: Edge[][]): Degree[] => {
  const degrees = Array(adjList.length)
    .fill(0)
    .map(() => ({ in: 0, out: 0 }));

  adjList.forEach((edges, from) => {
    edges.forEach((edge) => {
      degrees[from].out++;
      degrees[edge.to].in++;
    });
  });

  return degrees;
};

const calculateLayout = (nodes: number): Position[] => {
  const positions: Position[] = [];
  const radius = 150;
  const centerX = 300;
  const centerY = 200;

  for (let i = 0; i < nodes; i++) {
    const angle = (i * 2 * Math.PI) / nodes - Math.PI / 2;
    positions.push({
      x: centerX + radius * Math.cos(angle),
      y: centerY + radius * Math.sin(angle),
    });
  }
  return positions;
};

// 子组件：邻接表显示
interface ListDisplayProps {
  adjList: Edge[][];
}

const ListDisplay: React.FC<ListDisplayProps> = ({ adjList }) => {
  if (!adjList.length) return null;
  const degrees = calculateDegrees(adjList);

  return (
    <div className='overflow-x-auto'>
      <table className='min-w-full border-collapse'>
        <thead>
          <tr>
            <th className='p-2 border border-gray-300 bg-gray-50'>顶点</th>
            <th className='p-2 border border-gray-300 bg-gray-50'>邻接点</th>
            <th className='p-2 border border-gray-300 bg-gray-50'>入度</th>
            <th className='p-2 border border-gray-300 bg-gray-50'>出度</th>
          </tr>
        </thead>
        <tbody>
          {adjList.map((edges, i) => (
            <tr key={i}>
              <td className='p-2 border border-gray-300 bg-gray-50 font-medium'>
                {i}
              </td>
              <td className='p-2 border border-gray-300'>
                {edges.map((edge, j) => (
                  <span key={j} className='inline-block mr-2'>
                    {edge.to}
                    {edge.weight !== 1 ? `(${edge.weight})` : ''}
                  </span>
                ))}
              </td>
              <td className='p-2 border border-gray-300 text-center'>
                {degrees[i].in}
              </td>
              <td className='p-2 border border-gray-300 text-center'>
                {degrees[i].out}
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
};

// 子组件：图形显示
interface GraphProps {
  adjList: Edge[][];
  graphType: GraphType;
}

const Graph: React.FC<GraphProps> = ({ adjList, graphType }) => {
  if (!adjList.length) return null;

  const positions = calculateLayout(adjList.length);
  const { isDirected, isWeighted } = graphType;
  const degrees = calculateDegrees(adjList);

  let edgeIndex = 0;

  return (
    <svg viewBox='0 0 600 400' className='w-full h-full'>
      <defs>
        {adjList.map((edges, from) =>
          edges.map((edge, idx) => {
            const colorIndex = from * adjList.length + idx;
            const color = getColor(colorIndex);
            return (
              <marker
                key={`arrowhead-${colorIndex}`}
                id={`arrowhead-${colorIndex}`}
                markerWidth='6'
                markerHeight='4'
                refX='5'
                refY='2'
                orient='auto'
              >
                <path d='M0,0 L6,2 L0,4 Z' className={`fill-${color}`} />
              </marker>
            );
          })
        )}
      </defs>

      {adjList.map((edges, from) =>
        edges.map((edge, idx) => {
          const start = positions[from];
          const end = positions[edge.to];
          const colorIndex = from * adjList.length + idx;
          const color = getColor(colorIndex);

          // 检查是否存在反向边
          const hasReverseEdge = adjList[edge.to].some((e) => e.to === from);

          // 计算曲线控制点
          let curvature = 0;
          if (hasReverseEdge) {
            curvature = from < edge.to ? 50 : -50;
          }

          const dx = end.x - start.x;
          const dy = end.y - start.y;
          const angle = Math.atan2(dy, dx);

          // 计算控制点
          const mx = (start.x + end.x) / 2;
          const my = (start.y + end.y) / 2;
          const perpX = -Math.sin(angle) * curvature;
          const perpY = Math.cos(angle) * curvature;
          const controlPoint = {
            x: mx + perpX,
            y: my + perpY,
          };

          const nodeRadius = 25;
          const edgeAngle = Math.atan2(
            end.y - controlPoint.y,
            end.x - controlPoint.x
          );

          const startX = start.x + nodeRadius * Math.cos(angle);
          const startY = start.y + nodeRadius * Math.sin(angle);
          const endX = end.x - nodeRadius * Math.cos(edgeAngle);
          const endY = end.y - nodeRadius * Math.sin(edgeAngle);

          const currentEdgeIndex = from * adjList.length + idx;

          return (
            <g key={`edge-${from}-${edge.to}`}>
              <path
                d={`M ${startX} ${startY} Q ${controlPoint.x} ${controlPoint.y} ${endX} ${endY}`}
                className={`stroke-${color}`}
                fill='none'
                strokeWidth='2.5'
                markerEnd={isDirected ? `url(#arrowhead-${colorIndex})` : ''}
              />
              {isWeighted && edge.weight !== 1 && (
                <text
                  x={controlPoint.x}
                  y={
                    controlPoint.y +
                    (hasReverseEdge ? (from < edge.to ? 20 : -20) : 0)
                  }
                  className={`text-base font-semibold fill-${color} drop-shadow-sm`}
                  textAnchor='middle'
                >
                  {edge.weight}
                </text>
              )}
            </g>
          );
        })
      )}

      {positions.map((pos, i) => (
        <g key={`node-${i}`}>
          <circle
            cx={pos.x}
            cy={pos.y}
            r='25'
            className='fill-white stroke-gray-300'
            strokeWidth='2'
          />
          <text
            x={pos.x}
            y={pos.y}
            className='text-base font-medium fill-gray-600'
            textAnchor='middle'
            dominantBaseline='middle'
          >
            {i}
          </text>

          <g transform={`translate(${pos.x - 35}, ${pos.y - 35})`}>
            <rect
              x='-15'
              y='-10'
              width='30'
              height='20'
              rx='4'
              className='fill-white/80'
            />
            <text
              className='text-xs font-medium fill-blue-600'
              textAnchor='middle'
              dominantBaseline='middle'
            >
              入:{degrees[i].in}
            </text>
          </g>

          <g transform={`translate(${pos.x + 35}, ${pos.y - 35})`}>
            <rect
              x='-15'
              y='-10'
              width='30'
              height='20'
              rx='4'
              className='fill-white/80'
            />
            <text
              className='text-xs font-medium fill-orange-600'
              textAnchor='middle'
              dominantBaseline='middle'
            >
              出:{degrees[i].out}
            </text>
          </g>
        </g>
      ))}
    </svg>
  );
};

// 主组件
const AdjacencyListVisualizer: React.FC = () => {
  const [listText, setListText] = useState<string>(
    '0: 1(2), 2(3)\n1: 2(1)\n2:'
  );
  const [adjList, setAdjList] = useState<Edge[][]>([]);
  const [graphType, setGraphType] = useState<GraphType>({
    isDirected: true,
    isWeighted: true,
  });
  const [error, setError] = useState<string>('');

  const parseAdjList = (text: string): void => {
    try {
      const lines = text.trim().split('\n');
      const parsed: Edge[][] = [];

      // 首先确定顶点数量
      const vertices = lines.length;

      // 初始化邻接表
      for (let i = 0; i < vertices; i++) {
        parsed[i] = [];
      }

      let isDirected = false;
      let isWeighted = false;

      lines.forEach((line, index) => {
        const [vertex, edgesStr] = line.split(':');
        if (parseInt(vertex) !== index) {
          throw new Error('顶点必须按顺序排列，从0开始');
        }

        if (edgesStr.trim()) {
          const edges = edgesStr
            .trim()
            .split(',')
            .map((e) => e.trim());
          edges.forEach((edge) => {
            const match = edge.match(/(\d+)(?:\((\d+)\))?/);
            if (!match) {
              throw new Error('边格式无效');
            }
            const to = parseInt(match[1]);
            const weight = match[2] ? parseInt(match[2]) : 1;

            if (to >= vertices) {
              throw new Error('顶点编号不能超过顶点总数');
            }

            if (weight !== 1) isWeighted = true;
            parsed[index].push({ to, weight });
          });
        }
      });

      // 检查是否为有向图
      for (let i = 0; i < vertices; i++) {
        for (const edge of parsed[i]) {
          const hasReverse = parsed[edge.to].some((e) => e.to === i);
          if (!hasReverse) {
            isDirected = true;
            break;
          }
        }
        if (isDirected) break;
      }

      setGraphType({ isDirected, isWeighted });
      setAdjList(parsed);
      setError('');
    } catch (err: unknown) {
      setError(err instanceof Error ? err.message : '未知错误');
      setAdjList([]);
    }
  };

  useEffect(() => {
    parseAdjList(listText);
  }, [listText]);

  const generateExample = () => {
    const vertices = 3;
    let example = '';
    for (let i = 0; i < vertices; i++) {
      example += `${i}: `;
      const edges = [];
      for (let j = 0; j < vertices; j++) {
        if (i !== j && Math.random() < 0.5) {
          const weight = Math.floor(Math.random() * 4) + 1;
          edges.push(`${j}${weight > 1 ? `(${weight})` : ''}`);
        }
      }
      example += edges.join(', ');
      example += '\n';
    }
    setListText(example.trim());
  };

  return (
    <div className='container mx-auto px-4 py-6 max-w-[1600px]'>
      <Card className='bg-white/80 p-6'>
        <div className='space-y-6'>
          <div className='flex justify-between items-center'>
            <h1 className='text-2xl font-semibold text-gray-900'>
              邻接表可视化图
            </h1>
            <button
              onClick={generateExample}
              className='px-4 py-2 bg-blue-500 text-white rounded-lg hover:bg-blue-600 transition-colors'
            >
              生成示例
            </button>
          </div>

          <div className='flex gap-4 text-sm'>
            <div
              className={`px-3 py-1.5 rounded-full ${
                graphType.isDirected
                  ? 'bg-blue-100 text-blue-700'
                  : 'bg-gray-100 text-gray-700'
              }`}
            >
              {graphType.isDirected ? '有向图' : '无向图'}
            </div>
            <div
              className={`px-3 py-1.5 rounded-full ${
                graphType.isWeighted
                  ? 'bg-orange-100 text-orange-700'
                  : 'bg-gray-100 text-gray-700'
              }`}
            >
              {graphType.isWeighted ? '有权图' : '无权图'}
            </div>
          </div>

          {adjList.length > 0 && !error ? (
            <div className='grid grid-cols-1 lg:grid-cols-2 gap-6'>
              <div className='space-y-6'>
                <div>
                  <label className='block text-sm font-medium text-gray-700 mb-2'>
                    邻接表:
                  </label>
                  <textarea
                    value={listText}
                    onChange={(e) => setListText(e.target.value)}
                    className='w-full h-32 p-3 font-mono text-sm bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-transparent transition-shadow'
                    placeholder='请输入邻接表，格式：顶点: 邻接点1(权重1), 邻接点2(权重2)...'
                  />
                </div>
                <div className='bg-gray-50 rounded-lg p-4 border border-gray-200'>
                  <ListDisplay adjList={adjList} />
                </div>
              </div>

              <div className='bg-gray-50 rounded-lg p-4 border border-gray-200 min-h-[500px]'>
                <Graph adjList={adjList} graphType={graphType} />
              </div>
            </div>
          ) : (
            <div className='max-w-xl'>
              <div>
                <label className='block text-sm font-medium text-gray-700 mb-2'>
                  邻接表:
                </label>
                <textarea
                  value={listText}
                  onChange={(e) => setListText(e.target.value)}
                  className='w-full h-32 p-3 font-mono text-sm bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-transparent transition-shadow'
                  placeholder='请输入邻接表，格式：顶点: 邻接点1(权重1), 邻接点2(权重2)...'
                />
              </div>
              {error && (
                <div className='mt-4 p-3 bg-red-50 text-red-700 rounded-lg text-sm'>
                  {error}
                </div>
              )}
            </div>
          )}
        </div>
      </Card>
    </div>
  );
};

export default AdjacencyListVisualizer;
