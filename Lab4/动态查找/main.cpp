#include <iostream>
#include <iomanip>
#include <limits>
#include "search_table.h"

// 清除输入缓冲区
void clearInputStream()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// 显示菜单
void displayMenu()
{
    std::cout << "\n======= 动态查找表操作菜单 =======\n";
    std::cout << "1. 显示静态查找表\n";
    std::cout << "2. 显示动态查找表\n";
    std::cout << "3. 查找记录\n";
    std::cout << "4. 插入记录\n";
    std::cout << "5. 删除记录\n";
    std::cout << "0. 退出程序\n";
    std::cout << "请选择操作（0-5）：";
}

int main()
{
    // 创建静态查找表
    SSTable ST1;
    ST1.create(11);

    // 创建动态查找表
    DynamicSearchTable DT1;
    DT1.create(ST1);

    int choice;
    while (true)
    {
        displayMenu();

        // 安全地读取输入
        if (!(std::cin >> choice))
        {
            clearInputStream();
            std::cout << "输入无效，请重新输入！\n";
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            std::cout << "\n静态查找表：\n";
            ST1.traverse();
            break;
        }
        case 2:
        {
            std::cout << "\n动态查找表（中序遍历）：\n";
            DT1.traverseInOrder();
            break;
        }
        case 3:
        {
            std::cout << "\n===== 查找记录 =====\n";
            while (true)
            {
                std::cout << "请输入要查找的学号（输入-1返回）：";
                int key;
                std::cin >> key;

                if (key == -1)
                    break;

                count = 0; // 重置比较次数
                BSTNode *p = DT1.search(key);

                if (!p)
                {
                    std::cout << "记录不存在！\n";
                }
                else
                {
                    std::cout << "找到记录：";
                    outputRecord(p);
                    std::cout << "比较次数：" << count << std::endl;
                }
            }
            break;
        }
        case 4:
        {
            std::cout << "\n===== 插入记录 =====\n";
            while (true)
            {
                RecordType e = inputRecord();

                if (e.key == -1)
                    break;

                if (DT1.insert(e))
                {
                    std::cout << "插入成功！当前动态查找表：\n";
                    DT1.traverseInOrder();
                }
                else
                {
                    std::cout << "记录已存在，插入失败！\n";
                }
            }
            break;
        }
        case 5:
        {
            std::cout << "\n===== 删除记录 =====\n";
            while (true)
            {
                std::cout << "请输入要删除的学号（输入-1返回）：";
                int key;
                std::cin >> key;

                if (key == -1)
                    break;

                if (DT1.remove(key))
                {
                    std::cout << "删除成功！当前动态查找表：\n";
                    DT1.traverseInOrder();
                }
                else
                {
                    std::cout << "记录不存在，删除失败！\n";
                }
            }
            break;
        }
        case 0:
        {
            std::cout << "程序已退出。\n";
            return 0;
        }
        default:
        {
            std::cout << "无效的选择，请重新输入！\n";
            clearInputStream();
        }
        }

        // 暂停，等待用户按回车继续
        std::cout << "\n按回车键继续...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    return 0;
}