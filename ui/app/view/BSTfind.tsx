import React, { useState, useCallback, useEffect } from 'react';
import { Alert, AlertDescription } from '~/components/ui/alert';
import { Button } from '~/components/ui/button';
import { Input } from '~/components/ui/input';
import { Card, CardContent, CardHeader, CardTitle } from '~/components/ui/card';
import { Tabs, TabsList, TabsTrigger, TabsContent } from '~/components/ui/tabs';
import { ZoomIn, ZoomOut, MoveHorizontal } from 'lucide-react';

interface StudentData {
  key: number;
  name: string;
  sex: string;
  age: number;
}

interface Position {
  x: number;
  y: number;
}

interface Edge {
  from: Position;
  to: Position;
}

// Initial data remains the same
const initialData: StudentData[] = [
  { key: 56, name: 'Zhang', sex: 'F', age: 19 },
  { key: 19, name: 'Wang', sex: 'F', age: 20 },
  // ... rest of the data
].sort((a, b) => a.key - b.key);

class TreeNode {
  data: StudentData;
  left: TreeNode | null;
  right: TreeNode | null;
  x: number;
  y: number;
  level: number;

  constructor(data: StudentData) {
    this.data = data;
    this.left = null;
    this.right = null;
    this.x = 0;
    this.y = 0;
    this.level = 0;
  }
}

const getTreeHeight = (node: TreeNode | null): number => {
  if (!node) return 0;
  return 1 + Math.max(getTreeHeight(node.left), getTreeHeight(node.right));
};

const buildBalancedBST = (
  sortedData: StudentData[],
  start: number = 0,
  end: number = sortedData.length - 1
): TreeNode | null => {
  if (start > end) return null;

  const mid = Math.floor((start + end) / 2);
  const node = new TreeNode(sortedData[mid]);

  node.left = buildBalancedBST(sortedData, start, mid - 1);
  node.right = buildBalancedBST(sortedData, mid + 1, end);

  return node;
};

