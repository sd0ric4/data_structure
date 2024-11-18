#include "threaded_binary_tree.h"

// 初始化尚未线索化的二叉树
void InitBiThrTree(BiThrTree &T) {
    char ch;
    cin >> ch;
    if (ch == '#') {
        T = nullptr;
    } else {
        T = new BiThrNode;
        T->data = ch;
        T->LTag = T->RTag = 0; // 初始标记为孩子
        InitBiThrTree(T->lchild);
        InitBiThrTree(T->rchild);
    }
}

// 中序线索化
void InOrderThreading(BiThrTree &Thrt, BiThrTree T) {
    Thrt = new BiThrNode;
    Thrt->data = '@';    // 头结点数据
    Thrt->LTag = 0;      // 左指针指向根结点
    Thrt->RTag = 1;      // 右指针初始化为线索
    Thrt->rchild = Thrt; // 头结点右指针指向自己

    if (!T) { // 若树为空
        Thrt->lchild = Thrt;
        return;
    }

    BiThrTree pre = Thrt; // pre指向头结点
    Thrt->lchild = T;     // 左指针指向根结点

    stack<BiThrTree> s;
    BiThrTree p = T;

    while (p || !s.empty()) {
        while (p) {
            s.push(p);
            p = p->lchild;
        }
        if (!s.empty()) {
            p = s.top();
            s.pop();

            // 前驱线索化
            if (!p->lchild) {
                p->LTag = 1;
                p->lchild = pre;
            }

            // 后继线索化
            if (!pre->rchild) {
                pre->RTag = 1;
                pre->rchild = p;
            }

            pre = p;
            p = p->rchild;
        }
    }

    // 处理最后一个结点的线索化
    pre->RTag = 1;
    pre->rchild = Thrt;
    Thrt->rchild = pre;
}

// 递归中序遍历线索二叉树
void InOrderTraverse(BiThrTree T) {
    if (T) {
        if (T->LTag == 0)
            InOrderTraverse(T->lchild);
        cout << "| " << T->LTag << " | " << (T->lchild ? T->lchild->data : '^')
             << " | " << T->data << " | " << (T->rchild ? T->rchild->data : '^')
             << " | " << T->RTag << " |" << endl;
        if (T->RTag == 0)
            InOrderTraverse(T->rchild);
    }
}

// 非递归中序遍历线索二叉树
void InOrderTraverse_Thr(BiThrTree T) {
    BiThrTree p = T->lchild; // 从头结点的左指针开始
    while (p != T) {
        while (p->LTag == 0)
            p = p->lchild; // 找到最左边的节点
        cout << "| " << p->LTag << " | " << (p->lchild ? p->lchild->data : '^')
             << " | " << p->data << " | " << (p->rchild ? p->rchild->data : '^')
             << " | " << p->RTag << " |" << endl;

        while (p->RTag == 1 && p->rchild != T) {
            p = p->rchild; // 顺着线索找后继
            cout << "| " << p->LTag << " | "
                 << (p->lchild ? p->lchild->data : '^') << " | " << p->data
                 << " | " << (p->rchild ? p->rchild->data : '^') << " | "
                 << p->RTag << " |" << endl;
        }
        p = p->rchild; // 移动到右孩子
    }
}
