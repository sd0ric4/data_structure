import { type RouteConfig, index, route } from '@react-router/dev/routes';

export default [
  index('routes/home.tsx'),
  route('mgraph', './view/Mgraph.tsx'),
  route('algraph', './view/AlgorithmsGraph.tsx'),
  route('fgraph', './view/Fgraph.tsx'),
  route('mstgraph', './view/MSTgraph.tsx'),
  route('cpagraph', './view/CriticalPathAlgorithmVisualizer.tsx'),
  route('find', './view/find.tsx'),
  route('bstfind', './view/BSTfind.tsx'),
  route('hashtable', './view/HashTable.tsx'),
] satisfies RouteConfig;
