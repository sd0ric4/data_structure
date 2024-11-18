#ifndef STRING_STORAGE_H
#define STRING_STORAGE_H

/**
 * 串的顺序存储结构
 * ch 为字符数组
 * length 为串长度
 */
typedef struct
{
    char *ch;   // 若是非空串，则按串长分配存储区，否则 ch 为 NULL
    int length; // 串长度
} HString;

// 基本操作函数声明
void StrAssign(HString *S, const char *chars);
int StrCompare(HString S, HString T);
int StrLength(HString S);
void Concat(HString *S, HString T1, HString T2);
void SubString(HString *Sub, HString S, int pos, int len);
void StrTraverse(HString S);
void StrCopy(HString *S, HString T);

// 其他操作函数声明
int Index(HString S, HString T, int pos);
void Replace(HString *S, HString T1, HString T2);
void StrInsert(HString *S, int pos, HString T);
void StrDelete(HString *S, int pos, int len);

// 工具函数
void InitString(HString *S);
void ClearString(HString *S);

#endif