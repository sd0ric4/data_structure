// src/StackQueue.c
#include "StackQueue.h"

/**
 * 初始化栈, 构造一个空栈
 * @param S 栈指针
 * @return 初始化成功返回1，否则返回0
 *
 * 算法实现:
 * 1. 使用 malloc 分配存储空间。
 * 2. 检查分配是否成功，若不成功则返回 0。
 * 3. 设置栈顶指针和栈底指针。
 * 4. 返回 1。
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
 *
 * 算法实现:
 * 1. 将栈顶指针重置为栈底指针。
 * 2. 返回 1。
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
 *
 * 算法实现:
 * 1. 计算栈顶指针与栈底指针的差值。
 * 2. 返回差值。
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
 *
 * 算法实现:
 * 1. 检查栈是否为空。
 * 2. 若不为空，返回栈顶元素的值。
 * 3. 若为空，返回 0。
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
 *
 * 算法实现:
 * 1. 检查栈是否已满。
 * 2. 若已满，重新分配存储空间。
 * 3. 将元素 e 压入栈顶。
 * 4. 返回 1。
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
 *
 * 算法实现:
 * 1. 检查栈是否为空。
 * 2. 若不为空，弹出栈顶元素并返回其值。
 * 3. 若为空，返回 0。
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
 *
 * 算法实现:
 * 1. 使用 malloc 分配存储空间。
 * 2. 检查分配是否成功，若不成功则返回 0。
 * 3. 设置队列的 front 和 rear 指针。
 * 4. 返回 1。
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
 *
 * 算法实现:
 * 1. 将 front 和 rear 指针重置为 0。
 * 2. 返回 1。
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
 *
 * 算法实现:
 * 1. 计算 rear 和 front 指针的差值。
 * 2. 返回差值。
 */
int QueueLength(SqQueue Q)
{
    return (Q.rear - Q.front + MAXQSIZE) % MAXQSIZE;
}

/**
 * 判断队列是否为空
 * @param Q 队列
 * @return 为空返回1，否则返回0
 *
 * 算法实现:
 * 1. 检查 front 和 rear 指针是否相等。
 * 2. 返回检查结果。
 */
Status QueueEmpty(SqQueue Q)
{
    return Q.front == Q.rear;
}

/**
 * 判断队列是否已满
 * @param Q 队列
 * @return 已满返回1，否则返回0
 *
 * 算法实现:
 * 1. 检查 (rear + 1) % MAXQSIZE 是否等于 front。
 * 2. 返回检查结果。
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
 *
 * 算法实现:
 * 1. 检查队列是否已满。
 * 2. 若未满，将元素 e 放入 rear 位置，并更新 rear 指针。
 * 3. 返回 1。
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
 *
 * 算法实现:
 * 1. 检查队列是否为空。
 * 2. 若不为空，将队头元素出队，并更新 front 指针。
 * 3. 返回 1。
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
 *
 * 算法实现:
 * 1. 初始化栈。
 * 2. 将 m 转换为 n 进制数，并将每位数字压入栈中。
 * 3. 弹出栈中所有元素，输出转换结果。
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
 *
 * 算法实现:
 * 1. 初始化栈。
 * 2. 将队列中的所有元素依次出队并压入栈中。
 * 3. 将栈中的所有元素依次弹出并入队。
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