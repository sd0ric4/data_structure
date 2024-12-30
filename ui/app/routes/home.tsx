import { useTransition } from '~/context/TransitionProvider';
import { Card, CardContent } from '~/components/ui/card';
import { useEffect, useRef, useState } from 'react';
import {
  Grid2x2,
  Target,
  ListTree,
  Network,
  Search,
  Binary,
  Hash,
  GitGraph,
} from 'lucide-react';

interface Route {
  path: string;
  title: string;
  icon: React.ReactNode;
  description: string;
}

const NavHome = () => {
  const { navigateTo, goBack, isTransitioning } = useTransition();
  const [selectedIndex, setSelectedIndex] = useState(0);
  const cardRefs = useRef<(HTMLDivElement | null)[]>([]);

  const routes: Route[] = [
    {
      path: '/mgraph',
      title: '邻接矩阵可视化图',
      icon: <Grid2x2 className='w-8 h-8' />,
      description: '邻接矩阵可视化图演示',
    },
    {
      path: '/algraph',
      title: '图遍历动画',
      icon: <Target className='w-8 h-8' />,
      description: '图遍历动画演示',
    },
    {
      path: '/fgraph',
      title: '邻接表可视化图',
      icon: <ListTree className='w-8 h-8' />,
      description: '邻接表可视化图',
    },
    {
      path: '/cpagraph',
      title: '关键路径算法',
      icon: <GitGraph className='w-8 h-8' />,
      description: '关键路径算法可视化演示',
    },
    {
      path: '/mstgraph',
      title: '最小生成树',
      icon: <Network className='w-8 h-8' />,
      description: '最小生成树算法演示',
    },
    {
      path: '/find',
      title: '查找',
      icon: <Search className='w-8 h-8' />,
      description: '查找算法的基本操作',
    },
    {
      path: '/bstfind',
      title: '二叉搜索树',
      icon: <Binary className='w-8 h-8' />,
      description: '二叉搜索树的操作与查找',
    },
    {
      path: '/hashtable',
      title: '哈希表',
      icon: <Hash className='w-8 h-8' />,
      description: '哈希表的实现与查找演示',
    },
  ];

  useEffect(() => {
    const handleKeyDown = (event: KeyboardEvent) => {
      if (isTransitioning) return;

      const cols =
        window.innerWidth >= 1024 ? 3 : window.innerWidth >= 768 ? 2 : 1;
      const rows = Math.ceil(routes.length / cols);

      switch (event.key) {
        case 'ArrowRight':
          setSelectedIndex((prev) => (prev + 1) % routes.length);
          break;
        case 'ArrowLeft':
          setSelectedIndex(
            (prev) => (prev - 1 + routes.length) % routes.length
          );
          break;
        case 'ArrowUp':
          setSelectedIndex((prev) => {
            const newIndex = prev - cols;
            return newIndex < 0 ? prev + (rows * cols - cols) : newIndex;
          });
          break;
        case 'ArrowDown':
          setSelectedIndex((prev) => {
            const newIndex = prev + cols;
            return newIndex >= routes.length ? prev % cols : newIndex;
          });
          break;
        case 'Enter':
        case ' ':
          if (!isTransitioning) {
            navigateTo(routes[selectedIndex].path);
          }
          event.preventDefault();
          break;
      }
    };

    window.addEventListener('keydown', handleKeyDown);
    return () => window.removeEventListener('keydown', handleKeyDown);
  }, [navigateTo, isTransitioning, routes.length, selectedIndex]);

  useEffect(() => {
    cardRefs.current[selectedIndex]?.focus();
  }, [selectedIndex]);

  return (
    <div className='min-h-screen bg-gradient-to-br from-blue-50 to-indigo-50 p-8'>
      <div className='max-w-7xl mx-auto'>
        <h1 className='text-4xl font-bold text-center mb-12 text-gray-800'>
          数据结构与算法可视化
        </h1>

        <div className='grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6'>
          {routes.map((route, index) => (
            <Card
              key={route.path}
              ref={(el: HTMLDivElement | null) => {
                cardRefs.current[index] = el;
              }}
              className={`group transition-all duration-300 cursor-pointer border-2 outline-none
                ${
                  index === selectedIndex
                    ? 'border-blue-500 ring-2 ring-blue-300 shadow-lg'
                    : 'hover:border-blue-500 hover:shadow-lg'
                }`}
              onClick={() => !isTransitioning && navigateTo(route.path)}
              onFocus={() => setSelectedIndex(index)}
              tabIndex={0}
            >
              <CardContent className='p-6 flex flex-col items-center text-center'>
                <div
                  className={`mb-4 transform transition-transform duration-300
                  ${
                    index === selectedIndex
                      ? 'text-blue-500 scale-110'
                      : 'text-gray-600 group-hover:text-blue-500 group-hover:scale-110'
                  }`}
                >
                  {route.icon}
                </div>
                <h2 className='text-xl font-semibold mb-2 text-gray-800'>
                  {route.title}
                </h2>
                <p className='text-gray-600'>{route.description}</p>
              </CardContent>
            </Card>
          ))}
        </div>
      </div>
    </div>
  );
};

export default NavHome;
