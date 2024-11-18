#include "binary_tree.h"

// 创建二叉树
void CreateBiTree(BiTree &T) {
    char ch;
    cin >> ch;
    if (ch == '#') {
        T = nullptr;
    } else {
        T = new BiTNode;
        T->data = ch;
        CreateBiTree(T->lchild);
        CreateBiTree(T->rchild);
    }
}

// 递归遍历
void PreOrderTraverse(BiTree T) {
    if (T) {
        cout << T->data << " ";
        PreOrderTraverse(T->lchild);
        PreOrderTraverse(T->rchild);
    }
}

void InOrderTraverse(BiTree T) {
    if (T) {
        InOrderTraverse(T->lchild);
        cout << T->data << " ";
        InOrderTraverse(T->rchild);
    }
}

void PostOrderTraverse(BiTree T) {
    if (T) {
        PostOrderTraverse(T->lchild);
        PostOrderTraverse(T->rchild);
        cout << T->data << " ";
    }
}

// 非递归遍历
void NRPreOrderTraverse(BiTree T) {
    if (!T)
        return;
    stack<BiTNode *> s;
    s.push(T);
    while (!s.empty()) {
        BiTNode *node = s.top();
        s.pop();
        cout << "访问节点：" << node->data << endl;
        if (node->rchild)
            s.push(node->rchild);
        if (node->lchild)
            s.push(node->lchild);
    }
}

void NRInOrderTraverse(BiTree T) {
    stack<BiTNode *> s;
    BiTNode *current = T;
    while (current || !s.empty()) {
        while (current) {
            s.push(current);
            current = current->lchild;
        }
        if (!s.empty()) {
            current = s.top();
            s.pop();
            cout << "访问节点：" << current->data << endl;
            current = current->rchild;
        }
    }
}

void NRPostOrderTraverse(BiTree T) {
    if (!T)
        return;
    stack<BiTNode *> s1, s2;
    s1.push(T);
    while (!s1.empty()) {
        BiTNode *node = s1.top();
        s1.pop();
        s2.push(node);
        if (node->lchild)
            s1.push(node->lchild);
        if (node->rchild)
            s1.push(node->rchild);
    }
    while (!s2.empty()) {
        cout << "访问节点：" << s2.top()->data << endl;
        s2.pop();
    }
}

// 节点度统计
int TNodes(BiTree T, int d) {
    if (!T)
        return 0;
    int count = 0;
    if ((T->lchild != nullptr) + (T->rchild != nullptr) == d)
        count++;
    return count + TNodes(T->lchild, d) + TNodes(T->rchild, d);
}

// 求树的高度
int High(BiTree T) {
    if (!T)
        return 0;
    return max(High(T->lchild), High(T->rchild)) + 1;
}

// 创建二叉搜索树
void CreateBST(BiTree &T, const char *chars) {
    T = nullptr;
    for (int i = 0; chars[i] != '\0'; i++) {
        BiTNode *newNode = new BiTNode;
        newNode->data = chars[i];
        newNode->lchild = newNode->rchild = nullptr;
        if (!T) {
            T = newNode;
        } else {
            BiTNode *current = T, *parent = nullptr;
            while (current) {
                parent = current;
                if (chars[i] < current->data)
                    current = current->lchild;
                else
                    current = current->rchild;
            }
            if (chars[i] < parent->data)
                parent->lchild = newNode;
            else
                parent->rchild = newNode;
        }
    }
}
