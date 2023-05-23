#pragma once
#ifndef DEFINED
#include <iostream>
#include<string>
#include<fstream>
using namespace std;

// 
enum LexType
{
	ID, IF, BEGIN, INTC, END, PLUS, MINUS, TIMES, OVER, EQ, LT, LMIDPAREN, RMIDPAREN, DOT
	, TYPE, VAR, PROCEDURE, PROGRAM, SEMI, INTEGER, CHAR, ARRAY, RECORD, UNDERANGE
	, OF, COMMA, LPAREN, RPAREN, ENDWH, WHILE, RETURN, READ, WRITE, ASSIGN, THEN, FI
	, ELSE, DO
};

struct Token
{
	int lineNum;  // �к�
	LexType lex;  // �ʷ���Ϣ
	string sem;   // ������Ϣ
};

enum state {
	START, INASSIGN, INCOMMENT, INNUM, INID, INCHAR, INRANGE, DONE, FINISH
};
#endif