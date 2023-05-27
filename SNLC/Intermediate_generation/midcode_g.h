#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "../Semantic/semantic.h"

enum Form{ ValueForm, LabelForm, AddrForm };

enum CodeKind {
	// ��������
	ADD, SUB, MULT, DIV,
	// ��ϵ����
	EQC, LTC, 
	// ���
	READC, WRITEC, RETURNC, ASSIG, AADD, LABEL, JUMP, JUMP0, CALL, VARACT, VALACT,
	// ��λ
	PENTRY, ENDPROC, MENTRY
};

struct ArgRecord {
	Form form;
	union 
	{
		int value;
		int label;
		struct {
			string name;
			int dataLevel;
			int dataOff;
			AccessKind access; // dir or indir
		}Addr;
	} Attr;
};

// �м����ṹ
struct MidCode {
	CodeKind codekind;
	ArgRecord* arg1; // ��������1
	ArgRecord* arg2; // ��������2
	ArgRecord* arg3; // ���
};

// �м��������
struct CodeFile {
	CodeFile* former;
	MidCode* onecode;
	CodeFile* next;
};


//-------------------------------��������------------------------

/*---Util.cpp---*/
// �½�һ����ʱ����
ArgRecord* NewTemp(AccessKind access);

// �½���ֵ�� ARG �ṹ
ArgRecord* ARGValue(int value);

// ����һ���µı��
int NewLabel();

// �½������ ARG �ṹ
ArgRecord* ARGLabel(int label);

// ������ַ�� ARG �ṹ
ArgRecord* ARGAddr(char* id, int level, int off, AccessKind access);

// ����м����
void PrintMidCode(CodeFile* firstCode);

// �����м����
CodeFile* GenCode(CodeKind codekind, ArgRecord* Arg1, ArgRecord
	* Arg2, ArgRecord* Arg3);


/*---midcode.cpp---*/
// �м��������������
CodeFile* GenMidCode(TreeNode* t);

// ���������м�������ɺ���
void GenProcDec(TreeNode* t);

// ������м�������ɺ���
void GenBody(TreeNode* t);

// �����м�������ɺ���
void GenStatement(TreeNode* t);

// ��ֵ����м�������ɺ���
void GenAssignS(TreeNode* t);

// �����м�������ɺ���
ArgRecord* GenVar(TreeNode* t);

//����������м�������ɺ���
ArgRecord* GenArray(ArgRecord* Vlarg, TreeNode* t, int low, int size);

// ���Ա�������м��������
ArgRecord* GenField(ArgRecord* V1arg, TreeNode* t, fieldChain* head);

// ���ʽ���м�������ɺ���
ArgRecord* GenExpr(TreeNode* t);

// ���̵�������м�������ɺ���
void GenCallS(TreeNode* t);

// ������м�������ɺ���
void GenReadS(TreeNode* t);

// ��������м�������ɺ���
void GenIfS(TreeNode* t);

// д����м�������ɺ���
void GenWriteS(TreeNode* t);

// ѭ������м�������ɺ���
void GenWhileS(TreeNode* t);
