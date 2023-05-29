#pragma once
#include "../Intermediate_generation/midcode_g.h"

struct ConstDefT {
	ConstDefT* former; // ǰһ������ֵ�ڵ�
	ArgRecord* variable; // ARG�ṹָ��
	int constValue; // ������ǰ��ֵ
	ConstDefT* next; // ��һ������ֵ�ڵ�
};

extern vector<vector<CodeFile>> BaseBlock;

// ���ֻ�����
vector<vector<CodeFile>> DivBaseBlock(CodeFile* head, vector<vector<CodeFile>> baseBlock);

// �����ʽ�Ż�������
CodeFile* ConstOptimize();

// �������ڵĳ����ʽ�Ż�����
CodeFile* OptiBlock(int i);

// �����ͱȽ�������Ż�����
bool ArithC(CodeFile* code);

// ֵ�滻����
void SubstiArg(CodeFile* code, int i);

// ������ֵ����Һ���
bool FindConstT(ArgRecord* arg, ConstDefT** Entry);

// ������ֵ����Ӻ���
void AppendTable(ArgRecord* arg, int result);

// ɾ��������ֵ���е�һ��
void DelConst(ArgRecord* arg);

