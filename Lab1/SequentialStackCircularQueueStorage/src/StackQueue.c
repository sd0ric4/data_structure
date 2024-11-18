// src/StackQueue.c
#include "StackQueue.h"

/**
 * 初始化栈, 构造一个空栈
 * @param S 栈指针
 * @return 初始化成功返回1，否则返回0
 */
Status InitStack(SqStack *S)
{
    S->base = (SElemType *)malloc(STACK_INIT_SIZE * sizeof(SElemType));
    if (!S->base)
        return 0;
    S->top = S->base;
    S->stacksize = STACK_INIT_SIZE;
    return 1;
}
/**
 * 清空栈
 * @param S 栈指针
 * @return 清空成功返回1
 */
Status ClearStack(SqStack *S)
{
    S->top = S->base;
    return 1;
}
/**
 * 获取栈长度
 * @param S 栈
 * @return 栈长度
 */
int StackLength(SqStack S)
{
    return S.top - S.base;
}

/**
 * 获取栈顶元素
 * @param S 栈
 * @param e 保存栈顶元素
 * @return 获取成功返回1，否则返回0
 */
Status GetTop(SqStack S, SElemType *e)
{
    if (S.top == S.base)
        return 0;
    *e = *(S.top - 1);
    return 1;
}
/**
 * 入栈
 * @param S 栈指针
 * @param e 入栈元素
 * @return 入栈成功返回1，否则返回0
 */
Status Push(SqStack *S, SElemType e)
{
    if (S->top - S->base >= S->stacksize)
    {
        SElemType *newbase = (SElemType *)realloc(S->base,
                                                  (S->stacksize + STACKINCREMENT) * sizeof(SElemType));
        if (!newbase)
            return 0;
        S->base = newbase;
        S->top = S->base + S->stacksize;
        S->stacksize += STACKINCREMENT;
    }
    *S->top++ = e;
    return 1;
}
/**
 * 出栈
 * @param S 栈指针
 * @param e 保存出栈元素
 * @return 出栈成功返回1，否则返回0
 */
Status Pop(SqStack *S, SElemType *e)
{
    if (S->top == S->base)
        return 0;
    *e = *--S->top;
    return 1;
}

/**
 * 初始化队列
 * @param Q 队列指针
 * @return 初始化成功返回1，否则返回0
 */
Status InitQueue(SqQueue *Q)
{
    Q->base = (QElemType *)malloc(MAXQSIZE * sizeof(QElemType));
    if (!Q->base)
        return 0;
    Q->front = Q->rear = 0;
    return 1;
}

/**
 * 清空队列
 * @param Q 队列指针
 * @return 清空成功返回1
 */
Status ClearQueue(SqQueue *Q)
{
    Q->front = Q->rear = 0;
    return 1;
}

/**
 * 获取队列长度
 * @param Q 队列
 * @return 队列长度
 */
int QueueLength(SqQueue Q)
{
    return (Q.rear - Q.front + MAXQSIZE) % MAXQSIZE;
}

/**
 * 判断队列是否为空
 * @param Q 队列
 * @return 为空返回1，否则返回0
 */
Status QueueEmpty(SqQueue Q)
{
    return Q.front == Q.rear;
}

/**
 * 判断队列是否已满
 * @param Q 队列
 * @return 已满返回1，否则返回0
 */
Status QueueFull(SqQueue Q)
{
    return (Q.rear + 1) % MAXQSIZE == Q.front;
}

/**
 * 入队
 * @param Q 队列指针
 * @param e 入队元素
 * @return 入队成功返回1，否则返回0
 */
Status EnQueue(SqQueue *Q, QElemType e)
{
    if (QueueFull(*Q))
        return 0;
    Q->base[Q->rear] = e;
    Q->rear = (Q->rear + 1) % MAXQSIZE;
    return 1;
}

/**
 * 出队
 * @param Q 队列指针
 * @param e 保存出队元素
 * @return 出队成功返回1，否则返回0
 */
Status DeQueue(SqQueue *Q, QElemType *e)
{
    if (QueueEmpty(*Q))
        return 0;
    *e = Q->base[Q->front];
    Q->front = (Q->front + 1) % MAXQSIZE;
    return 1;
}

/**
 * 功能：将十进制数m转换为n进制数
 * @param m 十进制数
 * @param n n进制数
 */
void Conversion(int m, int n)
{
    SqStack S;
    InitStack(&S);

    while (m > 0)
    {
        Push(&S, m % n);
        m = m / n;
    }

    printf("转换为%d进制数: ", n);
    int e;
    while (Pop(&S, &e))
    {
        printf("%d", e);
    }
    printf("\n");
}

/**
 * 功能：将队列逆序重排
 * @param Q 队列指针
 */
void QueueReverse(SqQueue *Q)
{
    SqStack S;
    InitStack(&S);
    QElemType e;

    // 将队列元素全部入栈
    while (!QueueEmpty(*Q))
    {
        DeQueue(Q, &e);
        Push(&S, e);
    }

    // 将栈中元素全部出栈并入队
    while (StackLength(S))
    {
        Pop(&S, &e);
        EnQueue(Q, e);
    }
}