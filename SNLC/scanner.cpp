// scanner.cpp : 词法分析主文件，源程序经过词法分析程序后生成Token序列

#include <iostream>
#include <string>
using namespace std;

// 取得下一非空字符
// -> void: 返回空
void getNextChar();

// 回退字符
void ungetNextChar();

// 查找保留字
// @PARAM s: 要检查的标识符
void reservedLookup(string s);

// 取得所有单词
void getTokenList();
