#pragma once
#ifndef DEFINED
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

// 
enum LexType
{
	/*标识符*/
	/*保留字*/
	/*无符号整数*/
	/*单字符分界符*/
	/*双字符分界符*/
	/*注释头符*/
	/*字符标识符*/
	/*数组下标界限符*/
	/*错误*/
	ID, IF, BEGIN, INTC, END, PLUS, MINUS, TIMES, OVER, EQ, LT, LMIDPAREN, RMIDPAREN, DOT
	, TYPE, VAR, PROCEDURE, PROGRAM, SEMI, INTEGER, CHAR, ARRAY, RECORD, UNDERANGE
	, OF, COMMA, LPAREN, RPAREN, ENDWH, WHILE, RETURN, READ, WRITE, ASSIGN, THEN, FI
	, ELSE, DO
};

string reserved_word[] = { "if","begin","intc","end"
	,"type","var", "procedure","program","integer","char","array","record"
	,"of","endwh","while","return","read","write","then"
	,"else","do"
};

struct Token
{
	int lineShow;  // 行号
	LexType lex;  // 词法信息
	string sem;   // 语义信息
};

enum state {
	START, INASSIGN, INCOMMENT, INNUM, INID, INCHAR, INRANGE, DONE,FINISH
};
#endif

//-----------------------------函数声明------------------------------

/*---scanner.cpp---*/
// 取得下一非空字符
// -> void: 返回空
void getNextChar();

// 回退字符
void ungetNextChar();

// 查找保留字
// @PARAM s: 要检查的标识符
// -> Lextype: 返回保留字词法
LexType reservedLookup(string s);

// 取得所有单词
void getTokenList(string inputFile, string outputFile);

// 测试程序
void Lexer(string inputFile, string outputFile);

/*---Util_Lex.cpp---*/

// 显示语法分析结果: 打印Token列表
void printTokenlist(vector<Token> tokenlist);

// 显示一个单词
// @PARAM token: 要打印的token
void printToken(Token token);

// 将链表中的token存入文件
// @PARAM Chainhead: Token链表头指针
void ChainToFile(vector<Token> tokenlist, string outputfile);

// 报错信息
void InputError(string info,const string file);