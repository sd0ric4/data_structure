// src/main.c
#include "StackQueue.h"

int main()
{
    // 数制转换测试
    printf("数制转换测试：\n");
    int m = 1348;
    printf("十进制数 %d 的转换结果：\n", m);
    Conversion(m, 2); // 转换为二进制
    Conversion(m, 8); // 转换为八进制

    // 循环队列测试
    SqQueue Q;
    InitQueue(&Q);

    // 连续入队9个整数
    printf("\n循环队列测试：\n");
    printf("连续入队9个整数：\n");
    for (int i = 1; i <= 9; i++)
    {
        EnQueue(&Q, i);
    }
    printf("此时front=%d, rear=%d\n", Q.front, Q.rear);

    // 再入队1个整数
    if (EnQueue(&Q, 10))
    {
        printf("第10个数入队成功\n");
    }
    else
    {
        printf("队列已满，入队失败\n");
    }

    // 连续出队5个整数，再入队2个整数
    for (int i = 0; i < 5; i++)
    {
        int e;
        DeQueue(&Q, &e);
    }
    EnQueue(&Q, 11);
    EnQueue(&Q, 12);

    printf("出队5个，入队2个后：\n");
    printf("队列长度=%d, front=%d, rear=%d\n",
           QueueLength(Q), Q.front, Q.rear);

    // 队列逆序重排并输出
    printf("\n队列逆序重排后的元素：\n");
    QueueReverse(&Q);
    int e;
    while (!QueueEmpty(Q))
    {
        DeQueue(&Q, &e);
        printf("%d ", e);
    }
    printf("\n");

    return 0;
}