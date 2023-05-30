#include "midcode_g.h"
 int tmp_num;
 int label_num;
 CodeFile* head, * tail;

// 中间代码生成主函数
CodeFile* GenMidCode(TreeNode* t) {
	tmp_num = 1;
	TreeNode* tmp = t;
	tmp = t->child[1];
	while (tmp->nodekind != ProcDecK) {
		tmp = tmp->sibling;
	}
	//if (tmp != NULL) tmp = tmp->child[0];

	//可能要改
	while (tmp != NULL) {
		GenProcDec(tmp);
		tmp = tmp->sibling;

	}

	//生成入口代码
	tmp = t->child[2];
	ArgRecord* arg2 = ARGValue(initOff);
	ArgRecord* arg3 = ARGValue(34);//偏移量
	CodeFile* c = GenCode(MENTRY, NULL, arg2, arg3);

	tmp_num ++;//活动记录第一个临时变量的偏移???
	GenBody(tmp->child[0]);

	//活动记录的大小？？？回填arg2
	//arg2->value = tmp->table[0]->attrIR.More.ProcAttr.size;
	PrintMidCode(head);
	return head;
}

// 过程声明中间代码生成函数
void GenProcDec(TreeNode* t) {
	int ProcEny = NewLabel();

	t->table[0]->attrIR.More.ProcAttr.code = ProcEny;//过程入口标号回填到符号表过程标识符属性中

	ArgRecord* arg1 = ARGValue(ProcEny);
	ArgRecord* arg2 = ARGValue(t->table[0]->attrIR.More.ProcAttr.level);//层数
	ArgRecord* arg3 = ARGValue(t->table[0]->attrIR.More.ProcAttr.code);//偏移量 ?????????

	//处理过程声明中的过程声明
	TreeNode* tmp = t->child[1];
	while (tmp && tmp->nodekind != ProcDecK) {
		tmp = tmp->sibling;
	}
	if (tmp != NULL) tmp = tmp->child[0];
	while (tmp != NULL) {
		GenProcDec(tmp);
		tmp = tmp->sibling;
	}

	CodeFile* c = GenCode(PENTRY, arg1, arg2, arg3);

	//初始化此过程临时变量的开始标号为过程活动记录中第一个临时变量的偏移

	GenBody(t->child[2]->child[0]);
	//得到过程活动记录的大小，回填入过程入口中间代码中

	CodeFile* b = GenCode(ENDPROC, NULL, NULL, NULL);
}

// 循环语句中间代码生成函数
void GenWhileS(TreeNode* t)
{
	ArgRecord* InLarg = ARGLabel(NewLabel());
	ArgRecord* OutLarg = ARGLabel(NewLabel());
	ArgRecord* Earg = GenExpr((*t).child[0]);
	GenCode(WHILESTART, InLarg, NULL, NULL);
	GenCode(JUMP0, Earg, OutLarg, NULL);
	GenBody((*t).child[1]);
	GenCode(JUMP, InLarg, NULL, NULL);
	GenCode(ENDWHILE, OutLarg, NULL, NULL);
}

// 条件语句中间代码生成函数
void GenIfS(TreeNode* t)
{
	ArgRecord* ElseLarg = ARGLabel(NewLabel());
	ArgRecord* OutLarg = ARGLabel(NewLabel());
	ArgRecord* Earg = GenExpr((*t).child[0]);
	GenCode(JUMP0, Earg, ElseLarg, NULL);
	GenBody((*t).child[1]);
	GenCode(JUMP, OutLarg, NULL, NULL);
	GenCode(LABEL, ElseLarg, NULL, NULL);
	GenBody((*t).child[2]);
	GenCode(LABEL, OutLarg, NULL, NULL);
}


// 写语句中间代码生成函数
void GenWriteS(TreeNode* t)
{
	ArgRecord* Earg = GenExpr((*t).child[0]);
	GenCode(WRITEC, Earg, NULL, NULL);
}


// 读语句中间代码生成函数
void GenReadS(TreeNode* t)
{
	symbtable* s = (*t).table[0];
	ArgRecord* Varg = ARGAddr(s->idname, s->attrIR.More.VarAttr.level, s->attrIR.More.VarAttr.off, s->attrIR.More.VarAttr.access);
	GenCode(READC, Varg, NULL, NULL);
}

