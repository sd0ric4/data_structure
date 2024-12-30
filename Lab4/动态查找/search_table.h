#ifndef SEARCH_TABLE_H
#define SEARCH_TABLE_H

#include <string>
#include <vector>

// 关键字类型
using KeyType = int;

// 数据元素（记录）的类型定义
struct RecordType
{
    KeyType key;      // 学号(关键字)
    std::string name; // 姓名
    std::string sex;  // 性别
    int age;          // 年龄

    // 构造函数，方便初始化
    RecordType() = default;
    RecordType(KeyType k, const std::string &n, const std::string &s, int a)
        : key(k), name(n), sex(s), age(a) {}
};

// 静态查找表结构体
struct SSTable
{
    std::vector<RecordType> elem; // 使用vector替代数组
    int length;

    // 初始化静态查找表
    void create(int n);
    void traverse() const;
};

// 二叉搜索树节点定义
struct BSTNode
{
    RecordType data;
    BSTNode *lchild, *rchild;

    // 构造函数
    explicit BSTNode(const RecordType &record)
        : data(record), lchild(nullptr), rchild(nullptr) {}
};

// 别名定义
using BSTree = BSTNode *;

// 全局比较次数变量
extern int count;

// 动态查找表操作函数声明
class DynamicSearchTable
{
public:
    // 构造函数
    DynamicSearchTable() = default;

    // 从静态查找表创建
    void create(const SSTable &ST);

    // 插入元素
    bool insert(const RecordType &e);

    // 中序遍历
    void traverseInOrder() const;

    // 查找
    BSTNode *search(KeyType key);

    // 删除
    bool remove(KeyType key);

    // 析构函数（清理内存）
    ~DynamicSearchTable();

private:
    BSTree root = nullptr;

    // 辅助函数
    void destroyTree(BSTree &node);
    BSTree findMin(BSTree node);
    void inOrderTraversal(BSTree node) const;
};

// 输入输出操作
RecordType inputRecord();
void outputRecord(const BSTNode *p);

#endif // SEARCH_TABLE_H