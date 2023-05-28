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
};

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
		} ArrayAttr;
		fieldChain* body; /*��¼�����е�����*/
	} More;
};

// ��������
const int initOff = 7;
extern int level;
extern int off;
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
bool Enter(string Id, AttributeIR* AttribP, symbtable** Entry);

// Ѱ�ұ����ַ
bool FindEntry(string id, bool flag, symbtable** Entry);

// �ӱ�ͷ��ʼ�����ν��ڵ��еı�ʶ�����ֺ� id �Ƚ��Ƿ���ͬ
bool SearchoneTable(string id, int currentLevel, symbtable** Entry);

//������в�������
bool FindField(string Id, fieldChain* head, fieldChain** Entry);

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
void printSymbTable(string path);


// ��ӡ���ű��һ��
void printOneLayer();

/*---Analyze.cpp---*/

// ��ʼ����������
void initialize();

// �������ͷ���
typeIR* TypeProcess(TreeNode* t);

// �����Զ���������
typeIR* nameType(TreeNode* t);

// �����������͵��ڲ���ʾ
typeIR* arrayType(TreeNode* t);

// �����¼���͵��ڲ���ʾ
typeIR* recordType(TreeNode* t);

// ����������������
void TypeDecPart(TreeNode* t);

// ���������������
void VarDecList(TreeNode* t);

// ���������������
void ProcDecPart(TreeNode* t);

// �������ͷ���������
void HeadProcess();

// ��������������������
ParamTable* ParaDecList(TreeNode* t);

// ����ִ���岿�ֵ��������
void Body(TreeNode* t);

// �������״̬
void statement(TreeNode* t);

// ������ʽ
typeIR* Expr(TreeNode* t, AccessKind* Ekind);

// �����±����
typeIR* arrayVar(TreeNode* t);

// ���������
typeIR* recordVar(TreeNode* t);

// ����ֵ���
void assignstatement(TreeNode* t);

// ����������
void callstatement(TreeNode* t);

// �����������
void ifstatement(TreeNode* t);

// ����ѭ�����
void whilestatement(TreeNode* t);

// ��������
void readstatement(TreeNode* t);

// ����д���
void writestatement(TreeNode* t);

// ���������
void returnstatement(TreeNode* t);

// �﷨����������
void Analyze(TreeNode *currentP);

//�����﷨����������Ӧ�ĺ��������﷨�����
void bianli(TreeNode* t);