import React, { useState, useCallback } from 'react';

interface Student {
  key: number;
  name: string;
  sex: string;
  age: number;
}

const initialData: Student[] = [
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

const ANIMATION_SPEED = 500; // 动画速度(毫秒)

const SearchTable = () => {
  const [data, setData] = useState<Student[]>(initialData);
  const [searchKey, setSearchKey] = useState('');
  const [searchResult, setSearchResult] = useState<Student | null>(null);
  const [comparisons, setComparisons] = useState(0);
  const [searchType, setSearchType] = useState<'sequential' | 'binary'>(
    'sequential'
  );
  const [isSorted, setIsSorted] = useState(false);
  const [currentIndex, setCurrentIndex] = useState(-1);
  const [isSearching, setIsSearching] = useState(false);
  const [searchPath, setSearchPath] = useState<number[]>([]);

  // 重置搜索状态
  const resetSearch = () => {
    setSearchResult(null);
    setComparisons(0);
    setCurrentIndex(-1);
    setSearchPath([]);
  };

  // 顺序查找
  const sequentialSearch = useCallback(
    async (key: string) => {
      setIsSearching(true);
      resetSearch();
      const keyInt = parseInt(key);

      for (let i = 0; i < data.length; i++) {
        setCurrentIndex(i);
        setComparisons((prev) => prev + 1);

        // 等待动画时间
        await new Promise((resolve) => setTimeout(resolve, ANIMATION_SPEED));

        if (data[i].key === keyInt) {
          setSearchResult(data[i]);
          setIsSearching(false);
          return;
        }
      }

      setSearchResult(null);
      setIsSearching(false);
    },
    [data]
  );

  // 折半查找
  const binarySearch = useCallback(
    async (key: string) => {
      if (!isSorted) {
        sortTable();
      }

      setIsSearching(true);
      resetSearch();
      const keyInt = parseInt(key);
      let left = 0;
      let right = data.length - 1;
      const newSearchPath: number[] = [];

      while (left <= right) {
        const mid = Math.floor((left + right) / 2);
        setCurrentIndex(mid);
        setComparisons((prev) => prev + 1);
        newSearchPath.push(mid);
        setSearchPath(newSearchPath);

        // 等待动画时间
        await new Promise((resolve) => setTimeout(resolve, ANIMATION_SPEED));

        if (data[mid].key === keyInt) {
          setSearchResult(data[mid]);
          setIsSearching(false);
          return;
        }

        if (data[mid].key < keyInt) {
          left = mid + 1;
        } else {
          right = mid - 1;
        }
      }

      setSearchResult(null);
      setIsSearching(false);
    },
    [data, isSorted]
  );

  // 排序
  const sortTable = () => {
    const sortedData = [...data].sort((a, b) => a.key - b.key);
    setData(sortedData);
    setIsSorted(true);
  };

  // 处理搜索
  const handleSearch = () => {
    if (!searchKey || isSearching) return;
    if (searchType === 'sequential') {
      sequentialSearch(searchKey);
    } else {
      binarySearch(searchKey);
    }
  };

  // 获取行的背景色样式
  const getRowStyle = (index: number) => {
    if (index === currentIndex) {
      return 'bg-yellow-200 transition-colors duration-300';
    }
    if (searchType === 'binary' && searchPath.includes(index)) {
      return 'bg-blue-50';
    }
    return 'hover:bg-gray-50';
  };

  return (
    <div className='container mx-auto p-4'>
      <div className='bg-white rounded-lg shadow-md mb-8'>
        <div className='p-6'>
          <h2 className='text-2xl font-bold mb-4'>静态查找表</h2>

          <div className='flex flex-col md:flex-row gap-4 mb-6'>
            <input
              type='number'
              placeholder='请输入学号'
              value={searchKey}
              onChange={(e) => setSearchKey(e.target.value)}
              className='border rounded px-3 py-2 w-full md:w-64 focus:outline-none focus:ring-2 focus:ring-blue-500 bg-white text-gray-900'
              disabled={isSearching}
            />

            <select
              className='border rounded px-3 py-2 focus:outline-none focus:ring-2 focus:ring-blue-500 bg-white text-gray-900'
              value={searchType}
              onChange={(e) =>
                setSearchType(e.target.value as 'sequential' | 'binary')
              }
              disabled={isSearching}
            >
              <option value='sequential'>顺序查找</option>
              <option value='binary'>折半查找</option>
            </select>

            <button
              onClick={handleSearch}
              disabled={isSearching}
              className={`${
                isSearching ? 'bg-gray-400' : 'bg-blue-500 hover:bg-blue-600'
              } text-white rounded px-4 py-2 focus:outline-none focus:ring-2 focus:ring-blue-500`}
            >
              {isSearching ? '查找中...' : '查找'}
            </button>

            <button
              onClick={sortTable}
              disabled={isSearching}
              className='border border-gray-300 rounded px-4 py-2 hover:bg-gray-50 focus:outline-none focus:ring-2 focus:ring-blue-500 whitespace-nowrap disabled:bg-gray-100 bg-white text-gray-900'
            >
              按学号排序
            </button>
          </div>

          {searchResult && (
            <div className='mb-4 p-4 bg-green-50 rounded border border-green-200'>
              <h3 className='font-bold mb-2'>查找结果:</h3>
              <p>学号: {searchResult.key}</p>
              <p>姓名: {searchResult.name}</p>
              <p>性别: {searchResult.sex}</p>
              <p>年龄: {searchResult.age}</p>
              <p>比较次数: {comparisons}</p>
            </div>
          )}

          {!searchResult && searchKey && !isSearching && (
            <div className='mb-4 p-4 bg-red-50 rounded border border-red-200'>
              <p>未找到匹配记录</p>
              <p>比较次数: {comparisons}</p>
            </div>
          )}

          <div className='overflow-x-auto'>
            <table className='w-full border-collapse table-auto'>
              <caption className='text-lg font-semibold mb-2'>
                学生信息表
                {isSearching && (
                  <span className='text-sm font-normal text-gray-500 ml-2'>
                    (正在比较第 {comparisons} 次)
                  </span>
                )}
              </caption>
              <thead>
                <tr className='bg-gray-50 border-b'>
                  <th className='px-4 py-2 text-left'>学号</th>
                  <th className='px-4 py-2 text-left'>姓名</th>
                  <th className='px-4 py-2 text-left'>性别</th>
                  <th className='px-4 py-2 text-left'>年龄</th>
                </tr>
              </thead>
              <tbody>
                {data.map((item, index) => (
                  <tr
                    key={item.key}
                    className={`border-b transition-all ${getRowStyle(index)}`}
                  >
                    <td className='px-4 py-2'>{item.key}</td>
                    <td className='px-4 py-2'>{item.name}</td>
                    <td className='px-4 py-2'>{item.sex}</td>
                    <td className='px-4 py-2'>{item.age}</td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        </div>
      </div>
    </div>
  );
};

export default SearchTable;
