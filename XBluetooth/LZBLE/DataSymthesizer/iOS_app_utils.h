#ifndef iOS_app_utils_h
#define iOS_app_utils_h

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * 字符串
 */
char* substring(char* ch,int pos,int length);

/**
 * 切割字符串
 */
void split(char** arr,char *str, const char *del);

/*
 * 16进制转10进制
 */
int hextoi(const char* str);

/**
 * 字符串替换
 * char *pInput 源字符串
 * char *pOutput 输出目标字符数组
 * char *pSrc 要替换的字符串
 * char *pDst 替换成的字符串
 */
void replace(char *pInput, char *pOutput, char *pSrc, char *pDst);

/**
 *功能:将数值转换为字符串
 *参数:第一个是要转化的整数
 *第二个是转化后的字符串
 *第三个是要转化整数的基数，就是说如果基数是10，就可以直接转化，如果不是10，是其他值（2-36之间），则先把该整数转化为该基数的数后，再转化为字符串
 */
void itoa (int val, char *buf, unsigned radix);

void reversestr(char *source,char target[],unsigned int length);

/**
 * 10进制转为16进制
 */
void tohex(unsigned long num,char *hexStr);

int tobcd(int a,int b);

#endif /* iOS_app_utils_h */
