// scanner.cpp : �ʷ��������ļ���Դ���򾭹��ʷ��������������Token����

#include <iostream>
#include <string>
using namespace std;

// ȡ����һ�ǿ��ַ�
// -> void: ���ؿ�
void getNextChar();

// �����ַ�
void ungetNextChar();

// ���ұ�����
// @PARAM s: Ҫ���ı�ʶ��
void reservedLookup(string s);

// ȡ�����е���
void getTokenList();
