#pragma once

#include<iostream>
#include "../Intermediate_generation/midcode_g.h"


struct LoopInfo     //ѭ����Ϣ��
{
	int state;             //ѭ��״̬��0��1��
	CodeFile* whileEntry;  //ָ��ѭ������м����
	int varDef;            //������ֵ����±�ֵ��������Ŀ�ʼλ��
	CodeFile* whileEnd;    //ָ��ѭ�������м����
};

extern ArgRecord* VarDefSet[100000];  //������ֵ��
extern LoopInfo* LoopInfoStack[100000];  //ѭ����Ϣջ
extern int setNum;
extern int IStop;


//ѭ������ʽ�Ż�������
CodeFile* LoopOpti();

//ѭ����ڵĴ�����
void whileEntry(CodeFile* code);

//���̵������Ĵ�����
void call(CodeFile* code);

//ѭ�����ڴ�����
void whileEnd(CodeFile* code);

//������ֵ����Һ���
int SearchTable(ArgRecord* arg, int head);

//ɾ��������ֵ���е�һ��
void DelItem(ArgRecord* arg, int head);

//��ӱ�����ֵ���е�һ��
void AddTable(ArgRecord* arg);

//ѭ�����ᴦ����
void LoopOutSide(CodeFile* entry);

//ѭ�������Ż��������
void PrintOptMidCode(CodeFile* code);