const BinarySearchTree: React.FC = () => {
  const [root, setRoot] = useState<TreeNode | null>(null);
  const [nodes, setNodes] = useState<TreeNode[]>([]);
  const [edges, setEdges] = useState<Edge[]>([]);
  const [searchKey, setSearchKey] = useState<string>('');
  const [searchResult, setSearchResult] = useState<StudentData | null>(null);
  const [comparisons, setComparisons] = useState<number>(0);
  const [activeNode, setActiveNode] = useState<TreeNode | null>(null);
  const [isSearching, setIsSearching] = useState<boolean>(false);
  const [scale, setScale] = useState<number>(1);
  const [position, setPosition] = useState<Position>({ x: 400, y: 50 });
  const [isDragging, setIsDragging] = useState<boolean>(false);
  const [startPos, setStartPos] = useState<Position>({ x: 0, y: 0 });

  const calculateNodePositions = (
    node: TreeNode | null,
    level: number = 0,
    minX: number = 0,
    maxX: number = 1200
  ): void => {
    if (!node) return;

    const width = maxX - minX;
    const x = minX + width / 2;
    const y = level * 100;

    node.x = x;
    node.y = y;
    node.level = level;

    if (node.left) {
      calculateNodePositions(node.left, level + 1, minX, x - 20);
    }
    if (node.right) {
      calculateNodePositions(node.right, level + 1, x + 20, maxX);
    }
  };

  const updateVisualization = (): void => {
    if (!root) return;

    const height = getTreeHeight(root);
    const width = Math.pow(2, height) * 100;

    calculateNodePositions(root, 0, 0, width);

    const newNodes: TreeNode[] = [];
    const newEdges: Edge[] = [];

    const queue: TreeNode[] = [root];
    while (queue.length > 0) {
      const node = queue.shift();
      if (!node) continue;

      newNodes.push(node);

      if (node.left) {
        newEdges.push({
          from: { x: node.x, y: node.y },
          to: { x: node.left.x, y: node.left.y },
        });
        queue.push(node.left);
      }

      if (node.right) {
        newEdges.push({
          from: { x: node.x, y: node.y },
          to: { x: node.right.x, y: node.right.y },
        });
        queue.push(node.right);
      }
    }

    setNodes(newNodes);
    setEdges(newEdges);
  };

  useEffect(() => {
    const newRoot = buildBalancedBST(initialData);
    setRoot(newRoot);
  }, []);

  useEffect(() => {
    updateVisualization();
  }, [root, activeNode]);

  const searchBST = async (
    node: TreeNode | null,
    key: number
  ): Promise<TreeNode | null> => {
    if (!node) return null;

    setActiveNode(node);
    setComparisons((prev) => prev + 1);
    await new Promise((resolve) => setTimeout(resolve, 500));

    if (key === node.data.key) {
      setSearchResult(node.data);
      return node;
    }

    if (key < node.data.key) {
      return searchBST(node.left, key);
    } else {
      return searchBST(node.right, key);
    }
  };

  const handleSearch = async (): Promise<void> => {
    if (!searchKey || isSearching) return;

    setIsSearching(true);
    setSearchResult(null);
    setComparisons(0);
    setActiveNode(null);

    const keyNum = parseInt(searchKey);
    await searchBST(root, keyNum);
    setIsSearching(false);
  };

  const handleMouseDown = (e: React.MouseEvent): void => {
    setIsDragging(true);
    setStartPos({
      x: e.clientX - position.x,
      y: e.clientY - position.y,
    });
  };

  const handleMouseMove = (e: React.MouseEvent): void => {
    if (!isDragging) return;
    setPosition({
      x: e.clientX - startPos.x,
      y: e.clientY - startPos.y,
    });
  };

  const handleMouseUp = (): void => {
    setIsDragging(false);
  };

  const handleZoom = (direction: 'in' | 'out'): void => {
    setScale((prev) => {
      if (direction === 'in' && prev < 2) return prev + 0.1;
      if (direction === 'out' && prev > 0.5) return prev - 0.1;
      return prev;
    });
  };

  const resetView = (): void => {
    setScale(1);
    setPosition({ x: 400, y: 50 });
  };

  return (
    <div className='container mx-auto p-4'>
      <Card className='w-full'>
        <CardHeader>
          <CardTitle>二叉搜索树演示</CardTitle>
        </CardHeader>
        <CardContent>
          <div className='flex gap-4 mb-6'>
            <Input
              type='number'
              placeholder='请输入学号查找'
              value={searchKey}
              onChange={(e) => setSearchKey(e.target.value)}
              className='w-64'
              disabled={isSearching}
            />
            <Button onClick={handleSearch} disabled={isSearching}>
              {isSearching ? '查找中...' : '查找'}
            </Button>
          </div>

          {searchResult && (
            <Alert className='mb-4'>
              <AlertDescription>
                <div className='font-semibold'>查找结果:</div>
                <div>学号: {searchResult.key}</div>
                <div>姓名: {searchResult.name}</div>
                <div>性别: {searchResult.sex}</div>
                <div>年龄: {searchResult.age}</div>
                <div>比较次数: {comparisons}</div>
              </AlertDescription>
            </Alert>
          )}

          {!searchResult && searchKey && !isSearching && (
            <Alert className='mb-4' variant='destructive'>
              <AlertDescription>
                未找到匹配记录 (比较次数: {comparisons})
              </AlertDescription>
            </Alert>
          )}

          <Tabs defaultValue='table'>
            <TabsList>
              <TabsTrigger value='table'>表格视图</TabsTrigger>
              <TabsTrigger value='tree'>树形视图</TabsTrigger>
            </TabsList>

            <TabsContent value='table'>
              <div className='overflow-x-auto'>
                <table className='w-full'>
                  <thead>
                    <tr className='bg-gray-50'>
                      <th className='px-4 py-2 text-left'>学号</th>
                      <th className='px-4 py-2 text-left'>姓名</th>
                      <th className='px-4 py-2 text-left'>性别</th>
                      <th className='px-4 py-2 text-left'>年龄</th>
                    </tr>
                  </thead>
                  <tbody>
                    {nodes.map((node) => (
                      <tr
                        key={node.data.key}
                        className={`border-b hover:bg-gray-50 
                          ${node === activeNode ? 'bg-yellow-200' : ''}`}
                      >
                        <td className='px-4 py-2'>{node.data.key}</td>
                        <td className='px-4 py-2'>{node.data.name}</td>
                        <td className='px-4 py-2'>{node.data.sex}</td>
                        <td className='px-4 py-2'>{node.data.age}</td>
                      </tr>
                    ))}
                  </tbody>
                </table>
              </div>
            </TabsContent>

            <TabsContent value='tree'>
              <div className='flex gap-2 mb-4'>
                <Button variant='outline' onClick={() => handleZoom('in')}>
                  <ZoomIn className='w-4 h-4' />
                </Button>
                <Button variant='outline' onClick={() => handleZoom('out')}>
                  <ZoomOut className='w-4 h-4' />
                </Button>
                <Button variant='outline' onClick={resetView}>
                  <MoveHorizontal className='w-4 h-4' />
                </Button>
                <span className='text-sm text-gray-500 leading-9 ml-2'>
                  提示：按住鼠标拖动可平移视图
                </span>
              </div>

              <div
                className='relative w-full h-96 border rounded-lg overflow-hidden bg-white cursor-move'
                onMouseDown={handleMouseDown}
                onMouseMove={handleMouseMove}
                onMouseUp={handleMouseUp}
                onMouseLeave={handleMouseUp}
              >
                <div
                  className='absolute inset-0'
                  style={{
                    transform: `translate(${position.x}px, ${position.y}px) scale(${scale})`,
                  }}
                >
                  <svg className='absolute' width='2400' height='1200'>
                    {edges.map((edge, index) => (
                      <line
                        key={index}
                        x1={edge.from.x}
                        y1={edge.from.y}
                        x2={edge.to.x}
                        y2={edge.to.y}
                        stroke='#333'
                        strokeWidth='2'
                      />
                    ))}
                  </svg>

                  {nodes.map((node) => (
                    <div
                      key={node.data.key}
                      className={`absolute p-2 rounded-lg border-2 
                        ${
                          node === activeNode
                            ? 'bg-yellow-200 border-yellow-500'
                            : 'bg-white border-blue-500'
                        } 
                        transition-colors duration-300`}
                      style={{
                        left: node.x - 30,
                        top: node.y - 20,
                      }}
                    >
                      <div className='text-sm font-semibold'>
                        {node.data.key}
                      </div>
                      <div className='text-xs'>{node.data.name}</div>
                    </div>
                  ))}
                </div>
              </div>
            </TabsContent>
          </Tabs>
        </CardContent>
      </Card>
    </div>
  );
};

export default BinarySearchTree;
