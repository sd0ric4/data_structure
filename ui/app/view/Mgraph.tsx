import React, { useState, useEffect } from 'react';
import { Card } from '~/components/ui/card';
// 添加在文件顶部
interface GraphType {
  isDirected: boolean;
  isWeighted: boolean;
}

interface Position {
  x: number;
  y: number;
}

interface Degree {
  in: number;
  out: number;
}
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

const getEdgeColor = (index: number): string =>
  EDGE_COLORS[index % EDGE_COLORS.length];
const getTextColor = (index: number): string =>
  TEXT_COLORS[index % TEXT_COLORS.length];

// 辅助函数
const calculateDegrees = (matrix: number[][]): Degree[] => {
  const n = matrix.length;
  const degrees = Array(n)
    .fill(0)
    .map(() => ({ in: 0, out: 0 }));

  for (let i = 0; i < n; i++) {
    for (let j = 0; j < n; j++) {
      if (matrix[i][j] !== 0) {
        degrees[i].out++;
        degrees[j].in++;
      }
    }
  }

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

const getTotalEdges = (matrix: number[][]): number => {
  let count = 0;
  matrix.forEach((row) => {
    row.forEach((value) => {
      if (value !== 0) count++;
    });
  });
  return count;
};
interface MatrixDisplayProps {
  matrix: number[][];
}

// 子组件
const MatrixDisplay: React.FC<MatrixDisplayProps> = ({ matrix }) => {
  if (!matrix.length) return null;
  const degrees = calculateDegrees(matrix);

  return (
    <div className='overflow-x-auto'>
      <table className='min-w-full border-collapse'>
        <thead>
          <tr>
            <th className='p-2 border border-gray-300 bg-gray-50'></th>
            {matrix[0].map((_, i) => (
              <th
                key={i}
                className='p-2 border border-gray-300 bg-gray-50 w-12'
              >
                列{i}
              </th>
            ))}
            <th className='p-2 border border-gray-300 bg-gray-50'>入度</th>
            <th className='p-2 border border-gray-300 bg-gray-50'>出度</th>
          </tr>
        </thead>
        <tbody>
          {matrix.map((row, i) => (
            <tr key={i}>
              <td className='p-2 border border-gray-300 bg-gray-50 font-medium'>
                行{i}
              </td>
              {row.map((value, j) => (
                <td
                  key={j}
                  className='p-2 border border-gray-300 text-center bg-white'
                >
                  {value}
                </td>
              ))}
              <td className='p-2 border border-gray-300 text-center bg-white'>
                {degrees[i].in}
              </td>
              <td className='p-2 border border-gray-300 text-center bg-white'>
                {degrees[i].out}
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
};
interface GraphProps {
  matrix: number[][];
  graphType: GraphType;
}
const Graph: React.FC<GraphProps> = ({ matrix, graphType }) => {
  if (!matrix.length) return null;

  const positions = calculateLayout(matrix.length);
  const { isDirected, isWeighted } = graphType;
  const degrees = calculateDegrees(matrix);
  const totalEdges = getTotalEdges(matrix);

  return (
    <svg viewBox='0 0 600 400' className='w-full h-full'>
      <defs>
        {Array.from({ length: totalEdges }).map((_, index) => (
          <marker
            key={`arrowhead-${index}`}
            id={`arrowhead-${index}`}
            markerWidth='6'
            markerHeight='4'
            refX='5'
            refY='2'
            orient='auto'
            className={getTextColor(index)}
          >
            <path d='M0,0 L6,2 L0,4 Z' />
          </marker>
        ))}
      </defs>

      {matrix.map((row, i) =>
        row.map((weight, j) => {
          if (weight === 0 || (!isDirected && i > j)) return null;

          // 计算当前边的索引
          let edgeIndex = 0;
          let found = false;
          matrix.forEach((r, ri) => {
            r.forEach((v, rj) => {
              if (ri === i && rj === j) found = true;
              if (!found && v !== 0) edgeIndex++;
            });
          });

          const start = positions[i];
          const end = positions[j];

          const dx = end.x - start.x;
          const dy = end.y - start.y;

          // 增加曲率以区分重叠的边
          let curvature = 0;
          const parallelEdge = matrix[j][i] !== 0;
          if (parallelEdge) {
            curvature = i < j ? 50 : -50;
          }

          // 计算控制点
          const mx = (start.x + end.x) / 2;
          const my = (start.y + end.y) / 2;
          const angle = Math.atan2(dy, dx);
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

          // 计算权重文本位置
          const textOffset = parallelEdge ? (i < j ? 20 : -20) : 0;
          const textX = controlPoint.x;
          const textY = controlPoint.y + textOffset;

          return (
            <g key={`edge-${i}-${j}`}>
              <path
                d={`M ${startX} ${startY} Q ${controlPoint.x} ${controlPoint.y} ${endX} ${endY}`}
                className={`${getEdgeColor(edgeIndex)} transition-colors`}
                fill='none'
                strokeWidth='2.5'
                markerEnd={isDirected ? `url(#arrowhead-${edgeIndex})` : ''}
              />
              {isWeighted && weight !== 0 && (
                <text
                  x={textX}
                  y={textY}
                  className={`text-base font-semibold ${getTextColor(
                    edgeIndex
                  )} drop-shadow-sm`}
                  textAnchor='middle'
                >
                  {weight}
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
const GraphVisualizer: React.FC = () => {
  const [matrixText, setMatrixText] = useState<string>('0 1 2\n0 0 3\n0 0 0');
  const [matrix, setMatrix] = useState<number[][]>([]);
  const [graphType, setGraphType] = useState<GraphType>({
    isDirected: true,
    isWeighted: true,
  });
  const [error, setError] = useState<string>('');

  const parseMatrix = (text: string): void => {
    try {
      const rows = text.trim().split('\n');
      const parsed = rows.map((row) =>
        row
          .trim()
          .split(/\s+/)
          .map((val) => Number(val))
      );

      const size = parsed.length;
      if (!parsed.every((row) => row.length === size)) {
        throw new Error('矩阵必须是方阵');
      }
      if (parsed.some((row) => row.some((val) => isNaN(val) || val < 0))) {
        throw new Error('矩阵只能包含非负数');
      }

      setMatrix(parsed);

      // 自动检测图的类型
      let isDirected = false;
      let isWeighted = false;

      for (let i = 0; i < size; i++) {
        for (let j = 0; j < size; j++) {
          if (parsed[i][j] !== parsed[j][i]) {
            isDirected = true;
          }
          if (parsed[i][j] !== 0 && parsed[i][j] !== 1) {
            isWeighted = true;
          }
        }
      }

      setGraphType({ isDirected, isWeighted });
      setError('');
    } catch (err: unknown) {
      setError(err instanceof Error ? err.message : '未知错误');
      setMatrix([]);
    }
  };

  useEffect(() => {
    parseMatrix(matrixText);
  }, [matrixText]);

  const generateExample = () => {
    const size = 3;
    let example = '';
    for (let i = 0; i < size; i++) {
      for (let j = 0; j < size; j++) {
        if (i === j) {
          example += '0 ';
        } else {
          example += Math.floor(Math.random() * 5) + ' ';
        }
      }
      example = example.trim() + '\n';
    }
    setMatrixText(example.trim());
  };

  return (
    <div className='container mx-auto px-4 py-6 max-w-[1600px]'>
      <Card className='bg-white/80 p-6'>
        <div className='space-y-6'>
          <div className='flex justify-between items-center'>
            <h1 className='text-2xl font-semibold text-gray-900'>
              邻接矩阵可视化图
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

          {matrix.length > 0 && !error ? (
            <div className='grid grid-cols-1 lg:grid-cols-2 gap-6'>
              <div className='space-y-6'>
                <div>
                  <label className='block text-sm font-medium text-gray-700 mb-2'>
                    邻接矩阵:
                  </label>
                  <textarea
                    value={matrixText}
                    onChange={(e) => setMatrixText(e.target.value)}
                    className='w-full h-32 p-3 font-mono text-sm bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-transparent transition-shadow'
                    placeholder='请输入邻接矩阵，数字间用空格分隔，行用换行分隔'
                  />
                </div>
                <div className='bg-gray-50 rounded-lg p-4 border border-gray-200'>
                  <MatrixDisplay matrix={matrix} />
                </div>
              </div>

              <div className='bg-gray-50 rounded-lg p-4 border border-gray-200 min-h-[500px]'>
                <Graph matrix={matrix} graphType={graphType} />
              </div>
            </div>
          ) : (
            <div className='max-w-xl'>
              <div>
                <label className='block text-sm font-medium text-gray-700 mb-2'>
                  邻接矩阵:
                </label>
                <textarea
                  value={matrixText}
                  onChange={(e) => setMatrixText(e.target.value)}
                  className='w-full h-32 p-3 font-mono text-sm bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-transparent transition-shadow'
                  placeholder='请输入邻接矩阵，数字间用空格分隔，行用换行分隔'
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

export default GraphVisualizer;
