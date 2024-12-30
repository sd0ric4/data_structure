import React, { useState, useEffect } from 'react';
import { Card } from '~/components/ui/card';
import { Button } from '~/components/ui/button';
import { Play, Pause, RotateCcw, ZoomIn, ZoomOut } from 'lucide-react';
import { Alert, AlertTitle, AlertDescription } from '~/components/ui/alert';

// Constants and Types
const INFINITY = 32767;

interface Edge {
  to: number;
  weight: number;
}

interface Vertex {
  earlyTime: number;
  lateTime: number;
  inDegree: number;
}

interface AnimationStep {
  type: 'forward' | 'backward' | 'critical';
  vertex?: number;
  message: string;
  vertices: Vertex[];
  criticalPath?: number[];
}

// Calculate vertex layout positions
const calculateLayout = (adjList: Edge[][]) => {
  const positions: { x: number; y: number }[] = [];
  const width = 800;
  const height = 400;
  const startX = 100;
  const startY = 80;

  const inDegrees = new Array(adjList.length).fill(0);
  adjList.forEach((edges) => {
    edges.forEach((edge) => {
      inDegrees[edge.to]++;
    });
  });

  const layers: number[][] = [];
  const visited = new Set<number>();
  const queue: number[] = [];

  inDegrees.forEach((degree, index) => {
    if (degree === 0) queue.push(index);
  });

  while (queue.length > 0) {
    const currentLayer = [...queue];
    layers.push(currentLayer);
    queue.length = 0;

    for (const node of currentLayer) {
      visited.add(node);
      adjList[node].forEach((edge) => {
        if (!visited.has(edge.to)) {
          inDegrees[edge.to]--;
          if (inDegrees[edge.to] === 0) {
            queue.push(edge.to);
          }
        }
      });
    }
  }

  const remainingNodes = [];
  for (let i = 0; i < adjList.length; i++) {
    if (!visited.has(i)) remainingNodes.push(i);
  }
  if (remainingNodes.length > 0) {
    layers.push(remainingNodes);
  }

  const layerWidth = width / (layers.length || 1);
  layers.forEach((layer, layerIndex) => {
    const layerX = startX + layerIndex * layerWidth;
    const spacing = height / (layer.length + 1);
    layer.forEach((nodeIndex, index) => {
      positions[nodeIndex] = {
        x: layerX,
        y: startY + (index + 1) * spacing,
      };
    });
  });

  return positions;
};

// Critical path calculation function
const calculateCriticalPath = (adjList: Edge[][]): AnimationStep[] => {
  const steps: AnimationStep[] = [];
  const n = adjList.length;
  const vertices: Vertex[] = Array(n)
    .fill(null)
    .map(() => ({
      earlyTime: 0,
      lateTime: INFINITY,
      inDegree: 0,
    }));

  adjList.forEach((edges) => {
    edges.forEach((edge) => {
      vertices[edge.to].inDegree++;
    });
  });

  // Forward pass
  const queue: number[] = [];
  for (let i = 0; i < n; i++) {
    if (vertices[i].inDegree === 0) queue.push(i);
  }

  steps.push({
    type: 'forward',
    message: '开始前向传播计算最早开始时间',
    vertices: JSON.parse(JSON.stringify(vertices)),
  });

  while (queue.length > 0) {
    const v = queue.shift()!;
    adjList[v].forEach((edge) => {
      const newEarly = vertices[v].earlyTime + edge.weight;
      if (newEarly > vertices[edge.to].earlyTime) {
        vertices[edge.to].earlyTime = newEarly;
      }
      vertices[edge.to].inDegree--;
      if (vertices[edge.to].inDegree === 0) {
        queue.push(edge.to);
      }
    });

    steps.push({
      type: 'forward',
      vertex: v,
      message: `计算顶点 ${String.fromCharCode(65 + v)} 的最早开始时间: ${
        vertices[v].earlyTime
      }`,
      vertices: JSON.parse(JSON.stringify(vertices)),
    });
  }

  // Find maximum completion time
  const maxTime = Math.max(...vertices.map((v) => v.earlyTime));

  // Backward pass
  vertices.forEach((v) => (v.lateTime = maxTime));
  steps.push({
    type: 'backward',
    message: '开始反向传播计算最晚开始时间',
    vertices: JSON.parse(JSON.stringify(vertices)),
  });

  for (let i = n - 1; i >= 0; i--) {
    adjList[i].forEach((edge) => {
      const newLate = vertices[edge.to].lateTime - edge.weight;
      if (newLate < vertices[i].lateTime) {
        vertices[i].lateTime = newLate;
      }
    });

    steps.push({
      type: 'backward',
      vertex: i,
      message: `计算顶点 ${String.fromCharCode(65 + i)} 的最晚开始时间: ${
        vertices[i].lateTime
      }`,
      vertices: JSON.parse(JSON.stringify(vertices)),
    });
  }

  // Identify critical path
  const criticalPath: number[] = [];
  for (let i = 0; i < n; i++) {
    if (vertices[i].earlyTime === vertices[i].lateTime) {
      criticalPath.push(i);
    }
  }

  steps.push({
    type: 'critical',
    message: '关键路径已找到',
    vertices: JSON.parse(JSON.stringify(vertices)),
    criticalPath,
  });

  return steps;
};

