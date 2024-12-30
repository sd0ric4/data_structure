#include <iostream>
#include "search_table.h"

void SSTable::create(int n)
{
    // 调整vector大小以便使用标准下标
    elem.resize(n + 1);
    length = n;

    // 初始化数据，使用列表初始化和构造函数
    elem[1] = {56, "Zhang", "F", 19};
    elem[2] = {19, "Wang", "F", 20};
    elem[3] = {80, "Zhou", "F", 19};
    elem[4] = {5, "Huang", "M", 20};
    elem[5] = {21, "Zheng", "M", 20};
    elem[6] = {64, "Li", "M", 19};
    elem[7] = {88, "Liu", "F", 18};
    elem[8] = {13, "Qian", "F", 19};
    elem[9] = {37, "Sun", "M", 20};
    elem[10] = {75, "Zhao", "M", 20};
    elem[11] = {92, "Chen", "M", 20};
}

void SSTable::traverse() const
{
    std::cout << "学号 姓名 性别 年龄\n";
    for (int i = 1; i <= length; i++)
    {
        std::cout << elem[i].key << " "
                  << elem[i].name << " "
                  << elem[i].sex << " "
                  << elem[i].age << std::endl;
    }
}