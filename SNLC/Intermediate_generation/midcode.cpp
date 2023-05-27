#include "midcode_g.h"

// ѭ������м�������ɺ���
void GenWhileS(TreeNode* t)
{
	ArgRecord* InLarg = ARGLabel(NewLabel());
	ArgRecord* OutLarg = ARGLabel(NewLabel());
	ArgRecord* Earg = GenExpr(t);
	GenCode(LABEL, InLarg, NULL, NULL);
	GenCode(JUMP0, Earg, OutLarg, NULL);
	GenBody(t);
	GenCode(JUMP, InLarg, NULL, NULL);
	GenCode(LABEL, OutLarg, NULL, NULL);
}

// ��������м�������ɺ���
void GenIfS(TreeNode* t)
{
	ArgRecord* ElseLarg = ARGLabel(NewLabel());
	ArgRecord* OutLarg = ARGLabel(NewLabel());
	ArgRecord* Earg = GenExpr(t);
	GenCode(JUMP0, Earg, ElseLarg, NULL);
	GenBody(t);
	GenBody(JUMP, OutLarg, NULL, NULL);
	GenCode(LABEL, ElseLarg, NULL, NULL);
	GenBody(t);
	GenCode(LABEL, OutLarg, NULL, NULL);
}

// д����м�������ɺ���
void GenWriteS(TreeNode* t)
{
	ArgRecord* Earg = GenExpr(t);
	GenCode(WRITEC, Earg, NULL, NULL);
}

// ������м�������ɺ���
void GenReadS(TreeNode* t)
{
	ArgRecord*  Varg = ARGAddr((*t).name[1], int level, int off, AccessKind access); //(������)
	GenCode(READC,Varg,NULL,NULL);
}

// ���̵�������м�������ɺ���
void GenCallS(TreeNode* t)
{

}

// ���ʽ���м�������ɺ���
ArgRecord* GenExpr(TreeNode* t)     
{
	ArgRecord* arg = NULL;
	if ((*t).kind.exp == IdEK)
	{
		arg = GenVar(t);
	}
	else if ((*t).kind.exp == ConstK)
	{
		arg = ARGValue((*t).attr.expAttr.val);
	}
	else if ((*t).kind.exp == OpK)
	{
		ArgRecord* Larg = GenExpr((*t).child[0]);
		CodeKind op = (*t).attr.expAttr.op;
		ArgRecord * Rarg = GenExpr((*t).child[1]);
		ArgRecord* temp1 =  NewTemp(dir);
		GenCode(op, Larg, Rarg, temp1);
		arg = temp1;
	}
	return arg;
}

// ���Ա�������м��������(�����⣬ƫ��)
ArgRecord* GenField(ArgRecord* V1arg, TreeNode* t, fieldChain* head)
{
	ArgRecord* temp1 = NewTemp(dir);
	GenCode(AADD, Vlarg, offArg, temp1);

}

//����������м�������ɺ���
ArgRecord* GenArray(ArgRecord* Vlarg, TreeNode* t, int low, int size)
{
	ArgRecord* Earg = GenExpr(t);
	ArgRecord* lowArg = ARGValue(low);
	ArgRecord* sizeArg = ARGValue(size);
	ArgRecord* temp1 = NewTemp(dir); 
	ArgRecord* temp2 = NewTemp(dir);
	ArgRecord* temp3 = NewTemp(dir);
	GenCode(SUB, Earg, lowArg, temp1);
	GenCode(MULT, temp1, sizeArg, temp2);
	GenCode(AADD, Vlarg, temp2, temp3);
	return temp3;
}

// �����м�������ɺ���
ArgRecord* GenVar(TreeNode* t)
{
	ArgRecord* Varg = NULL;
	if ()
	{

	}
	else if ((*t).kind.dec == ArrayK)
	{
		Varg = GenArray()
	}
	else if ((*t).kind.dec == RecordK)
	{

	}
	return Varg;
}

// ��ֵ����м�������ɺ���
void GenAssignS(TreeNode* t)
{
	ArgRecord* Larg = GenVar((*t).child[0]);
	ArgRecord* Rarg = GenExpr((*t).child[1]);
	GenCode(ASSIGN, Larg, Rarg, NULL);
}

// �����м�������ɺ���(�������)
void GenStatement(TreeNode* t)
{
	if ((*t).kind.stmt == AssignK) GenAssignS(t);
	else if ((*t).kind.stmt == CallK) GenCallS(t);
	else if ((*t).kind.stmt == ReadK) GenReadS(t);
	else if ((*t).kind.stmt == WriteK) GenWriteS(t);
	else if ((*t).kind.stmt == IfK) GenIfS(t);
	else if ((*t).kind.stmt == WhileK) GenWhileS(t);
	else if((*t).kind.stmt == ReturnK) GenCode(RETURNC,NULL,NULL,NULL);
	else
	{
		//error
	}
}

// ������м�������ɺ���
void GenBody(TreeNode* t)
{
	while (!t)
	{
		GenStatement(t);
		t = (*t).sibling;
	}
}

