//#include "midcode_g.h"
//extern symbtable Scope[10000];
//
//// �м��������������
//CodeFile* GenMidCode(TreeNode* t) {
//	TreeNode* tmp = t;
//	tmp = t->child[1];
//	while (tmp->nodekind != ProcDecK) {
//		tmp = tmp->sibling;
//	}
//	if (tmp != NULL) tmp = tmp->child[0];
//	while (tmp != NULL) {
//		GenProcDec(tmp);
//		tmp = tmp->sibling;
//
//	}
//	//������ڴ���
//	tmp = t->child[2];
//	ArgRecord* arg2 = ARGValue(initOff);
//	ArgRecord* arg3 = ARGValue(0);//ƫ����
//	CodeFile* c = GenCode(MENTRY, NULL, arg2, arg3);
//
//	tmp_num = 0;//���¼��һ����ʱ������ƫ��???
//
//	GenBody(tmp);
//
//	//���¼�Ĵ�С����������arg2
//	arg2->value = Scope[tmp->table[0]].attrIR.More.ProcAttr.size;
//
//	return head;
//}
//
//// ���������м�������ɺ���
//void GenProcDec(TreeNode* t) {
//	int ProcEny = NewLabel();
//
//	Scope[t->table[0]].attrIR.More.ProcAttr.code;//������ڱ�Ż�����ű���̱�ʶ��������
//
//	ArgRecord* arg1 = ARGValue(ProcEny);
//	ArgRecord* arg2 = ARGValue(Scope[t->table[0]].attrIR.More.ProcAttr.level);//����
//	ArgRecord* arg3 = ARGValue(Scope[t->table[0]].attrIR.More.ProcAttr.code);//ƫ���� ?????????
//
//	//������������еĹ�������
//	TreeNode* tmp = t->child[1];
//	while (tmp->nodekind != ProcDecK) {
//		tmp = tmp->sibling;
//	}
//	if (tmp != NULL) tmp = tmp->child[0];
//	while (tmp != NULL) {
//		GenProcDec(tmp);
//		tmp = tmp->sibling;
//	}
//
//	CodeFile* c = GenCode(PENTRY, arg1, arg2, arg3);
//
//	//��ʼ���˹�����ʱ�����Ŀ�ʼ���Ϊ���̻��¼�е�һ����ʱ������ƫ��
//
//	GenBody(t->child[2]);
//	//�õ����̻��¼�Ĵ�С���������������м������
//
//	CodeFile* c = GenCode(ENDPROC, NULL, NULL, NULL);
//}
//
//// ѭ������м�������ɺ���
//void GenWhileS(TreeNode* t)
//{
//	ArgRecord* InLarg = ARGLabel(NewLabel());
//	ArgRecord* OutLarg = ARGLabel(NewLabel());
//	ArgRecord* Earg = GenExpr((*t).child[0]);
//	GenCode(LABEL, InLarg, NULL, NULL);
//	GenCode(JUMP0, Earg, OutLarg, NULL);
//	GenBody((*t).child[1]);
//	GenCode(JUMP, InLarg, NULL, NULL);
//	GenCode(LABEL, OutLarg, NULL, NULL);
//}
//
//// ��������м�������ɺ���
//void GenIfS(TreeNode* t)
//{
//	ArgRecord* ElseLarg = ARGLabel(NewLabel());
//	ArgRecord* OutLarg = ARGLabel(NewLabel());
//	ArgRecord* Earg = GenExpr((*t).child[0]);
//	GenCode(JUMP0, Earg, ElseLarg, NULL);
//	GenBody((*t).child[1]);
//	GenCode(JUMP, OutLarg, NULL, NULL);
//	GenCode(LABEL, ElseLarg, NULL, NULL);
//	GenBody((*t).child[2]);
//	GenCode(LABEL, OutLarg, NULL, NULL);
//}
//
//
//// д����м�������ɺ���
//void GenWriteS(TreeNode* t)
//{
//	ArgRecord* Earg = GenExpr((*t).child[0]);
//	GenCode(WRITEC, Earg, NULL, NULL);
//}
//
//
//// ������м�������ɺ���
//void GenReadS(TreeNode* t)
//{
//	symbtable s = Scope[(*t).table[0]];
//	ArgRecord* Varg = ARGAddr(s.idname, s.attrIR.More.VarAttr.level, s.attrIR.More.VarAttr.off, s.attrIR.More.VarAttr.access);
//	GenCode(READC, Varg, NULL, NULL);
//}
//
//// ���̵�������м�������ɺ���
//void GenCallS(TreeNode* t)
//{
//	//�����ڵ�ַ��??
//	int EntryAddr = Scope[t->table[0]].attrIR.More.ProcAttr.code;
//
//	ParamTable* p = Scope[t->table[0]].attrIR.More.ProcAttr.param;
//
//	TreeNode* tmp = t->child[1];
//	while (tmp != NULL) {
//		ArgRecord* Earg = GenExpr(tmp);
//		ArgRecord* Rarg;//�βε�ƫ��
//		if (p->entry->attrIR.More.VarAttr.access==(AccessKind)dir) {
//			Rarg = ARGValue(p->entry->attrIR.More.VarAttr.off);
//			GenCode(VALACT, Earg, Rarg, NULL);
//
//		}
//		else {
//			Rarg = ARGValue(p->entry->attrIR.More.VarAttr.off);
//			GenCode(VARACT, Earg, Rarg, NULL);
//		}
//
//		p = p->next;
//		tmp = tmp->sibling;
//	}
//
//	ArgRecord* labelarg = ARGLabel(EntryAddr);//���
//	ArgRecord* pOff = ARGValue(1);//����ƫ����
//	GenCode(CALL, labelarg, NULL, pOff); //????????????????
//
//}
//
//// ���ʽ���м�������ɺ���
//ArgRecord* GenExpr(TreeNode* t)
//{
//	ArgRecord* arg = NULL;
//	if ((*t).kind.exp == IdEK)
//	{
//		arg = GenVar(t);
//	}
//	else if ((*t).kind.exp == ConstK)
//	{
//		arg = ARGValue((*t).attr.expAttr.val);
//	}
//	else if ((*t).kind.exp == OpK)
//	{
//		ArgRecord* Larg = GenExpr((*t).child[0]);
//		CodeKind op = (*t).attr.expAttr.op;
//		ArgRecord* Rarg = GenExpr((*t).child[1]);
//		ArgRecord* temp1 = NewTemp(dir);
//		GenCode(op, Larg, Rarg, temp1);
//		arg = temp1;
//	}
//	return arg;
//}
//
//// ���Ա�������м��������(�����⣬ƫ��)
//ArgRecord* GenField(ArgRecord* V1arg, TreeNode* t, fieldChain* head)
//{
//	ArgRecord* offArg = ARGAddr(head->idname, Scope[t->table[0]].attrIR.More.VarAttr.level, head->offset, Scope[t->table[0]].attrIR.More.VarAttr.access);
//	ArgRecord* temp1 = NewTemp(dir);
//	GenCode(AADD, V1arg, offArg, temp1);
//	ArgRecord* FieldV = NULL;
//	if (head->unitType->kind == arrayTy)
//	{
//		GenArray(temp1, t, t->attr.arrayAttr.low, t->attr.arrayAttr.up - t->attr.arrayAttr.low);
//	}
//	else FieldV = temp1;
//	return FieldV;
//
//}
//
////����������м�������ɺ����������±���ʽ��
//ArgRecord* GenArray(ArgRecord* Vlarg, TreeNode* t, int low, int size)
//{
//	ArgRecord* Earg = GenExpr((*t).child[0]);       //����һ
//	ArgRecord* lowArg = ARGValue(low);
//	ArgRecord* sizeArg = ARGValue(size);
//	ArgRecord* temp1 = NewTemp(dir);
//	ArgRecord* temp2 = NewTemp(dir);
//	ArgRecord* temp3 = NewTemp(dir);
//	GenCode(SUB, Earg, lowArg, temp1);
//	GenCode(MULT, temp1, sizeArg, temp2);
//	GenCode(AADD, Vlarg, temp2, temp3);
//	return temp3;
//}
//
//// �����м�������ɺ���
//ArgRecord* GenVar(TreeNode* t)
//{
//	string id = t->name[0];
//	AccessKind acc = (AccessKind)Scope[t->table[0]].attrIR.More.VarAttr.access;
//	int lev = Scope[t->table[0]].attrIR.More.VarAttr.level;
//	int off = Scope[t->table[0]].attrIR.More.VarAttr.off;
//	ArgRecord* Vlarg = ARGAddr(id, lev, off, acc);
//	ArgRecord* Varg = NULL;
//	if (t->attr.expAttr.varkind== IdV)
//	{
//		Varg = Vlarg;
//	}
//	else if (t->attr.expAttr.varkind == ArrayMembV)
//	{
//		int _size = Scope[t->table[0]].attrIR.More.VarAttr.off;
//		int low = Scope[t->table[0]].attrIR.More.VarAttr.level;
//		Varg = GenArray(Vlarg, t->child[0], low, _size);
//	}
//	else if (t->attr.expAttr.varkind == FieldMembV)
//	{
//		fieldChain* ff = Scope[t->table[0]].attrIR.idtype->More.body;//�Ƿ���t
//		Varg = GenField(Vlarg, t->child[0], ff);
//	}
//	return Varg;
//}
//
//// ��ֵ����м�������ɺ���
//void GenAssignS(TreeNode* t)
//{
//	ArgRecord* Larg = GenVar((*t).child[0]);
//	ArgRecord* Rarg = GenExpr((*t).child[1]);
//	GenCode(ASSIG, Larg, Rarg, NULL);
//}
//// �����м�������ɺ���(����������ᴦ��)
//void GenStatement(TreeNode* t)
//{
//	if ((*t).kind.stmt == AssignK) GenAssignS(t);
//	else if ((*t).kind.stmt == CallK) GenCallS(t);
//	else if ((*t).kind.stmt == ReadK) GenReadS(t);
//	else if ((*t).kind.stmt == WriteK) GenWriteS(t);
//	else if ((*t).kind.stmt == IfK) GenIfS(t);
//	else if ((*t).kind.stmt == WhileK) GenWhileS(t);
//	else if ((*t).kind.stmt == ReturnK) GenCode(RETURNC, NULL, NULL, NULL);
//	else
//	{
//		//error
//	}
//}
//
//// ������м�������ɺ���
//void GenBody(TreeNode* t)
//{
//	while (t)
//	{
//		GenStatement(t);
//		t = (*t).sibling;
//	}
//}
//
//
///*---------=======================-----------*/
//
//
//// 
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
