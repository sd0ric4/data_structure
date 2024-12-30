import {
  createContext,
  useContext,
  useState,
  useEffect,
  type ReactNode,
} from 'react';
import { useLocation, useNavigate } from 'react-router';
import '../styles/transition.css';

interface TransitionContextType {
  isTransitioning: boolean;
  navigateTo: (to: string) => void;
  goBack: () => void;
}

const TransitionContext = createContext<TransitionContextType>({
  isTransitioning: false,
  navigateTo: () => {},
  goBack: () => {},
});

export const useTransition = () => useContext(TransitionContext);

export function TransitionProvider({ children }: { children: ReactNode }) {
  const [isTransitioning, setIsTransitioning] = useState(false);
  const [pageState, setPageState] = useState<'active' | 'leave' | 'enter'>(
    'active'
  );
  const navigate = useNavigate();
  const location = useLocation();
  const [history, setHistory] = useState<string[]>([location.pathname]);

  useEffect(() => {
    // 更新历史记录
    if (!history.includes(location.pathname)) {
      setHistory((prev) => [...prev, location.pathname]);
    }
  }, [location.pathname]);

  useEffect(() => {
    const handleKeyDown = async (event: KeyboardEvent) => {
      if (event.key === 'Escape' && !isTransitioning && history.length > 1) {
        event.preventDefault();
        goBack();
      }
    };

    window.addEventListener('keydown', handleKeyDown);
    return () => window.removeEventListener('keydown', handleKeyDown);
  }, [isTransitioning, history]);

  const performTransition = async (navigationFn: () => void) => {
    if (isTransitioning) return;

    setIsTransitioning(true);
    setPageState('leave');

    // 等待离开动画完成
    await new Promise((resolve) => setTimeout(resolve, 400));

    navigationFn();
    setPageState('enter');

    // 等待进入动画开始
    await new Promise((resolve) => setTimeout(resolve, 50));

    setPageState('active');

    // 等待进入动画完成
    await new Promise((resolve) => setTimeout(resolve, 400));
    setIsTransitioning(false);
  };

  const navigateTo = async (to: string) => {
    if (to === location.pathname) return;
    await performTransition(() => navigate(to));
  };

  const goBack = async () => {
    if (history.length <= 1) return;
    await performTransition(() => {
      navigate(-1);
      setHistory((prev) => prev.slice(0, -1));
    });
  };

  return (
    <TransitionContext.Provider value={{ isTransitioning, navigateTo, goBack }}>
      <div className={`page-container page-${pageState}`}>
        <div className='page-outer'>
          <div className='page-inner'>{children}</div>
        </div>
      </div>
    </TransitionContext.Provider>
  );
}
