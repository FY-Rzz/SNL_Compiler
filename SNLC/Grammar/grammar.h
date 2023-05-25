#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <cstdlib>
#include <stack>

#include "../Lexical/lexical.h"
using namespace std;

/*******************ö������************************/
enum NodeKindEnum {	//�﷨���ڵ����ͷ�ö��
	ProK,				//����־�ڵ�
	PheadK,			//����ͷ��־�ڵ�
	TypeK,				//����������־�ڵ�
	VarK,				//	����������־�ڵ�
	ProcDecK,		//	����������־�ڵ�
	StmLK,				//������б�־�ڵ�
	DecK,				//�����ڵ�
	StmtK,				//���ڵ�
	ExpK					//���ʽ�ڵ�
};
enum DeckEnum {	//�����ڵ�����ö��
	ArrayK,				//��������
	CharK,				//�ַ�����
	IntegerK,			//��������
	RecordK,			//��¼����
	IdK					//�����ͱ�־����Ϊ����
};
enum StmtKEnum {	//���ڵ�����ö��
	IfK,					//�ж��������
	WhileK,				//ѭ���������
	AssignK,			//��ֵ�������
	ReadK,				//���������
	WriteK,				//д�������
	CallK,				//���������������
	ReturnK			//�����������
};
enum ExpKEnum {	//	���ʽ�ڵ�����ö��
	OpK,					//	����������
	ConstK,			//����������
	IdEK					//��ʶ������
};
enum ParamtEnum {	//������ֵ�λ��Ǳ��
	Valparamtype,
	Varparamtype
};
enum VarKindEnum {	//���������,�ֱ��ʾ�����Ǳ�־�������������Ա�����������Ա����
	IdV,
	ArrayMembV,
	FieldMembV
};

enum ExpType {
	Void, Integer, Boolean
};
/*******************����������************************/
union Kind {
	DeckEnum dec;		//��¼�﷨���ڵ����������,��nodekind=DecKʱ��Ч
	StmtKEnum stmt;		//��¼�﷨���ڵ���������,��nodekind=StmtKʱ��Ч
	ExpKEnum exp;		//��¼�﷨���ڵ�ı��ʽ����,��nodekind=ExpKʱ��Ч
};
/*******************�ṹ������************************/
struct ArrayAttr {	//��¼�������͵����ԡ�
	int low;
	int up;
	DeckEnum childType;
};
struct ProcAttr {	//��¼���̵����ԡ�
	ParamtEnum paramt;
};
struct ExpAttr {	//��¼���ʽ�����ԡ�
	LexType op;	//��¼�﷨���ڵ����������ʣ�Ϊ��������
	int val;		//��¼�﷨���ڵ����ֵ,
	VarKindEnum varkind;	//��¼���������
	ExpType type;	//��¼�﷨���ڵ�ļ�����ͣ�ȡֵVoid, Integer,Boolean,Ϊ���ͼ��ExpType����
};
struct Attr {		//��¼�﷨���ڵ���������
	ArrayAttr arrayAttr;
	ProcAttr procAttr;
	ExpAttr expAttr;
};
struct TreeNode
{
	TreeNode* child[3];	//ָ�����﷨���ڵ�ָ��,0�ǳ���ͷ,1��������2�ǳ�����
	TreeNode* sibling;	//ָ���ֵ��﷨���ڵ�ָ��
	int lineno;					//��¼Դ�����к�
	NodeKindEnum nodekind;//	��¼�﷨���ڵ�����
	Kind kind;					//��¼�﷨���ڵ�ľ�������
	int idnum = 0;				//��¼һ���ڵ��б�־������
	vector<string> name;	//������Ա�ǽڵ��еı�־��������
	vector<int*> table;	//	������Ա�ǽڵ��еĸ�����־���ڷ��ű��е����
	string type_name;	//��¼�����������ڵ�Ϊ���������������������ͱ�־����ʾ����Ч
	Attr attr;				//��¼�﷨���ڵ���������
};
