import React, { useState, useEffect } from 'react';
import { Card } from '~/components/ui/card';
import { Button } from '~/components/ui/button';
import { Play, Pause, RotateCcw } from 'lucide-react';
import { Alert, AlertTitle, AlertDescription } from '~/components/ui/alert';
import {
  INFINITY,
  GraphKind,
  type MGraph,
  type AnimationStep,
  primMST,
  kruskalMST,
} from '~/utils/algorithms/mst';

// 计算顶点布局位置
const calculateLayout = (nodes: number) => {
  const positions: { x: number; y: number }[] = [];
  const radius = 120;
  const centerX = 250;
  const centerY = 180;

  for (let i = 0; i < nodes; i++) {
    const angle = (i * 2 * Math.PI) / nodes - Math.PI / 2;
    positions.push({
      x: centerX + radius * Math.cos(angle),
      y: centerY + radius * Math.sin(angle),
    });
  }
  return positions;
};

// 主组件
const MSTVisualizer: React.FC = () => {
  const [graph, setGraph] = useState<MGraph | null>(null);
  const [algorithm, setAlgorithm] = useState<'prim' | 'kruskal'>('prim');
  const [animationSteps, setAnimationSteps] = useState<AnimationStep[]>([]);
  const [currentStep, setCurrentStep] = useState<number>(-1);
  const [isPlaying, setIsPlaying] = useState<boolean>(false);
  const [animationSpeed, setAnimationSpeed] = useState<number>(1000);

  // 初始化示例图
  useEffect(() => {
    const exampleGraph: MGraph = {
      vexs: ['A', 'B', 'C', 'D', 'E', 'F'],
      arcs: [
        [INFINITY, 6, 1, 5, INFINITY, INFINITY],
        [6, INFINITY, 5, INFINITY, 3, INFINITY],
        [1, 5, INFINITY, 5, 6, 4],
        [5, INFINITY, 5, INFINITY, INFINITY, 2],
        [INFINITY, 3, 6, INFINITY, INFINITY, 6],
        [INFINITY, INFINITY, 4, 2, 6, INFINITY],
      ],
      vexnum: 6,
      arcnum: 10,
      kind: GraphKind.UDN,
    };
    setGraph(exampleGraph);
  }, []);

  // 动画控制
  useEffect(() => {
    let intervalId: NodeJS.Timeout;

    if (isPlaying && currentStep < animationSteps.length - 1) {
      intervalId = setInterval(() => {
        setCurrentStep((prev) => prev + 1);
      }, animationSpeed);
    } else if (currentStep >= animationSteps.length - 1) {
      setIsPlaying(false);
    }

    return () => {
      if (intervalId) clearInterval(intervalId);
    };
  }, [isPlaying, currentStep, animationSteps.length, animationSpeed]);

  // 开始动画
  const startAnimation = () => {
    if (!graph) return;
    const steps = algorithm === 'prim' ? primMST(graph) : kruskalMST(graph);
    setAnimationSteps(steps);
    setCurrentStep(0);
    setIsPlaying(true);
  };

  // 重置动画
  const resetAnimation = () => {
    setCurrentStep(-1);
    setIsPlaying(false);
  };

  // 切换播放/暂停
  const togglePlayPause = () => {
    setIsPlaying(!isPlaying);
  };

  // 图形可视化组件
  const GraphVisualization: React.FC<{ graph: MGraph }> = ({ graph }) => {
    const positions = calculateLayout(graph.vexnum);
    const currentStepEdges = animationSteps
      .slice(0, currentStep + 1)
      .filter((step) => step.type === 'add')
      .map((step) => step.edge!);

    return (
      <svg viewBox='0 0 500 360' className='w-full h-full'>
        {/* 绘制所有边 */}
        {graph.vexs.map((_, i) =>
          graph.vexs.map((_, j) => {
            if (i < j && graph.arcs[i][j] !== INFINITY) {
              const start = positions[i];
              const end = positions[j];
              const isSelected = currentStepEdges.some(
                (edge) =>
                  (edge.start === i && edge.end === j) ||
                  (edge.start === j && edge.end === i)
              );

              return (
                <g key={`edge-${i}-${j}`}>
                  <line
                    x1={start.x}
                    y1={start.y}
                    x2={end.x}
                    y2={end.y}
                    className={`${
                      isSelected ? 'stroke-blue-500' : 'stroke-gray-300'
                    } transition-colors`}
                    strokeWidth={isSelected ? '3' : '1'}
                  />
                  <text
                    x={(start.x + end.x) / 2}
                    y={(start.y + end.y) / 2}
                    className='fill-gray-600 text-sm'
                    textAnchor='middle'
                    dominantBaseline='middle'
                  >
                    {graph.arcs[i][j]}
                  </text>
                </g>
              );
            }
            return null;
          })
        )}

        {/* 绘制顶点 */}
        {positions.map((pos, i) => (
          <g key={`node-${i}`}>
            <circle
              cx={pos.x}
              cy={pos.y}
              r='20'
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
              {graph.vexs[i]}
            </text>
          </g>
        ))}
      </svg>
    );
  };

  return (
    <div className='container mx-auto px-4 py-6 max-w-[1600px]'>
      <Card className='bg-white/80 p-6'>
        <div className='space-y-6'>
          <div className='mb-4'>
            <h1 className='text-2xl font-semibold text-gray-900'>
              最小生成树算法演示
            </h1>
          </div>

          <div className='grid grid-cols-1 lg:grid-cols-2 gap-6'>
            <div className='space-y-6'>
              <div className='flex gap-4'>
                <select
                  value={algorithm}
                  onChange={(e) =>
                    setAlgorithm(e.target.value as 'prim' | 'kruskal')
                  }
                  className='p-2 border rounded-lg bg-white text-gray-700'
                >
                  <option value='prim'>Prim算法</option>
                  <option value='kruskal'>Kruskal算法</option>
                </select>

                <Button
                  onClick={startAnimation}
                  disabled={!graph || isPlaying}
                  className='flex items-center gap-2'
                >
                  <Play className='w-4 h-4' />
                  开始生成
                </Button>

                <Button
                  onClick={togglePlayPause}
                  disabled={!graph || currentStep === -1}
                  className='flex items-center gap-2'
                >
                  {isPlaying ? (
                    <Pause className='w-4 h-4' />
                  ) : (
                    <Play className='w-4 h-4' />
                  )}
                  {isPlaying ? '暂停' : '继续'}
                </Button>

                <Button
                  onClick={resetAnimation}
                  disabled={!graph || currentStep === -1}
                  className='flex items-center gap-2'
                >
                  <RotateCcw className='w-4 h-4' />
                  重置
                </Button>
              </div>

              <div className='flex items-center gap-4'>
                <label className='text-sm font-medium text-gray-700'>
                  动画速度:
                </label>
                <input
                  type='range'
                  min='200'
                  max='2000'
                  step='100'
                  value={animationSpeed}
                  onChange={(e) => setAnimationSpeed(Number(e.target.value))}
                  className='w-48'
                />
                <span className='text-sm text-gray-500'>
                  {animationSpeed}ms
                </span>
              </div>

              {graph && currentStep >= 0 && (
                <div className='space-y-4'>
                  <Alert>
                    <AlertTitle>
                      当前步骤 ({currentStep + 1}/{animationSteps.length})
                    </AlertTitle>
                    <AlertDescription>
                      {animationSteps[currentStep].message}
                    </AlertDescription>
                  </Alert>

                  {algorithm === 'prim' &&
                    animationSteps[currentStep].closedge && (
                      <div className='bg-blue-50 p-4 rounded-lg'>
                        <h3 className='text-sm font-medium text-blue-900 mb-2'>
                          辅助数组 closedge[]
                        </h3>
                        <div className='grid grid-cols-6 gap-2'>
                          {graph.vexs.map((vex, i) => (
                            <div key={i} className='text-center'>
                              <div className='text-xs text-blue-600 mb-1'>
                                {vex}
                              </div>
                              <div className='bg-white p-2 rounded border border-blue-200 text-sm'>
                                {animationSteps[currentStep].closedge![i] ===
                                INFINITY
                                  ? '∞'
                                  : animationSteps[currentStep].closedge![i]}
                              </div>
                            </div>
                          ))}
                        </div>
                      </div>
                    )}

                  {algorithm === 'kruskal' &&
                    animationSteps[currentStep].cnvx && (
                      <div className='bg-purple-50 p-4 rounded-lg'>
                        <h3 className='text-sm font-medium text-purple-900 mb-2'>
                          连通分量数组 cnvx[]
                        </h3>
                        <div className='grid grid-cols-6 gap-2'>
                          {graph.vexs.map((vex, i) => (
                            <div key={i} className='text-center'>
                              <div className='text-xs text-purple-600 mb-1'>
                                {vex}
                              </div>
                              <div className='bg-white p-2 rounded border border-purple-200 text-sm'>
                                {animationSteps[currentStep].cnvx![i]}
                              </div>
                            </div>
                          ))}
                        </div>
                      </div>
                    )}

                  <div className='bg-green-50 p-4 rounded-lg'>
                    <h3 className='text-sm font-medium text-green-900 mb-2'>
                      已选边集合
                    </h3>
                    <div className='space-y-1'>
                      {animationSteps
                        .slice(0, currentStep + 1)
                        .filter((step) => step.type === 'add')
                        .map((step, i) => (
                          <div key={i} className='text-sm text-green-700'>
                            ({graph.vexs[step.edge!.start]},{' '}
                            {graph.vexs[step.edge!.end]}, {step.edge!.weight})
                          </div>
                        ))}
                    </div>
                  </div>
                </div>
              )}
            </div>

            <div className='bg-gray-50 rounded-lg p-4 border border-gray-200 min-h-[400px]'>
              {graph && <GraphVisualization graph={graph} />}
            </div>
          </div>

          <div className='mt-8'>
            <h2 className='text-lg font-semibold text-gray-900 mb-4'>
              算法说明
            </h2>
            {algorithm === 'prim' ? (
              <div className='bg-blue-50 p-4 rounded-lg space-y-2'>
                <h3 className='font-medium text-blue-900'>Prim算法步骤</h3>
                <ol className='list-decimal list-inside space-y-1 text-blue-800'>
                  <li>从任意顶点开始构造最小生成树</li>
                  <li>选择与当前树最近的顶点加入树中</li>
                  <li>更新其他顶点到树的距离</li>
                  <li>重复步骤2-3直到所有顶点都加入树中</li>
                </ol>
              </div>
            ) : (
              <div className='bg-purple-50 p-4 rounded-lg space-y-2'>
                <h3 className='font-medium text-purple-900'>Kruskal算法步骤</h3>
                <ol className='list-decimal list-inside space-y-1 text-purple-800'>
                  <li>将所有边按权值从小到大排序</li>
                  <li>
                    选择权值最小的边，如果加入该边不会形成回路，则加入生成树
                  </li>
                  <li>重复步骤2直到选择了n-1条边</li>
                </ol>
              </div>
            )}
          </div>
        </div>
      </Card>
    </div>
  );
};

export default MSTVisualizer;
