import React, { useState, useEffect } from 'react';
import { Alert, AlertDescription } from '~/components/ui/alert';
import { Button } from '~/components/ui/button';
import { Input } from '~/components/ui/input';
import { Card, CardContent, CardHeader, CardTitle } from '~/components/ui/card';
import { Tabs, TabsList, TabsTrigger, TabsContent } from '~/components/ui/tabs';

interface HashTableItem {
  key: number;
  isActive: boolean;
}

interface LinkedItem {
  key: number;
  isActive: boolean;
}

interface HashChain {
  items: LinkedItem[];
  isActive: boolean;
}

const HASH_SIZE = 13;
const TABLE_SIZE = 16;
const ANIMATION_SPEED = 500;

const initialKeys = [19, 14, 23, 1, 68, 20, 84, 27, 55, 11, 10, 79];

const HashTableVisual: React.FC = () => {
  // 状态定义
  const [searchKey, setSearchKey] = useState('');
  const [isSearching, setIsSearching] = useState(false);
  const [comparisons, setComparisons] = useState(0);
  const [activeIndex, setActiveIndex] = useState(-1);
  const [currentMethod, setCurrentMethod] = useState<'linear' | 'chain'>(
    'linear'
  );
  const handleReset = () => {
    setSearchKey('');
    setIsSearching(false);
    setComparisons(0);
    setActiveIndex(-1);
    initLinearTable();
    initChainTable();
  };
  // 线性探测法状态
  const [linearTable, setLinearTable] = useState<HashTableItem[]>(
    Array(TABLE_SIZE)
      .fill(null)
      .map(() => ({ key: -1, isActive: false }))
  );

  // 链地址法状态
  const [chainTable, setChainTable] = useState<HashChain[]>(
    Array(HASH_SIZE)
      .fill(null)
      .map(() => ({ items: [], isActive: false }))
  );

  // 哈希函数
  const hash = (key: number): number => {
    return key % HASH_SIZE;
  };

  // 初始化线性探测哈希表
  const initLinearTable = () => {
    const newTable = Array(TABLE_SIZE)
      .fill(null)
      .map(() => ({ key: -1, isActive: false }));

    initialKeys.forEach((key) => {
      let index = hash(key);
      while (newTable[index].key !== -1) {
        index = (index + 1) % TABLE_SIZE;
      }
      newTable[index] = { key, isActive: false };
    });

    setLinearTable(newTable);
  };

  // 初始化链地址法哈希表
  const initChainTable = () => {
    const newTable: HashChain[] = Array(HASH_SIZE) // 注意这里使用 HASH_SIZE 而不是 TABLE_SIZE
      .fill(null)
      .map(() => ({
        items: [], // HashChain 类型需要 items 数组
        isActive: false,
      }));

    initialKeys.forEach((key) => {
      const index = hash(key);
      newTable[index].items.unshift({ key, isActive: false });
    });

    setChainTable(newTable);
  };

  // 线性探测法查找
  const searchLinear = async (key: number) => {
    setIsSearching(true);
    setComparisons(0);

    let index = hash(key);
    let count = 0;

    while (count < TABLE_SIZE) {
      setActiveIndex(index);
      setComparisons((prev) => prev + 1);

      await new Promise((resolve) => setTimeout(resolve, ANIMATION_SPEED));

      if (linearTable[index].key === -1) break;
      if (linearTable[index].key === key) {
        const newTable = [...linearTable];
        newTable[index].isActive = true;
        setLinearTable(newTable);
        setIsSearching(false);
        return true;
      }

      index = (index + 1) % TABLE_SIZE;
      count++;
    }

    setIsSearching(false);
    return false;
  };

  // 链地址法查找
  const searchChain = async (key: number) => {
    setIsSearching(true);
    setComparisons(0);

    const index = hash(key);
    setActiveIndex(index);

    const chain = chainTable[index];

    for (let i = 0; i < chain.items.length; i++) {
      setComparisons((prev) => prev + 1);
      await new Promise((resolve) => setTimeout(resolve, ANIMATION_SPEED));

      if (chain.items[i].key === key) {
        const newTable = [...chainTable];
        newTable[index].items[i].isActive = true;
        setChainTable(newTable);
        setIsSearching(false);
        return true;
      }
    }

    setIsSearching(false);
    return false;
  };

  // 处理查找
  const handleSearch = async () => {
    if (!searchKey || isSearching) return;

    const key = parseInt(searchKey);

    // 重置高亮状态
    if (currentMethod === 'linear') {
      setLinearTable((prev) =>
        prev.map((item) => ({ ...item, isActive: false }))
      );
      await searchLinear(key);
    } else {
      setChainTable((prev) =>
        prev.map((chain) => ({
          ...chain,
          items: chain.items.map((item) => ({ ...item, isActive: false })),
        }))
      );
      await searchChain(key);
    }
  };

  // 初始化
  useEffect(() => {
    initLinearTable();
    initChainTable();
  }, []);

  return (
    <div className='container mx-auto p-4'>
      <Card className='w-full'>
        <CardHeader>
          <CardTitle>哈希表查找演示</CardTitle>
        </CardHeader>
        <CardContent>
          <div className='flex gap-4 mb-6'>
            <Input
              type='number'
              placeholder='请输入关键字查找'
              value={searchKey}
              onChange={(e) => setSearchKey(e.target.value)}
              className='w-64'
              disabled={isSearching}
            />

            <Button onClick={handleSearch} disabled={isSearching}>
              {isSearching ? '查找中...' : '查找'}
            </Button>

            <Button
              onClick={handleReset}
              disabled={isSearching}
              variant='outline'
            >
              重置
            </Button>
          </div>

          <Alert className='mb-4'>
            <AlertDescription>比较次数: {comparisons}</AlertDescription>
          </Alert>

          <Tabs
            value={currentMethod}
            onValueChange={(v) => setCurrentMethod(v as 'linear' | 'chain')}
          >
            <TabsList>
              <TabsTrigger value='linear'>线性探测再散列</TabsTrigger>
              <TabsTrigger value='chain'>链地址法</TabsTrigger>
            </TabsList>

            <TabsContent value='linear'>
              <div className='grid grid-cols-8 gap-2'>
                {linearTable.map((item, index) => (
                  <div
                    key={index}
                    className={`p-4 border-2 rounded-lg text-center transition-colors duration-300
                      ${index === activeIndex ? 'bg-yellow-100' : ''}
                      ${item.isActive ? 'bg-green-100' : ''}
                      ${
                        item.key === -1 ? 'border-gray-200' : 'border-blue-500'
                      }`}
                  >
                    <div className='font-bold'>{index}</div>
                    <div>{item.key === -1 ? '-' : item.key}</div>
                  </div>
                ))}
              </div>
            </TabsContent>

            <TabsContent value='chain'>
              <div className='space-y-2'>
                {chainTable.map((chain, index) => (
                  <div key={index} className='flex items-center gap-2'>
                    <div
                      className={`w-12 h-12 flex items-center justify-center border-2 rounded-lg
                        ${index === activeIndex ? 'bg-yellow-100' : ''}
                        ${chain.isActive ? 'bg-green-100' : ''}`}
                    >
                      {index}
                    </div>
                    {chain.items.length > 0 ? (
                      <div className='flex items-center'>
                        <div className='w-8 h-px bg-gray-400'></div>
                        {chain.items.map((item, itemIndex) => (
                          <React.Fragment key={itemIndex}>
                            <div
                              className={`p-2 border-2 rounded-lg
                                ${
                                  item.isActive
                                    ? 'bg-green-100 border-green-500'
                                    : 'border-blue-500'
                                }`}
                            >
                              {item.key}
                            </div>
                            {itemIndex < chain.items.length - 1 && (
                              <div className='w-8 h-px bg-gray-400'></div>
                            )}
                          </React.Fragment>
                        ))}
                      </div>
                    ) : null}
                  </div>
                ))}
              </div>
            </TabsContent>
          </Tabs>
        </CardContent>
      </Card>

      {/* 添加算法说明部分 */}
      <Card className='mt-8'>
        <CardHeader>
          <CardTitle>算法说明</CardTitle>
        </CardHeader>
        <CardContent>
          <Tabs value={currentMethod}>
            <TabsList>
              <TabsTrigger value='linear'>线性探测再散列</TabsTrigger>
              <TabsTrigger value='chain'>链地址法</TabsTrigger>
            </TabsList>

            <TabsContent value='linear'>
              <div className='bg-blue-50 p-4 rounded-lg space-y-4'>
                <div>
                  <h3 className='font-medium text-blue-900 mb-2'>基本原理</h3>
                  <p className='text-blue-800'>
                    当发生冲突时，按顺序查找下一个空位置，直到找到一个空位置为止。
                    查找时也按照同样的顺序查找，直到找到关键字或遇到空位置。
                  </p>
                </div>

                <div>
                  <h3 className='font-medium text-blue-900 mb-2'>算法步骤</h3>
                  <ol className='list-decimal list-inside space-y-1 text-blue-800'>
                    <li>计算哈希值：key % {HASH_SIZE}</li>
                    <li>如果该位置为空(-1)，则查找失败</li>
                    <li>如果该位置的关键字等于查找关键字，则查找成功</li>
                    <li>否则，探测下一个位置：(当前位置 + 1) % 表长</li>
                    <li>重复步骤2-4，直到找到或表满</li>
                  </ol>
                </div>

                <div>
                  <h3 className='font-medium text-blue-900 mb-2'>性能分析</h3>
                  <ul className='list-disc list-inside space-y-1 text-blue-800'>
                    <li>优点：实现简单，节省空间</li>
                    <li>缺点：容易产生聚集现象</li>
                    <li>查找成功时ASL取决于关键字的位置</li>
                    <li>查找不成功时需要探测到空位置</li>
                  </ul>
                </div>
              </div>
            </TabsContent>

            <TabsContent value='chain'>
              <div className='bg-purple-50 p-4 rounded-lg space-y-4'>
                <div>
                  <h3 className='font-medium text-purple-900 mb-2'>基本原理</h3>
                  <p className='text-purple-800'>
                    将所有关键字为同一哈希值的记录存储在同一线性链表中。
                    查找时先计算哈希值，然后在对应链表中查找关键字。
                  </p>
                </div>

                <div>
                  <h3 className='font-medium text-purple-900 mb-2'>算法步骤</h3>
                  <ol className='list-decimal list-inside space-y-1 text-purple-800'>
                    <li>计算哈希值：key % {HASH_SIZE}</li>
                    <li>获取该位置的链表头指针</li>
                    <li>在链表中顺序查找关键字</li>
                    <li>找到则查找成功，否则查找失败</li>
                  </ol>
                </div>

                <div>
                  <h3 className='font-medium text-purple-900 mb-2'>性能分析</h3>
                  <ul className='list-disc list-inside space-y-1 text-purple-800'>
                    <li>优点：不会出现聚集，删除操作容易</li>
                    <li>缺点：需要额外的链表空间</li>
                    <li>查找成功时ASL取决于同义词的个数</li>
                    <li>查找不成功时只需要搜索对应链表</li>
                  </ul>
                </div>

                <div>
                  <h3 className='font-medium text-purple-900 mb-2'>补充说明</h3>
                  <ul className='list-disc list-inside space-y-1 text-purple-800'>
                    <li>使用头插法插入新节点到链表</li>
                    <li>链表中存储的是具有相同哈希值的关键字</li>
                    <li>链表越短，查找效率越高</li>
                  </ul>
                </div>
              </div>
            </TabsContent>
          </Tabs>
        </CardContent>
      </Card>
    </div>
  );
};

export default HashTableVisual;
