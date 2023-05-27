#include "midcode_g.h"

// 循环语句中间代码生成函数
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

// 条件语句中间代码生成函数
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

// 写语句中间代码生成函数
void GenWriteS(TreeNode* t)
{
	ArgRecord* Earg = GenExpr(t);
	GenCode(WRITEC, Earg, NULL, NULL);
}

// 读语句中间代码生成函数
void GenReadS(TreeNode* t)
{
	ArgRecord*  Varg = ARGAddr((*t).name[1], int level, int off, AccessKind access); //(有问题)
	GenCode(READC,Varg,NULL,NULL);
}

// 过程调用语句中间代码生成函数
void GenCallS(TreeNode* t)
{

}

// 表达式的中间代码生成函数
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

// 域成员变量的中间代码生成(有问题，偏移)
ArgRecord* GenField(ArgRecord* V1arg, TreeNode* t, fieldChain* head)
{
	ArgRecord* temp1 = NewTemp(dir);
	GenCode(AADD, Vlarg, offArg, temp1);

}

//数组变量的中间代码生成函数
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

// 变量中间代码生成函数
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

// 赋值语句中间代码生成函数
void GenAssignS(TreeNode* t)
{
	ArgRecord* Larg = GenVar((*t).child[0]);
	ArgRecord* Rarg = GenExpr((*t).child[1]);
	GenCode(ASSIGN, Larg, Rarg, NULL);
}

// 语句的中间代码生成函数(错误情况)
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

// 语句体中间代码生成函数
void GenBody(TreeNode* t)
{
	while (!t)
	{
		GenStatement(t);
		t = (*t).sibling;
	}
}

