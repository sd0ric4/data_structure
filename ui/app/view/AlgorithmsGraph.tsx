import React, { useState, useEffect } from 'react';
import { Card } from '~/components/ui/card';
import { Button } from '~/components/ui/button';
import { Play, Pause, RotateCcw } from 'lucide-react';
import { Alert, AlertTitle, AlertDescription } from '~/components/ui/alert';
const MAX_VERTEX_NUM = 20;

enum GraphKind {
  DG,
  DN,
  UDG,
  UDN,
}

interface ArcNode {
  adjvex: number;
  nextarc: ArcNode | null;
  weight: number;
}

interface VNode {
  data: string;
  firstarc: ArcNode | null;
}

interface ALGraph {
  vertices: VNode[];
  vexnum: number;
  arcnum: number;
  kind: GraphKind;
}

interface AnimationStep {
  type: 'visit' | 'edge';
  from?: number;
  to?: number;
  node?: number;
}

const EDGE_COLORS = [
  'stroke-rose-500',
  'stroke-purple-500',
  'stroke-indigo-500',
  'stroke-blue-500',
  'stroke-cyan-500',
];

const TEXT_COLORS = [
  'fill-rose-500',
  'fill-purple-500',
  'fill-indigo-500',
  'fill-blue-500',
  'fill-cyan-500',
];

const getEdgeColor = (index: number): string =>
  EDGE_COLORS[index % EDGE_COLORS.length];
const getTextColor = (index: number): string =>
  TEXT_COLORS[index % TEXT_COLORS.length];

