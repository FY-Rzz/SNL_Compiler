#include "midcode_g.h"
 int tmp_num;
 int label_num;
 CodeFile* head, * tail;

// �м��������������
CodeFile* GenMidCode(TreeNode* t) {
	tmp_num = 1;
	TreeNode* tmp = t;
	tmp = t->child[1];
	while (tmp->nodekind != ProcDecK) {
		tmp = tmp->sibling;
	}
	//if (tmp != NULL) tmp = tmp->child[0];

	//����Ҫ��
	while (tmp != NULL) {
		GenProcDec(tmp);
		tmp = tmp->sibling;

	}

	//������ڴ���
	tmp = t->child[2];
	ArgRecord* arg2 = ARGValue(initOff);
	ArgRecord* arg3 = ARGValue(34);//ƫ����
	CodeFile* c = GenCode(MENTRY, NULL, arg2, arg3);

	tmp_num ++;//���¼��һ����ʱ������ƫ��???
	GenBody(tmp->child[0]);

	//���¼�Ĵ�С����������arg2
	//arg2->value = tmp->table[0]->attrIR.More.ProcAttr.size;
	PrintMidCode(head);
	return head;
}

// ���������м�������ɺ���
void GenProcDec(TreeNode* t) {
	int ProcEny = NewLabel();

	t->table[0]->attrIR.More.ProcAttr.code = ProcEny;//������ڱ�Ż�����ű���̱�ʶ��������

	ArgRecord* arg1 = ARGValue(ProcEny);
	ArgRecord* arg2 = ARGValue(t->table[0]->attrIR.More.ProcAttr.level);//����
	ArgRecord* arg3 = ARGValue(t->table[0]->attrIR.More.ProcAttr.code);//ƫ���� ?????????

	//������������еĹ�������
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

	//��ʼ���˹�����ʱ�����Ŀ�ʼ���Ϊ���̻��¼�е�һ����ʱ������ƫ��

	GenBody(t->child[2]->child[0]);
	//�õ����̻��¼�Ĵ�С���������������м������

	CodeFile* b = GenCode(ENDPROC, NULL, NULL, NULL);
}

// ѭ������м�������ɺ���
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

// ��������м�������ɺ���
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


// д����м�������ɺ���
void GenWriteS(TreeNode* t)
{
	ArgRecord* Earg = GenExpr((*t).child[0]);
	GenCode(WRITEC, Earg, NULL, NULL);
}


// ������м�������ɺ���
void GenReadS(TreeNode* t)
{
	symbtable* s = (*t).table[0];
	ArgRecord* Varg = ARGAddr(s->idname, s->attrIR.More.VarAttr.level, s->attrIR.More.VarAttr.off, s->attrIR.More.VarAttr.access);
	GenCode(READC, Varg, NULL, NULL);
}

// ���̵�������м�������ɺ���
void GenCallS(TreeNode* t)
{
	//�����ڵ�ַ��??
	int EntryAddr = t->child[0]->table[0]->attrIR.More.ProcAttr.code;
	//int EntryAddr = 111;

	ParamTable* p = t->child[0]->table[0]->attrIR.More.ProcAttr.param;

	TreeNode* tmp = t->child[1];
	while (tmp != NULL) {
		ArgRecord* Earg = GenExpr(tmp);
		ArgRecord* Rarg;//�βε�ƫ��
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

	ArgRecord* labelarg = ARGLabel(EntryAddr);//���
	ArgRecord* pOff = ARGValue(1);//����ƫ����
	GenCode(CALL, labelarg, NULL, pOff); //????????????????

}

// ���ʽ���м�������ɺ���
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

// ���Ա�������м��������
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

//����������м�������ɺ����������±���ʽ��
ArgRecord* GenArray(ArgRecord* Vlarg, TreeNode* t, int low, int size)
{
	ArgRecord* Earg = GenExpr((*t).child[0]);       //����һ
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
		fieldChain* ff = t->table[0]->attrIR.idtype->More.body;//�Ƿ���t
		Varg = GenField(Vlarg, t->child[0], ff);
	}
	return Varg;
}

// ��ֵ����м�������ɺ���
void GenAssignS(TreeNode* t)
{
	ArgRecord* Larg = GenVar((*t).child[0]);
	ArgRecord* Rarg = GenExpr((*t).child[1]);
	GenCode(ASSIG, Larg, Rarg, NULL);
}
// �����м�������ɺ���(����������ᴦ��)
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

// ������м�������ɺ���
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


 















