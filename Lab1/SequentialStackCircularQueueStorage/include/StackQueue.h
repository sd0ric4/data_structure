// include/StackQueue.h
#ifndef STACK_QUEUE_H
#define STACK_QUEUE_H

#include <stdio.h>
#include <stdlib.h>

typedef int Status;
typedef int SElemType;
typedef int QElemType;

#define STACK_INIT_SIZE 10
#define STACKINCREMENT 5
#define MAXQSIZE 10

/**
 * 栈结构定义
 * base: 栈底指针
 * top: 栈顶指针
 * stacksize: 栈容量
 */
typedef struct
{
    SElemType *base;
    SElemType *top;
    int stacksize;
} SqStack;

/**
 * 队列结构定义
 * base: 队列存储空间基址
 * front: 队头指针
 * rear: 队尾指针
 */
typedef struct
{
    QElemType *base;
    int front;
    int rear;
} SqQueue;

// 栈操作函数声明
Status InitStack(SqStack *S);
Status ClearStack(SqStack *S);
int StackLength(SqStack S);
Status GetTop(SqStack S, SElemType *e);
Status Push(SqStack *S, SElemType e);
Status Pop(SqStack *S, SElemType *e);

// 队列操作函数声明
Status InitQueue(SqQueue *Q);
Status ClearQueue(SqQueue *Q);
int QueueLength(SqQueue Q);
Status EnQueue(SqQueue *Q, QElemType e);
Status DeQueue(SqQueue *Q, QElemType *e);
Status QueueEmpty(SqQueue Q);
Status QueueFull(SqQueue Q);

// 应用函数声明
void Conversion(int m, int n);
void QueueReverse(SqQueue *Q);

#endif