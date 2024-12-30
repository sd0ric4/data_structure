export interface StudentData {
  key: number;
  name: string;
  sex: string;
  age: number;
}

export interface Position {
  x: number;
  y: number;
}

export interface Edge {
  from: Position;
  to: Position;
}

export class TreeNode {
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

export class BSTAlgorithms {
  static insertIntoBST(root: TreeNode | null, data: StudentData): TreeNode {
    if (!root) {
      return new TreeNode(data);
    }

    if (data.key < root.data.key) {
      root.left = this.insertIntoBST(root.left, data);
    } else if (data.key > root.data.key) {
      root.right = this.insertIntoBST(root.right, data);
    }
    return root;
  }

  static deleteFromBST(root: TreeNode | null, key: number): TreeNode | null {
    if (!root) return null;

    if (key < root.data.key) {
      root.left = this.deleteFromBST(root.left, key);
    } else if (key > root.data.key) {
      root.right = this.deleteFromBST(root.right, key);
    } else {
      if (!root.left && !root.right) {
        return null;
      }
      if (!root.left) return root.right;
      if (!root.right) return root.left;

      let successor = this.findMin(root.right);
      root.data = successor.data;
      root.right = this.deleteFromBST(root.right, successor.data.key);
    }
    return root;
  }

  static findMin(node: TreeNode): TreeNode {
    let current = node;
    while (current.left) {
      current = current.left;
    }
    return current;
  }

  static getTreeHeight(node: TreeNode | null): number {
    if (!node) return 0;
    return (
      1 +
      Math.max(this.getTreeHeight(node.left), this.getTreeHeight(node.right))
    );
  }

  static calculateNodePositions(
    node: TreeNode | null,
    level: number = 0,
    minX: number = 0,
    maxX: number = 1200
  ): void {
    if (!node) return;

    const width = maxX - minX;
    const x = minX + width / 2;
    const y = level * 100;

    node.x = x;
    node.y = y;
    node.level = level;

    if (node.left) {
      this.calculateNodePositions(node.left, level + 1, minX, x - 20);
    }
    if (node.right) {
      this.calculateNodePositions(node.right, level + 1, x + 20, maxX);
    }
  }

  static async searchBST(
    node: TreeNode | null,
    key: number,
    onNodeVisit: (node: TreeNode) => void
  ): Promise<TreeNode | null> {
    if (!node) return null;

    onNodeVisit(node);
    await new Promise((resolve) => setTimeout(resolve, 500));

    if (key === node.data.key) {
      return node;
    }

    if (key < node.data.key) {
      return this.searchBST(node.left, key, onNodeVisit);
    } else {
      return this.searchBST(node.right, key, onNodeVisit);
    }
  }
}
