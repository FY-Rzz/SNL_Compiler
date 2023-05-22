// Util_Lex.cpp : 语法分析工具程序

#include <iostream>
#include <string>
using namespace std;

// 显示语法分析结果
void printTokenlist();

// 显示一个单词
// @PARAM token: 要打印的token
void printToken(string token);

// 将链表中的token存入文件
// @PARAM Chainhead: Token链表头指针
void ChainToFile();