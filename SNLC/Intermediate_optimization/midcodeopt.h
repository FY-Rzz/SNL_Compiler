#pragma once
#include "../Intermediate_generation/midcode_g.h"

struct ConstDefT {
	ConstDefT* former;
	ArgRecord* variable;
	int constValue; // ������ǰ��ֵ
	ConstDefT* next;
};

extern vector<CodeFile*> BaseBlock;

// ���ֻ�����
vector<CodeFile*> DivBaseBlock(CodeFile* head, vector<CodeFile*> baseBlock);

// �����ʽ�Ż�������
CodeFile* ConstOptimize();

// �������ڵĳ����ʽ�Ż�����
void OptiBlock(int i);

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

