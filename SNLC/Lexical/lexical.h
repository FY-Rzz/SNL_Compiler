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
	/*��ʶ��*/
	/*������*/
	/*�޷�������*/
	/*���ַ��ֽ��*/
	/*˫�ַ��ֽ��*/
	/*ע��ͷ��*/
	/*�ַ���ʶ��*/
	/*�����±���޷�*/
	/*����*/
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
	int lineShow;  // �к�
	LexType lex;  // �ʷ���Ϣ
	string sem;   // ������Ϣ
};

enum state {
	START, INASSIGN, INCOMMENT, INNUM, INID, INCHAR, INRANGE, DONE,FINISH
};
#endif

//-----------------------------��������------------------------------

/*---scanner.cpp---*/
// ȡ����һ�ǿ��ַ�
// -> void: ���ؿ�
void getNextChar();

// �����ַ�
void ungetNextChar();

// ���ұ�����
// @PARAM s: Ҫ���ı�ʶ��
// -> Lextype: ���ر����ִʷ�
LexType reservedLookup(string s);

// ȡ�����е���
void getTokenList(string inputFile, string outputFile);

// ���Գ���
void Lexer(string inputFile, string outputFile);

/*---Util_Lex.cpp---*/

// ��ʾ�﷨�������: ��ӡToken�б�
void printTokenlist(vector<Token> tokenlist);

// ��ʾһ������
// @PARAM token: Ҫ��ӡ��token
void printToken(Token token);

// �������е�token�����ļ�
// @PARAM Chainhead: Token����ͷָ��
void ChainToFile(vector<Token> tokenlist, string outputfile);

// ������Ϣ
void InputError(string info,const string file);