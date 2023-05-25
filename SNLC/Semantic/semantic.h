#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "../Grammar/grammar.h"

using namespace std;

//-------------------------------��������--------------------------------

/*---symbTable.cpp---*/

// ������ǰ�շ��ű�
void NewTable();

// �����µľֲ�����λ�Ŀշ��ű�
void CreateTable();

// ������ǰ���ű�
void DestroyTable();

// �ǼǱ�ʶ��������
void Enter();

// Ѱ�ұ����ַ
void FindEnter();

// ���Բ�ѯ
void FindAttr();

// �ж������Ƿ�����
void Compat();

// ������ǰ�������ڲ���ʾ
void NewTy();

// ������ǰ�ռ�¼������..
void NewBody();

// ������ǰ���β�����
void NewParam();

// ������ʾ
void ErrorPrompt();

// ��ӡ�ո�
void printTab();

// ��ӡ���ű�
void printSymbTable();


// ��ӡ���ű��һ��
void printOneLayer();

/*---Analyze.cpp---*/

// ��ʼ����������
void initialize();

// �������ͷ���
void TypeProcess();

// �����Զ���������
void nameType();

// �����������͵��ڲ���ʾ
void arrayType();

// �����¼���͵��ڲ���ʾ
void recordType();

// ����������������
void TypeDecPart();

// ���������������
void VarDecList();

// ���������������
void ProcDecPart();

// �������ͷ���������
void HeadProcess();

// ��������������������
void ParaDecList();

// ����ִ���岿�ֵ��������
void Body();

// �������״̬
void statement();

// ������ʽ
void Expr();

// �����±����
void arrayVar();

// ���������
void recordVar();

// ����ֵ���
void Assignstatement();

// ����������
void callstatement();

// �����������
void ifstatement();

// ����ѭ�����
void Whilestatement();

// ��������
void readstatement();

// ����д���
void Writestatement();

// ���������
void Returnstatement();

// �﷨����������
void Analyze();