const CriticalPathVisualizer = () => {
  const [adjList, setAdjList] = useState<Edge[][]>([]);
  const [animationSteps, setAnimationSteps] = useState<AnimationStep[]>([]);
  const [currentStep, setCurrentStep] = useState<number>(-1);
  const [isPlaying, setIsPlaying] = useState<boolean>(false);
  const [animationSpeed, setAnimationSpeed] = useState<number>(1000);
  const [transform, setTransform] = useState({ x: 0, y: 0, scale: 1 });
  const [isDragging, setIsDragging] = useState(false);
  const [dragStart, setDragStart] = useState({ x: 0, y: 0 });
  const [inputText, setInputText] = useState<string>(
    'A: B(8), C(6), E(7)\nB: D(3)\nC: D(10), G(9)\nD: F(4), I(19)\nE: G(9), H(13)\nF: J(14)\nG: I(8), H(2)\nH: I(6)\nI: J(10)\nJ:'
  );

  // Parse adjacency list from input text
  const parseAdjList = (text: string): void => {
    try {
      const lines = text.trim().split('\n');
      const vertexMap = new Map<string, number>();

      // Create vertex mapping
      lines.forEach((line) => {
        const vertex = line.split(':')[0].trim();
        if (!vertexMap.has(vertex)) {
          vertexMap.set(vertex, vertexMap.size);
        }
      });

      // Initialize adjacency list
      const parsed: Edge[][] = Array(vertexMap.size)
        .fill(null)
        .map(() => []);

      // Parse edges
      lines.forEach((line) => {
        const [vertex, edgesStr] = line.split(':');
        const fromIndex = vertexMap.get(vertex.trim())!;

        if (edgesStr.trim()) {
          const edges = edgesStr
            .trim()
            .split(',')
            .map((e) => e.trim());
          edges.forEach((edge) => {
            const match = edge.match(/([A-Z])\((\d+)\)/);
            if (match) {
              const toVertex = match[1];
              const weight = parseInt(match[2]);
              const toIndex = vertexMap.get(toVertex)!;
              parsed[fromIndex].push({ to: toIndex, weight });
            }
          });
        }
      });

      setAdjList(parsed);
    } catch (error) {
      console.error('Error parsing input:', error);
    }
  };

  // Initialize on input change
  useEffect(() => {
    parseAdjList(inputText);
  }, [inputText]);

  // Animation control
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

  // Mouse event handlers for pan
  const handleMouseDown = (e: React.MouseEvent) => {
    setIsDragging(true);
    setDragStart({
      x: e.clientX - transform.x,
      y: e.clientY - transform.y,
    });
  };

  const handleMouseMove = (e: React.MouseEvent) => {
    if (!isDragging) return;
    setTransform((prev) => ({
      ...prev,
      x: e.clientX - dragStart.x,
      y: e.clientY - dragStart.y,
    }));
  };

  const handleMouseUp = () => {
    setIsDragging(false);
  };

  // Zoom controls
  const handleZoomIn = () => {
    setTransform((prev) => ({
      ...prev,
      scale: Math.min(prev.scale * 1.2, 3),
    }));
  };

  const handleZoomOut = () => {
    setTransform((prev) => ({
      ...prev,
      scale: Math.max(prev.scale / 1.2, 0.5),
    }));
  };

  const handleResetView = () => {
    setTransform({ x: 0, y: 0, scale: 1 });
  };

  // Animation control functions
  const startAnimation = () => {
    if (!adjList.length) return;
    const steps = calculateCriticalPath(adjList);
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

  // Graph visualization component
  const GraphVisualization = () => {
    const positions = calculateLayout(adjList);
    const currentVertex =
      currentStep >= 0 ? animationSteps[currentStep].vertex : null;
    const criticalPath =
      currentStep >= 0 ? animationSteps[currentStep].criticalPath : [];

    return (
      <svg
        viewBox='0 0 1200 800'
        className='w-full h-full min-h-[600px]'
        preserveAspectRatio='xMidYMid meet'
      >
        <defs>
          <marker
            id='arrowhead'
            markerWidth='12'
            markerHeight='8'
            refX='32'
            refY='4'
            orient='auto'
          >
            <polygon points='0 0, 12 4, 0 8' className='fill-gray-400' />
          </marker>
          <marker
            id='arrowhead-critical'
            markerWidth='12'
            markerHeight='8'
            refX='32'
            refY='4'
            orient='auto'
          >
            <polygon points='0 0, 12 4, 0 8' className='fill-red-500' />
          </marker>
        </defs>

        {/* Draw edges */}
        {adjList.map((edges, from) =>
          edges.map((edge, i) => {
            const start = positions[from];
            const end = positions[edge.to];
            const isCritical =
              criticalPath?.includes(from) && criticalPath?.includes(edge.to);

            const dx = end.x - start.x;
            const dy = end.y - start.y;
            const curveOffset = Math.min(Math.abs(dx), Math.abs(dy)) * 0.2;
            const controlX = (start.x + end.x) / 2;
            const controlY = (start.y + end.y) / 2 - curveOffset;

            return (
              <g key={`edge-${from}-${edge.to}`}>
                <path
                  d={`M ${start.x} ${start.y} Q ${controlX} ${controlY} ${end.x} ${end.y}`}
                  className={`${
                    isCritical ? 'stroke-red-500' : 'stroke-gray-300'
                  } transition-colors fill-none`}
                  strokeWidth={isCritical ? '2.5' : '1.5'}
                  markerEnd={`url(#${
                    isCritical ? 'arrowhead-critical' : 'arrowhead'
                  })`}
                />
                <text
                  x={controlX}
                  y={controlY - 5}
                  className={`text-sm ${
                    isCritical ? 'fill-red-500' : 'fill-gray-600'
                  }`}
                  textAnchor='middle'
                >
                  {edge.weight}
                </text>
              </g>
            );
          })
        )}

        {/* Draw vertices */}
        {positions.map((pos, i) => {
          const isActive = i === currentVertex;
          const isCritical = criticalPath?.includes(i);
          const vertexData =
            currentStep >= 0 ? animationSteps[currentStep].vertices[i] : null;

          return (
            <g key={`node-${i}`}>
              <circle
                cx={pos.x}
                cy={pos.y}
                r='24'
                className={`${
                  isCritical
                    ? 'fill-red-100 stroke-red-500'
                    : isActive
                    ? 'fill-blue-100 stroke-blue-500'
                    : 'fill-white stroke-gray-300'
                } transition-colors`}
                strokeWidth='2'
              />
              <text
                x={pos.x}
                y={pos.y}
                className={`text-base font-medium ${
                  isCritical
                    ? 'fill-red-700'
                    : isActive
                    ? 'fill-blue-700'
                    : 'fill-gray-600'
                }`}
                textAnchor='middle'
                dominantBaseline='middle'
              >
                {String.fromCharCode(65 + i)}
              </text>
              {vertexData && (
                <>
                  <text
                    x={pos.x}
                    y={pos.y - 40}
                    className='text-xs fill-blue-600'
                    textAnchor='middle'
                  >
                    最早:{vertexData.earlyTime}
                  </text>
                  <text
                    x={pos.x}
                    y={pos.y + 40}
                    className='text-xs fill-green-600'
                    textAnchor='middle'
                  >
                    最晚:
                    {vertexData.lateTime === INFINITY
                      ? '∞'
                      : vertexData.lateTime}
                  </text>
                </>
              )}
            </g>
          );
        })}
      </svg>
    );
  };

  return (
    <div className='w-full min-h-screen bg-gray-50'>
      <div className='container mx-auto py-6'>
        <Card className='bg-white/80 p-4 md:p-6'>
          <div className='space-y-4 md:space-y-6'>
            {/* 标题区域 */}
            <div className='mb-4'>
              <h1 className='text-xl md:text-2xl font-semibold text-gray-900'>
                关键路径算法演示
              </h1>
            </div>

            {/* 主要内容区域 */}
            <div className='grid grid-cols-1 xl:grid-cols-5 gap-6'>
              {/* 左侧控制面板 */}
              <div className='xl:col-span-2 space-y-4'>
                <div>
                  <label className='block text-sm font-medium text-gray-700 mb-2'>
                    邻接表输入:
                  </label>
                  <textarea
                    value={inputText}
                    onChange={(e) => setInputText(e.target.value)}
                    className='w-full h-40 p-3 font-mono text-sm bg-gray-50 border border-gray-200 rounded-lg'
                    placeholder='格式：顶点: 邻接点1(权重1), 邻接点2(权重2)...'
                  />
                </div>

                <div className='flex flex-wrap gap-3'>
                  <Button
                    onClick={startAnimation}
                    disabled={!adjList.length || isPlaying}
                    className='flex items-center gap-2'
                  >
                    <Play className='w-4 h-4' />
                    开始演示
                  </Button>

                  <Button
                    onClick={togglePlayPause}
                    disabled={!adjList.length || currentStep === -1}
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
                    disabled={!adjList.length || currentStep === -1}
                    className='flex items-center gap-2'
                  >
                    <RotateCcw className='w-4 h-4' />
                    重置
                  </Button>
                </div>

                <div className='flex items-center gap-4'>
                  <label className='text-sm font-medium text-gray-700 whitespace-nowrap'>
                    动画速度:
                  </label>
                  <input
                    type='range'
                    min='200'
                    max='2000'
                    step='100'
                    value={animationSpeed}
                    onChange={(e) => setAnimationSpeed(Number(e.target.value))}
                    className='flex-grow'
                  />
                  <span className='text-sm text-gray-500 whitespace-nowrap'>
                    {animationSpeed}ms
                  </span>
                </div>

                {currentStep >= 0 && (
                  <Alert>
                    <AlertTitle>
                      当前步骤 ({currentStep + 1}/{animationSteps.length})
                    </AlertTitle>
                    <AlertDescription>
                      {animationSteps[currentStep].message}
                    </AlertDescription>
                  </Alert>
                )}
                {currentStep >= 0 &&
                  animationSteps[currentStep].criticalPath && (
                    <div className='bg-red-50 p-4 rounded-lg'>
                      <h3 className='font-medium text-red-900 mb-2'>
                        关键路径
                      </h3>
                      <div className='text-red-800'>
                        {animationSteps[currentStep].criticalPath
                          .map((node) => String.fromCharCode(65 + node))
                          .join(' → ')}
                      </div>
                    </div>
                  )}
              </div>

              {/* 右侧图形展示区域 */}
              <div className='xl:col-span-3'>
                <div className='relative bg-gray-50 rounded-lg p-4 border border-gray-200 min-h-[600px] w-full'>
                  {/* 缩放控制按钮 */}
                  <div className='absolute top-4 right-4 flex flex-col gap-2 z-10'>
                    <Button
                      variant='outline'
                      size='sm'
                      onClick={handleZoomIn}
                      className='bg-white/80 hover:bg-white'
                    >
                      <ZoomIn className='w-4 h-4' />
                    </Button>
                    <Button
                      variant='outline'
                      size='sm'
                      onClick={handleZoomOut}
                      className='bg-white/80 hover:bg-white'
                    >
                      <ZoomOut className='w-4 h-4' />
                    </Button>
                    <Button
                      variant='outline'
                      size='sm'
                      onClick={handleResetView}
                      className='bg-white/80 hover:bg-white'
                    >
                      <RotateCcw className='w-4 h-4' />
                    </Button>
                  </div>

                  {/* 图形容器 */}
                  <div
                    className='relative w-full h-[600px] overflow-hidden'
                    onMouseDown={handleMouseDown}
                    onMouseMove={handleMouseMove}
                    onMouseUp={handleMouseUp}
                    onMouseLeave={handleMouseUp}
                  >
                    <div
                      className='absolute inset-0'
                      style={{
                        transform: `translate(${transform.x}px, ${transform.y}px) scale(${transform.scale})`,
                        transformOrigin: '0 0',
                        transition: isDragging
                          ? 'none'
                          : 'transform 0.1s ease-out',
                      }}
                    >
                      <GraphVisualization />
                    </div>
                  </div>
                </div>
              </div>
            </div>

            {/* 算法说明区域 */}
            <div className='mt-8 grid grid-cols-1 md:grid-cols-2 gap-6'>
              <div className='space-y-4'>
                <h2 className='text-lg font-semibold text-gray-900'>
                  算法说明
                </h2>
                <div className='bg-blue-50 p-4 rounded-lg space-y-2'>
                  <h3 className='font-medium text-blue-900'>
                    关键路径算法步骤
                  </h3>
                  <ol className='list-decimal list-inside space-y-1 text-blue-800'>
                    <li>前向传播：计算每个顶点的最早开始时间</li>
                    <li>反向传播：计算每个顶点的最晚开始时间</li>
                    <li>计算时间余量：最晚开始时间减去最早开始时间</li>
                    <li>找出关键路径：时间余量为0的活动路径</li>
                  </ol>
                </div>
              </div>

              <div className='space-y-4'>
                <h2 className='text-lg font-semibold text-gray-900'>
                  概念说明
                </h2>
                <div className='bg-amber-50 p-4 rounded-lg space-y-2'>
                  <h3 className='font-medium text-amber-900'>关键概念说明</h3>
                  <ul className='list-disc list-inside space-y-1 text-amber-800'>
                    <li>最早开始时间(ES)：活动最早能够开始的时间</li>
                    <li>
                      最晚开始时间(LS)：活动最晚必须开始的时间，以不影响总工期为限
                    </li>
                    <li>时间余量：LS-ES，表示活动可以推迟的时间量</li>
                    <li>
                      关键路径：从开始到结束的最长路径，决定了项目的总工期
                    </li>
                  </ul>
                </div>
              </div>
            </div>
          </div>
        </Card>
      </div>
    </div>
  );
};

export default CriticalPathVisualizer;
