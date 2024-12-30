import React, { useState, useEffect } from 'react';
import { Alert, AlertDescription } from '~/components/ui/alert';
import { Button } from '~/components/ui/button';
import { Input } from '~/components/ui/input';
import { Card, CardContent, CardHeader, CardTitle } from '~/components/ui/card';
import { Tabs, TabsList, TabsTrigger, TabsContent } from '~/components/ui/tabs';
import { ZoomIn, ZoomOut, MoveHorizontal, Trash2, Plus } from 'lucide-react';

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
  { key: 80, name: 'Zhou', sex: 'F', age: 19 },
  { key: 5, name: 'Huang', sex: 'M', age: 20 },
  { key: 21, name: 'Zheng', sex: 'M', age: 20 },
  { key: 64, name: 'Li', sex: 'M', age: 19 },
  { key: 88, name: 'Liu', sex: 'F', age: 18 },
  { key: 13, name: 'Qian', sex: 'F', age: 19 },
  { key: 37, name: 'Sun', sex: 'M', age: 20 },
  { key: 75, name: 'Zhao', sex: 'M', age: 20 },
  { key: 92, name: 'Chen', sex: 'M', age: 20 },
];

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

const BinarySearchTree: React.FC = () => {
  const [root, setRoot] = useState<TreeNode | null>(null);
  const [nodes, setNodes] = useState<TreeNode[]>([]);
  const [edges, setEdges] = useState<Edge[]>([]);

  // 查找相关状态
  const [searchKey, setSearchKey] = useState<string>('');
  const [searchResult, setSearchResult] = useState<StudentData | null>(null);
  const [comparisons, setComparisons] = useState<number>(0);
  const [activeNode, setActiveNode] = useState<TreeNode | null>(null);
  const [isSearching, setIsSearching] = useState<boolean>(false);

  // 插入相关状态
  const [insertData, setInsertData] = useState<{
    key: string;
    name: string;
    sex: string;
    age: string;
  }>({ key: '', name: '', sex: '', age: '' });

  // 视图控制状态
  const [scale, setScale] = useState<number>(1);
  const [position, setPosition] = useState<Position>({ x: 400, y: 50 });
  const [isDragging, setIsDragging] = useState<boolean>(false);
  const [startPos, setStartPos] = useState<Position>({ x: 0, y: 0 });

  // 插入节点到BST
  const insertIntoBST = (
    root: TreeNode | null,
    data: StudentData
  ): TreeNode => {
    // 如果是空树，创建新节点作为根节点
    if (!root) {
      return new TreeNode(data);
    }

    // 递归插入到正确的位置
    if (data.key < root.data.key) {
      root.left = insertIntoBST(root.left, data);
    } else if (data.key > root.data.key) {
      root.right = insertIntoBST(root.right, data);
    }
    // 如果键值相等，不做任何操作（避免重复）
    return root;
  };

  // 删除节点
  // 删除节点
  const deleteFromBST = (
    root: TreeNode | null,
    key: number
  ): TreeNode | null => {
    if (!root) return null;

    // 递归查找要删除的节点
    if (key < root.data.key) {
      root.left = deleteFromBST(root.left, key);
    } else if (key > root.data.key) {
      root.right = deleteFromBST(root.right, key);
    } else {
      // 找到要删除的节点

      // 情况1: 叶子节点
      if (!root.left && !root.right) {
        return null;
      }

      // 情况2: 只有一个子节点
      if (!root.left) return root.right;
      if (!root.right) return root.left;

      // 情况3: 有两个子节点
      // 找到右子树中的最小值作为继承者
      let successor = findMin(root.right);
      root.data = successor.data;
      // 删除继承者的原始位置
      root.right = deleteFromBST(root.right, successor.data.key);
    }

    return root;
  };
  // 查找最小值的辅助函数
  const findMin = (node: TreeNode): TreeNode => {
    let current = node;
    while (current.left) {
      current = current.left;
    }
    return current;
  };

  // 初始化树
  useEffect(() => {
    let newRoot: TreeNode | null = null;
    for (const data of initialData) {
      newRoot = insertIntoBST(newRoot, data);
    }
    setRoot(newRoot);
  }, []);

  // 计算树的高度
  const getTreeHeight = (node: TreeNode | null): number => {
    if (!node) return 0;
    return 1 + Math.max(getTreeHeight(node.left), getTreeHeight(node.right));
  };

  // 计算节点位置
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

  // 更新可视化

  // 更新可视化
  const updateVisualization = (): void => {
    if (!root) return;

    const collectNodes = (node: TreeNode | null): TreeNode[] => {
      if (!node) return [];

      return [node, ...collectNodes(node.left), ...collectNodes(node.right)];
    };

    const allNodes = collectNodes(root);

    // 重新计算树的高度和节点位置
    const height = getTreeHeight(root);
    const width = Math.pow(2, height) * 100;

    calculateNodePositions(root, 0, 0, width);

    // 收集边
    const collectEdges = (node: TreeNode | null): Edge[] => {
      if (!node) return [];

      const edges: Edge[] = [];

      if (node.left) {
        edges.push({
          from: { x: node.x, y: node.y },
          to: { x: node.left.x, y: node.left.y },
        });
        edges.push(...collectEdges(node.left));
      }

      if (node.right) {
        edges.push({
          from: { x: node.x, y: node.y },
          to: { x: node.right.x, y: node.right.y },
        });
        edges.push(...collectEdges(node.right));
      }

      return edges;
    };

    const newEdges = collectEdges(root);

    // 更新状态
    setNodes(allNodes);
    setEdges(newEdges);
  };
  // 每次root变化时更新可视化
  useEffect(() => {
    updateVisualization();
  }, [root]);

  // 搜索函数
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

  // 处理搜索
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

  // 处理插入
  const handleInsert = () => {
    const { key, name, sex, age } = insertData;

    // 输入验证
    if (!key || !name || !sex || !age) {
      alert('请填写完整信息');
      return;
    }

    const keyNum = parseInt(key);
    if (nodes.some((node) => node.data.key === keyNum)) {
      alert('该学号已存在');
      return;
    }

    const newData: StudentData = {
      key: keyNum,
      name,
      sex,
      age: parseInt(age),
    };

    const newRoot = insertIntoBST(root, newData);
    setRoot(newRoot);
    // 立即更新可视化
    updateVisualization();

    setInsertData({ key: '', name: '', sex: '', age: '' });
  };

  // 处理删除
  // 处理删除操作
  // 修改 handleDelete 函数
  const handleDelete = (key: number) => {
    if (window.confirm('确定要删除这条记录吗？')) {
      const newRoot = deleteFromBST(root, key);
      setRoot(newRoot);
      // 立即更新可视化
      updateVisualization();
    }
  };
  // 视图控制函数
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
          {/* 搜索区域 */}
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

          {/* 搜索结果 */}
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

          {/* 插入区域 */}
          <div className='flex gap-4 mb-6'>
            <Input
              type='number'
              placeholder='学号'
              value={insertData.key}
              onChange={(e) =>
                setInsertData((prev) => ({ ...prev, key: e.target.value }))
              }
              className='w-24'
            />
            <Input
              type='text'
              placeholder='姓名'
              value={insertData.name}
              onChange={(e) =>
                setInsertData((prev) => ({ ...prev, name: e.target.value }))
              }
              className='w-24'
            />
            <Input
              type='text'
              placeholder='性别'
              value={insertData.sex}
              onChange={(e) =>
                setInsertData((prev) => ({ ...prev, sex: e.target.value }))
              }
              className='w-24'
            />
            <Input
              type='number'
              placeholder='年龄'
              value={insertData.age}
              onChange={(e) =>
                setInsertData((prev) => ({ ...prev, age: e.target.value }))
              }
              className='w-24'
            />
            <Button onClick={handleInsert}>
              <Plus className='mr-2 h-4 w-4' /> 插入
            </Button>
          </div>

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
                      <th className='px-4 py-2 text-left'>操作</th>
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
                        <td className='px-4 py-2'>
                          <Button
                            variant='destructive'
                            size='sm'
                            onClick={() => handleDelete(node.data.key)}
                          >
                            <Trash2 className='h-4 w-4' />
                          </Button>
                        </td>
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