const DFSTraverse = (G: ALGraph): AnimationStep[] => {
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

const BFSTraverse = (G: ALGraph): AnimationStep[] => {
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
const AlgorithmDescription: React.FC<{ type: 'dfs' | 'bfs' }> = ({ type }) => {
  if (type === 'bfs') {
    return (
      <div className='bg-blue-50 p-4 rounded-lg space-y-2'>
        <h3 className='font-medium text-blue-900'>
          广度优先搜索 (BFS) 算法说明
        </h3>
        <p className='text-sm text-blue-800'>
          BFS是一种层级遍历算法，按以下步骤执行：
        </p>
        <ol className='list-decimal list-inside text-sm text-blue-800 space-y-1'>
          <li>从起始节点开始，将其标记为已访问并加入队列</li>
          <li>当队列不为空时，取出队首节点，访问它的所有未访问的邻接节点</li>
          <li>将这些邻接节点标记为已访问并加入队列</li>
          <li>重复步骤2-3直到队列为空</li>
          <li>如果还有未访问的节点，选择一个作为新的起始节点，重复以上过程</li>
        </ol>
      </div>
    );
  } else {
    return (
      <div className='bg-purple-50 p-4 rounded-lg space-y-2'>
        <h3 className='font-medium text-purple-900'>
          深度优先搜索 (DFS) 算法说明
        </h3>
        <p className='text-sm text-purple-800'>
          DFS是一种优先纵深访问的算法，按以下步骤执行：
        </p>
        <ol className='list-decimal list-inside text-sm text-purple-800 space-y-1'>
          <li>从起始节点开始，将其标记为已访问</li>
          <li>递归地访问该节点的第一个未访问的邻接节点</li>
          <li>当无法继续访问邻接节点时，回溯到上一个节点</li>
          <li>继续访问该节点的下一个未访问的邻接节点</li>
          <li>如果还有未访问的节点，选择一个作为新的起始节点，重复以上过程</li>
        </ol>
      </div>
    );
  }
};

const getDetailedStepDescription = (
  step: AnimationStep,
  graph: ALGraph,
  type: 'dfs' | 'bfs',
  visited: number[],
  queue?: number[]
): string => {
  if (step.type === 'visit') {
    const node = step.node!;
    if (visited.indexOf(node) === 0) {
      return `开始遍历，访问起始节点 ${graph.vertices[node].data}${
        type === 'bfs' ? ' 并将其加入队列' : ''
      }`;
    }
    if (type === 'bfs') {
      return `从队列中取出节点 ${graph.vertices[node].data} 进行访问${
        queue
          ? `。当前队列: [${queue
              .map((q) => graph.vertices[q].data)
              .join(', ')}]`
          : ''
      }`;
    } else {
      return `递归访问节点 ${graph.vertices[node].data}`;
    }
  } else {
    const from = graph.vertices[step.from!].data;
    const to = graph.vertices[step.to!].data;
    if (type === 'bfs') {
      return `发现节点 ${from} 的未访问邻接节点 ${to}，将其标记为已访问并加入队列${
        queue
          ? `。当前队列: [${queue
              .map((q) => graph.vertices[q].data)
              .join(', ')}]`
          : ''
      }`;
    } else {
      return `从节点 ${from} 沿边探索到未访问的邻接节点 ${to}，准备递归访问`;
    }
  }
};
const GraphVisualizer: React.FC = () => {
  const [graph, setGraph] = useState<ALGraph | null>(null);
  const [listText, setListText] = useState<string>(
    '0: 1, 3\n1: 2\n2: 4\n3: 4\n4:'
  );
  const [graphType, setGraphType] = useState<GraphKind>(GraphKind.DG);
  const [error, setError] = useState<string>('');
  const [animationSteps, setAnimationSteps] = useState<AnimationStep[]>([]);
  const [currentStep, setCurrentStep] = useState<number>(-1);
  const [isPlaying, setIsPlaying] = useState<boolean>(false);
  const [traversalType, setTraversalType] = useState<'dfs' | 'bfs'>('dfs');
  const [animationSpeed, setAnimationSpeed] = useState<number>(1000);
  const [queue, setQueue] = useState<number[]>([]);
  const [visitedNodes, setVisitedNodes] = useState<number[]>([]);
  useEffect(() => {
    if (currentStep >= 0) {
      const visited = animationSteps
        .slice(0, currentStep + 1)
        .filter((step) => step.type === 'visit')
        .map((step) => step.node!);
      setVisitedNodes(visited);

      if (traversalType === 'bfs') {
        // Simulate queue state for current step
        const newQueue = [...visited];
        const processed = new Set();
        animationSteps.slice(0, currentStep + 1).forEach((step) => {
          if (step.type === 'visit') {
            processed.add(step.node);
            newQueue.splice(newQueue.indexOf(step.node!), 1);
          } else if (step.type === 'edge') {
            if (!processed.has(step.to)) {
              newQueue.push(step.to!);
            }
          }
        });
        setQueue(newQueue);
      }
    } else {
      setVisitedNodes([]);
      setQueue([]);
    }
  }, [currentStep, animationSteps, traversalType]);
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

  const createALGraph = (text: string): ALGraph | null => {
    try {
      const lines = text.trim().split('\n');
      const vertices: VNode[] = [];

      lines.forEach((line, i) => {
        vertices.push({
          data: i.toString(),
          firstarc: null,
        });
      });

      lines.forEach((line, from) => {
        const [, edgesStr] = line.split(':');
        if (!edgesStr?.trim()) return;

        const edges = edgesStr.split(',').map((e) => e.trim());
        edges.forEach((edge) => {
          const to = parseInt(edge);
          if (!isNaN(to) && to >= 0 && to < vertices.length) {
            const newArc: ArcNode = {
              adjvex: to,
              weight: 1,
              nextarc: vertices[from].firstarc,
            };
            vertices[from].firstarc = newArc;
          }
        });
      });

      return {
        vertices,
        vexnum: vertices.length,
        arcnum: lines.length,
        kind: graphType,
      };
    } catch (err) {
      setError(err instanceof Error ? err.message : '格式错误');
      return null;
    }
  };

  useEffect(() => {
    const newGraph = createALGraph(listText);
    if (newGraph) {
      setGraph(newGraph);
      setError('');
      // Reset animation when graph changes
      setCurrentStep(-1);
      setIsPlaying(false);
    }
  }, [listText, graphType]);

  const startAnimation = () => {
    if (!graph) return;

    const steps =
      traversalType === 'dfs' ? DFSTraverse(graph) : BFSTraverse(graph);

    setAnimationSteps(steps);
    setCurrentStep(0);
    setIsPlaying(true);
  };

  const resetAnimation = () => {
    setCurrentStep(-1);
    setIsPlaying(false);
  };

  const togglePlayPause = () => {
    setIsPlaying(!isPlaying);
  };

  const isNodeVisited = (nodeIndex: number) => {
    return animationSteps
      .slice(0, currentStep + 1)
      .some((step) => step.type === 'visit' && step.node === nodeIndex);
  };

  const isEdgeTraversed = (from: number, to: number) => {
    return animationSteps
      .slice(0, currentStep + 1)
      .some(
        (step) => step.type === 'edge' && step.from === from && step.to === to
      );
  };

  const GraphVisualization: React.FC<{ graph: ALGraph }> = ({ graph }) => {
    const positions = calculateLayout(graph.vexnum);
    let edgeIndex = 0;

    return (
      <svg viewBox='0 0 500 360' className='w-full h-full'>
        <defs>
          {graph.vertices.map((vertex, i) => {
            let arc = vertex.firstarc;
            const markers = [];
            while (arc) {
              markers.push(
                <marker
                  key={`arrowhead-${edgeIndex}`}
                  id={`arrowhead-${edgeIndex}`}
                  markerWidth='6'
                  markerHeight='4'
                  refX='5'
                  refY='2'
                  orient='auto'
                  className={getTextColor(edgeIndex)}
                >
                  <path d='M0,0 L6,2 L0,4 Z' />
                </marker>
              );
              edgeIndex++;
              arc = arc.nextarc;
            }
            return markers;
          })}
        </defs>

        {graph.vertices.map((vertex, from) => {
          let arc = vertex.firstarc;
          const edges = [];
          let localEdgeIndex = 0;
          while (arc) {
            const start = positions[from];
            const end = positions[arc.adjvex];
            const currentEdgeIndex = from * graph.vexnum + localEdgeIndex;

            const isActive = isEdgeTraversed(from, arc.adjvex);

            edges.push(
              <path
                key={`edge-${from}-${arc.adjvex}`}
                d={`M ${start.x} ${start.y} L ${end.x} ${end.y}`}
                className={`${getEdgeColor(
                  currentEdgeIndex
                )} transition-colors ${
                  isActive ? 'stroke-2 opacity-100' : 'opacity-30'
                }`}
                fill='none'
                strokeWidth={isActive ? '3' : '2'}
                markerEnd={`url(#arrowhead-${currentEdgeIndex})`}
              />
            );

            localEdgeIndex++;
            arc = arc.nextarc;
          }
          return edges;
        })}

        {positions.map((pos, i) => {
          const isVisited = isNodeVisited(i);
          return (
            <g key={`node-${i}`}>
              <circle
                cx={pos.x}
                cy={pos.y}
                r='25'
                className={`${
                  isVisited
                    ? 'fill-blue-100 stroke-blue-500'
                    : 'fill-white stroke-gray-300'
                } transition-colors`}
                strokeWidth='2'
              />
              <text
                x={pos.x}
                y={pos.y}
                className={`text-base font-medium ${
                  isVisited ? 'fill-blue-700' : 'fill-gray-600'
                } transition-colors`}
                textAnchor='middle'
                dominantBaseline='middle'
              >
                {graph.vertices[i].data}
              </text>
            </g>
          );
        })}
      </svg>
    );
  };

  return (
    <div className='container mx-auto px-4 py-6 max-w-[1600px]'>
      <Card className='bg-white/80 p-6'>
        <div className='space-y-6'>
          <div className='mb-4'>
            <h1 className='text-2xl font-semibold text-gray-900'>
              图遍历动画演示
            </h1>
          </div>

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
                  placeholder='请输入邻接表，格式：0: 1, 3'
                />
              </div>

              <div className='flex gap-4'>
                <select
                  value={traversalType}
                  onChange={(e) =>
                    setTraversalType(e.target.value as 'dfs' | 'bfs')
                  }
                  className='p-2 border rounded-lg bg-white text-gray-700 text-sm focus:ring-2 focus:ring-blue-500 focus:border-transparent transition-shadow'
                >
                  <option value='dfs'>深度优先遍历 (DFS)</option>
                  <option value='bfs'>广度优先遍历 (BFS)</option>
                </select>

                <Button
                  onClick={startAnimation}
                  disabled={!graph || isPlaying}
                  className='flex items-center gap-2'
                >
                  <Play className='w-4 h-4' />
                  开始遍历
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

              {error && (
                <div className='p-3 bg-red-50 text-red-700 rounded-lg text-sm'>
                  {error}
                </div>
              )}
              {graph && currentStep >= 0 && (
                <div className='space-y-4'>
                  <Alert>
                    <AlertTitle>
                      当前步骤 ({currentStep + 1}/{animationSteps.length})
                    </AlertTitle>
                    <AlertDescription>
                      {getDetailedStepDescription(
                        animationSteps[currentStep],
                        graph,
                        traversalType,
                        visitedNodes,
                        queue
                      )}
                    </AlertDescription>
                  </Alert>

                  {currentStep + 1 < animationSteps.length && (
                    <Alert>
                      <AlertTitle>下一步操作</AlertTitle>
                      <AlertDescription>
                        {getDetailedStepDescription(
                          animationSteps[currentStep + 1],
                          graph,
                          traversalType,
                          visitedNodes,
                          queue
                        )}
                      </AlertDescription>
                    </Alert>
                  )}

                  <div className='bg-blue-50 p-4 rounded-lg'>
                    <h3 className='text-sm font-medium text-blue-900 mb-2'>
                      遍历状态
                    </h3>
                    <div className='space-y-2'>
                      <div className='text-sm text-blue-700'>
                        <span className='font-medium'>已访问节点:</span>{' '}
                        {visitedNodes
                          .map((node) => graph.vertices[node].data)
                          .join(' → ')}
                      </div>
                      {traversalType === 'bfs' && (
                        <div className='text-sm text-blue-700'>
                          <span className='font-medium'>当前队列:</span>{' '}
                          {queue.length > 0
                            ? queue
                                .map((node) => graph.vertices[node].data)
                                .join(', ')
                            : '空'}
                        </div>
                      )}
                    </div>
                  </div>
                </div>
              )}
            </div>

            <div className='bg-gray-50 rounded-lg p-4 border border-gray-200 min-h-[400px]'>
              {graph && <GraphVisualization graph={graph} />}
            </div>
          </div>
        </div>
      </Card>
    </div>
  );
};

export default GraphVisualizer;