// 过程调用语句中间代码生成函数
void GenCallS(TreeNode* t)
{
	//获得入口地址，??
	int EntryAddr = t->child[0]->table[0]->attrIR.More.ProcAttr.code;
	//int EntryAddr = 111;

	ParamTable* p = t->child[0]->table[0]->attrIR.More.ProcAttr.param;

	TreeNode* tmp = t->child[1];
	while (tmp != NULL) {
		ArgRecord* Earg = GenExpr(tmp);
		ArgRecord* Rarg;//形参的偏移
		if (p->entry->attrIR.More.VarAttr.access==(AccessKind)dir) {
			Rarg = ARGValue(p->entry->attrIR.More.VarAttr.off);
			GenCode(VALACT, Earg, Rarg, NULL);

		}
		else {
			Rarg = ARGValue(p->entry->attrIR.More.VarAttr.off);
			GenCode(VARACT, Earg, Rarg, NULL);
		}

		p = p->next;
		tmp = tmp->sibling;
	}

	ArgRecord* labelarg = ARGLabel(EntryAddr);//入口
	ArgRecord* pOff = ARGValue(1);//过程偏移量
	GenCode(CALL, labelarg, NULL, pOff); //????????????????

}

// 表达式的中间代码生成函数
ArgRecord* GenExpr(TreeNode* t)
{
	ArgRecord* arg = NULL;
	if (!t) return NULL;
	if ((*t).nodekind == ExpK) {
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
			CodeKind op;
			LexType opp = (*t).attr.expAttr.op;   //ADD, SUB, MULT, DIV,EQC, LTC, 
			if (opp == PLUS) op = ADD;
			else if (opp == MINUS) op = SUB;
			else if (opp == TIMES) op = MULT;
			else if (opp == OVER) op = DIV;
			else if (opp == EQ) op = EQC;
			else if (opp == LT) op = LTC;
			ArgRecord* Rarg = GenExpr((*t).child[1]);
			ArgRecord* temp1 = NewTemp(dir);
			GenCode(op, Larg, Rarg, temp1);
			arg = temp1;
		}
	}
	return arg;
}

// 域成员变量的中间代码生成
ArgRecord* GenField(ArgRecord* V1arg, TreeNode* t, fieldChain* head)
{
	ArgRecord* offArg = ARGAddr(head->idname, 1, 1, dir);
	ArgRecord* temp1 = NewTemp(dir);
	GenCode(AADD, V1arg, offArg, temp1);
	ArgRecord* FieldV = NULL;
	ArgRecord* Field = NULL;
	if (head->unitType->kind == arrayTy)
	{
		GenArray(temp1, t, t->attr.arrayAttr.low, t->attr.arrayAttr.up - t->attr.arrayAttr.low + 1);
	}
	else FieldV = temp1;
	return Field;

}

//数组变量的中间代码生成函数（处理下标表达式）
ArgRecord* GenArray(ArgRecord* Vlarg, TreeNode* t, int low, int size)
{
	ArgRecord* Earg = GenExpr((*t).child[0]);       //问题一
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
	string id = t->name[0];


	AccessKind acc = (AccessKind)t->table[0]->attrIR.More.VarAttr.access;
	int lev = t->table[0]->attrIR.More.VarAttr.level;
	int off = t->table[0]->attrIR.More.VarAttr.off;
	ArgRecord* Vlarg = ARGAddr(id, lev, off, acc);
	ArgRecord* Varg = NULL;
	if (t->attr.expAttr.varkind== IdV)
	{
		Varg = Vlarg;
	}
	else if (t->attr.expAttr.varkind == ArrayMembV)
	{
		int _size = t->table[0]->attrIR.More.VarAttr.off;
		int low = t->table[0]->attrIR.More.VarAttr.level;
		Varg = GenArray(Vlarg, t->child[0], low, _size);
	}
	else if (t->attr.expAttr.varkind == FieldMembV)
	{
		fieldChain* ff = t->table[0]->attrIR.idtype->More.body;//是否是t
		Varg = GenField(Vlarg, t->child[0], ff);
	}
	return Varg;
}

// 赋值语句中间代码生成函数
void GenAssignS(TreeNode* t)
{
	ArgRecord* Larg = GenVar((*t).child[0]);
	ArgRecord* Rarg = GenExpr((*t).child[1]);
	GenCode(ASSIG, Larg, Rarg, NULL);
}
// 语句的中间代码生成函数(错误情况不会处理)
void GenStatement(TreeNode* t)
{
	if ((*t).kind.stmt == AssignK) GenAssignS(t);
	else if ((*t).kind.stmt == CallK) GenCallS(t);
	else if ((*t).kind.stmt == ReadK) GenReadS(t);
	else if ((*t).kind.stmt == WriteK) GenWriteS(t);
	else if ((*t).kind.stmt == IfK) GenIfS(t);
	else if ((*t).kind.stmt == WhileK) GenWhileS(t);
	else if ((*t).kind.stmt == ReturnK) GenCode(RETURNC, NULL, NULL, NULL);
	else
	{
		//error
	}
}

// 语句体中间代码生成函数
void GenBody(TreeNode* t)
{
	TreeNode* tmp = t;
	while (tmp)
	{
		GenStatement(tmp);
		tmp = (*tmp).sibling;
	}
}


/*---------=======================-----------*/


 















