#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "../Grammar/grammar.h"

using namespace std;

//-------------------------------
// ��ʶ�����
enum IdKind { typeKind, varKind, procKind };// ���͡�����������

//
enum AccessKind { dir, indir }; // ֱ�ӱ�������ӱ���

// ����ö��
enum TypeKind { intTy, charTy, arrayTy, recordTy, boolTy }; // 


//�β���Ϣ������
struct ParamTable;

//��ʶ����Ϣ��
typedef struct
{
	struct typeIR* idtype; /*ָ���ʶ���������ڲ���ʾ*/
	IdKind kind; /*��ʶ��������*/  //typeKind, varKind, procKind
	union
	{
		struct
		{
			//Access��ʾ��ֱ�ӱ������Ǽ�ӱ�����access = ��dir,indir��indir��ʾ�Ǽ�ӱ�����������ڼ�ӱ�������dir��ʾֱ�ӱ���
			AccessKind access;
			int level;
			int off;
		}VarAttr; /*������ʶ��������*/
		struct
		{
			int level;
			ParamTable* param; /*������*/ //�β���Ϣ���ַ
			int code;
			int size;
		}ProcAttr; /*��������ʶ��������*/
	}More; /*��ʶ���Ĳ�ͬ�����в�ͬ������*/
}AttributeIR;

//���ű���
struct symbtable
{
	string idname;
	AttributeIR attrIR;
	struct symbtable* next = nullptr;
}SymbTable;

struct ParamTable {
	symbtable* entry;                 //�βα�ʶ���ڷ��ű��е�λ��
	ParamTable* next = nullptr;
};

//��¼���
struct fieldChain
{
	string idname; //��¼���еı�ʶ����
	typeIR* unitType; //ָ�������͵��ڲ���ʾ��                                                               //�����д���
	int offset; //��ʾ��ǰ��ʶ���ڼ�¼�е�ƫ�ƣ�
	fieldChain* next; //ָ����һ����
};

//��ʶ���ڲ���ʾ
struct typeIR
{
	int size; /*������ռ�ռ��С*/
	TypeKind kind;  //intTy, charTy, arrayTy, recordTy, boolTy
	union
	{
		struct
		{
			struct typeIR* indexTy;
			struct typeIR* elemTy;
		}ArrayAttr;
		fieldChain* body; /*��¼�����е�����*/
	} More;
};

// ��������
//symbtable Scope[10000];//���ű�
//symbtable DestroyScope[10000];//�������ű�
//int destroylevel = 0;
//int level = 0;
//bool flagtree = false;//�����ж��Զ��������������������Ǳ�����������������Ϊfalse
//symbtable* table = nullptr;

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
void Analyze(TreeNode *currentP);