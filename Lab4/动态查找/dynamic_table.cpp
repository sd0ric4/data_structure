#include <iostream>
#include "search_table.h"

// 全局比较次数变量
int count = 0;

// 构造函数（创建）
void DynamicSearchTable::create(const SSTable &ST)
{
    // 清理可能存在的旧树
    destroyTree(root);

    // 逐个插入元素
    for (int i = 1; i <= ST.length; i++)
    {
        insert(ST.elem[i]);
    }
}

// 插入函数
bool DynamicSearchTable::insert(const RecordType &e)
{
    // 如果树为空，创建根节点
    if (root == nullptr)
    {
        root = new BSTNode(e);
        return true;
    }

    // 查找插入位置
    BSTree p = root, parent = nullptr;
    while (p != nullptr)
    {
        // 如果关键字已存在，插入失败
        if (e.key == p->data.key)
            return false;

        parent = p;
        if (e.key < p->data.key)
            p = p->lchild;
        else
            p = p->rchild;
    }

    // 创建新节点
    BSTree newNode = new BSTNode(e);

    // 插入新节点
    if (e.key < parent->data.key)
        parent->lchild = newNode;
    else
        parent->rchild = newNode;

    return true;
}

// 中序遍历
void DynamicSearchTable::traverseInOrder() const
{
    std::cout << "学号 姓名 性别 年龄\n";
    inOrderTraversal(root);
}

void DynamicSearchTable::inOrderTraversal(BSTree node) const
{
    if (node == nullptr)
        return;

    inOrderTraversal(node->lchild);

    std::cout << node->data.key << " "
              << node->data.name << " "
              << node->data.sex << " "
              << node->data.age << std::endl;

    inOrderTraversal(node->rchild);
}

// 查找函数
BSTNode *DynamicSearchTable::search(KeyType key)
{
    count = 0;
    BSTree p = root;

    while (p != nullptr)
    {
        count++;
        if (key == p->data.key)
            return p;
        else if (key < p->data.key)
            p = p->lchild;
        else
            p = p->rchild;
    }

    return nullptr;
}

// 找最小关键字节点
BSTree DynamicSearchTable::findMin(BSTree node)
{
    while (node->lchild != nullptr)
        node = node->lchild;
    return node;
}

// 删除函数
bool DynamicSearchTable::remove(KeyType key)
{
    if (root == nullptr)
        return false;

    BSTree parent = nullptr;
    BSTree current = root;
    bool isLeftChild = true;

    // 找到要删除的节点
    while (current->data.key != key)
    {
        parent = current;
        if (key < current->data.key)
        {
            isLeftChild = true;
            current = current->lchild;
        }
        else
        {
            isLeftChild = false;
            current = current->rchild;
        }

        if (current == nullptr)
            return false; // 未找到要删除的节点
    }

    // 情况1：要删除的是叶子节点
    if (current->lchild == nullptr && current->rchild == nullptr)
    {
        if (current == root)
            root = nullptr;
        else if (isLeftChild)
            parent->lchild = nullptr;
        else
            parent->rchild = nullptr;
        delete current;
    }
    // 情况2：只有一个子节点
    else if (current->rchild == nullptr)
    { // 只有左子节点
        if (current == root)
            root = current->lchild;
        else if (isLeftChild)
            parent->lchild = current->lchild;
        else
            parent->rchild = current->lchild;
        delete current;
    }
    else if (current->lchild == nullptr)
    { // 只有右子节点
        if (current == root)
            root = current->rchild;
        else if (isLeftChild)
            parent->lchild = current->rchild;
        else
            parent->rchild = current->rchild;
        delete current;
    }
    // 情况3：有两个子节点
    else
    {
        // 找到右子树中的最小节点（后继节点）
        BSTree successor = findMin(current->rchild);

        // 复制后继节点的数据到当前节点
        current->data = successor->data;

        // 删除后继节点
        if (current->rchild == successor)
            current->rchild = successor->rchild;
        else
        {
            BSTree successorParent = current;
            BSTree s = current->rchild;
            while (s != successor)
            {
                successorParent = s;
                s = s->lchild;
            }
            successorParent->lchild = successor->rchild;
        }
        delete successor;
    }

    return true;
}

// 析构函数，清理内存
DynamicSearchTable::~DynamicSearchTable()
{
    destroyTree(root);
}

void DynamicSearchTable::destroyTree(BSTree &node)
{
    if (node == nullptr)
        return;
    destroyTree(node->lchild);
    destroyTree(node->rchild);
    delete node;
    node = nullptr;
}

// 输入记录
RecordType inputRecord()
{
    RecordType e;
    std::cout << "请输入学号 姓名 性别 年龄（输入-1结束）: ";
    std::cin >> e.key;
    if (e.key == -1)
        return e;
    std::cin >> e.name >> e.sex >> e.age;
    return e;
}

// 输出记录
void outputRecord(const BSTNode *p)
{
    std::cout << p->data.key << " "
              << p->data.name << " "
              << p->data.sex << " "
              << p->data.age << std::endl;
}