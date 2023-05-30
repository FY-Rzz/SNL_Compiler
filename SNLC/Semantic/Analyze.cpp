#include "semantic.h"

typeIR* intPtr, * charPtr, * boolPtr;
bool flagtree = false;

// �������������
void Analyze(TreeNode* root)
{
	cout << "�����������..." << endl;
	CreateTable();
	initialize();
	// ѭ����������������
	// ѭ��������������
	bianli(root);
	// ��ӡ���ű�
	printSymbTable("./Docs/symbtable.txt");
	cout << "����������" << endl;
	return;
}

void initialize()
{
	intPtr = new typeIR;
	intPtr->size = 1;
	intPtr->kind = intTy;

	charPtr = new typeIR;
	charPtr->size = 1;
	charPtr->kind = charTy;

	boolPtr = new typeIR;
	boolPtr->size = 1;
	boolPtr->kind = boolTy;
}

//�Զ��������ڲ��ṹ��������
typeIR* nameType(TreeNode* t) {
	symbtable** Entry = new symbtable*;
	*Entry = nullptr;
	int i = 0;
	if (flagtree)i++;
	bool flag = FindEntry(t->name[i], false, Entry);
	t->table.push_back(*Entry);
	if (!flag) 
		cout << t->lineno << "�Զ�������" + t->name[i] + "δ����" << endl;
	else 
		return (*Entry)->attrIR.idtype;

	return nullptr;
}
//���������ڲ���ʾ������
typeIR* arrayType(TreeNode* t) {
	if (t->attr.arrayAttr.low > t->attr.arrayAttr.up) {
		cout << "�����±�Խ��" << endl;
	}
	else {
		typeIR* newarrayType = new typeIR;
		newarrayType->kind = arrayTy;
		newarrayType->size = (t->attr.arrayAttr.up) - (t->attr.arrayAttr.low);
		newarrayType->More.ArrayAttr.indexTy = intPtr;
		if (t->attr.arrayAttr.childType == CharK)
			newarrayType->More.ArrayAttr.elemTy = charPtr;
		else newarrayType->More.ArrayAttr.elemTy = intPtr;
		return newarrayType;
	}
	return nullptr;
}
void outputbody(fieldChain* bodyt) {
	if (bodyt == nullptr)
		return;
	else {
		cout << "  " << bodyt->idname;
		outputbody(bodyt->next);
		return;
	}
}
//�����¼���͵��ڲ���ʾ����
typeIR* recordType(TreeNode* t) {
	typeIR* newrecordType = new typeIR;
	newrecordType->kind = recordTy;
	fieldChain* newbody = nullptr, * dt = nullptr;
	TreeNode* nt = t->child[0];
	int recordoffset = 0;
	while (nt != nullptr) {
		for (int i = 0; i < nt->name.size(); i++) {
			if (newbody != nullptr) {
				fieldChain** Entry = new fieldChain*;
				(*Entry) = nullptr;
				bool flag = FindField(nt->name[i], newbody, Entry);
				if (flag)
					cout << t->lineno << "record������" << endl;
			}
			fieldChain* newt = new fieldChain;
			if (newbody == nullptr)
				newbody = newt;
			else {
				dt->next = newt;
			}
			newt->unitType = TypeProcess(nt);
			newt->idname = nt->name[i];
			newt->offset = recordoffset + newt->unitType->size;
			newt->next = nullptr;
			dt = newt;
		}
		nt = nt->sibling;
	}
	dt->next = nullptr;
	newrecordType->More.body = newbody;
	//cout << newbody->next;
	//outputbody(newbody);cout<<endl;
	return newrecordType;
}

//���ͷ��������������ͷ������������﷨���ĵ�ǰ�ڵ����ͣ����쵱ǰ���͵��ڲ�
//��ʾ���������ַ���ظ�Ptr�����ڲ���ʾ�ĵ�ַ��
typeIR* TypeProcess(TreeNode* t) {
	if (t->kind.dec == ArrayK)
		return arrayType(t);
	else if (t->kind.dec == CharK)
		return charPtr;
	else if (t->kind.dec == IntegerK)
		return intPtr;
	else if (t->kind.dec == RecordK)
		return recordType(t);
	else if (t->kind.dec == IdK)
		return nameType(t);
	return nullptr;
}

//�����������ַ���������
void TypeDecPart(TreeNode* t) {
	if (t == nullptr)
		return;

	//����д�������
	symbtable** Entry = new symbtable*;
	*Entry = nullptr;
	int i = 0;
	if (t->kind.dec == IdK) i++;
	for (; i < t->name.size(); i++) {
		bool flag = FindEntry(t->name[0], false, Entry);
		if (flag)
			cout << t->lineno << "���ͱ�ʶ��" + t->name[0] + "�ظ�����" << endl;
		else {
			AttributeIR* a = new AttributeIR;
			a->kind = typeKind;
			flagtree = true;
			a->idtype = TypeProcess(t);
			flagtree = false;
			Enter(t->name[0], a, Entry);
			// ����
			t->table.push_back(*Entry);
		}
	}TypeDecPart(t->sibling);
	return;
}
//�����������ַ���������
void VarDecList(TreeNode* t) {
	if (t == nullptr)
		return;
	//����д�������
	symbtable** Entry = new symbtable*;
	*Entry = nullptr;
	int i = 0;
	if (t->kind.dec == IdK) i++;
	for (; i < t->name.size(); i++) {
		bool flag = FindEntry(t->name[i], false, Entry);
		if (flag)
			cout << t->lineno << "������ʶ��" + t->name[i] + "�ظ�����" << endl;
		else {
			AttributeIR* a = new AttributeIR;
			a->kind = varKind;
			a->idtype = TypeProcess(t);
			a->More.VarAttr.access = dir;
			a->More.VarAttr.level = level;
			if (a->idtype != nullptr)
				a->More.VarAttr.off = off + a->idtype->size;
			Enter(t->name[i], a, Entry);
			// ���ű����TreeNode
			t->table.push_back(*Entry);
		}
	}
	VarDecList(t->sibling);
	return;
}
//�βη���������
ParamTable* ParaDecList(TreeNode* t) {
	if (t == nullptr)
		return nullptr;
	ParamTable* parat = nullptr;
	ParamTable* paralist = nullptr, * dt = nullptr;
	for (int i = 0; i < t->name.size(); i++) {
		symbtable** Entry = new symbtable*;
		*Entry = nullptr;
		bool flag = FindEntry(t->name[i], false, Entry);
		//t->table.push_back(*Entry);
		if (flag)
			cout << t->lineno << "�βα�ʶ��" + t->name[i] + "�ظ�����" << endl;
		else {
			AttributeIR* a = new AttributeIR;
			a->kind = varKind;
			a->idtype = TypeProcess(t);
			a->More.VarAttr.access = dir;
			a->More.VarAttr.level = level;
			a->More.VarAttr.off = off + a->idtype->size;

			Enter(t->name[i], a, Entry);
			t->table.push_back(*Entry);
			parat = new ParamTable;
			if (paralist == nullptr)
				paralist = parat;
			else dt->next = parat;
			parat->entry = (*Entry);
			//parat->next =new ParamTable;
			parat->next = nullptr;
			dt = parat;
		}
	}
	dt->next = ParaDecList(t->sibling);
	return paralist;
}
//�����������ַ���������
void ProcDecPart(TreeNode* t) {
	if (t == nullptr)
		return;
	//��������ӽ����ű�
	AttributeIR* procir = new AttributeIR;
	procir->kind = procKind;
	procir->More.ProcAttr.level = level;
	ParamTable* paramt = new ParamTable;
	procir->More.ProcAttr.param = paramt;

	symbtable** Entry = new symbtable*;
	*Entry = nullptr;
	Enter(t->name[0], procir, Entry);
	t->table.push_back(*Entry);

	CreateTable();
	//�����βΣ������βα�������ķ��ű��ڵ�param��
	(*Entry)->attrIR.More.ProcAttr.param = ParaDecList(t->child[0]);

	bianli(t->child[1]);                       //����                
	Body(t->child[2]->child[0]);               //���
	DestroyTable();
	return;
}

//���ʽ����������
typeIR* Expr(TreeNode* t, AccessKind* Ekind) {
	if (t->kind.exp == IdEK) {
		symbtable** Entry = new symbtable*;
		*Entry = nullptr;
		bool flag = FindEntry(t->name[0], true, Entry);
		t->table.push_back(*Entry);
		if (!flag)
			cout << t->lineno << "���ʽ�����" + t->name[0] + "δ����" << endl;
		else {
			if (t->child[0] != nullptr) {
				if ((*Entry)->attrIR.idtype->kind == arrayTy)                //�������ͷ���Ԫ�ص��ڲ���ʾ��kind
				{
					Expr(t->child[0], Ekind);
					if (Expr(t->child[0], Ekind) != nullptr) {
						if (Expr(t->child[0], Ekind)->kind == arrayTy)
							return Expr(t->child[0], Ekind);
					}
					return (*Entry)->attrIR.idtype->More.ArrayAttr.elemTy;
				}
				else if ((*Entry)->attrIR.idtype->kind == recordTy) {
					fieldChain* aa = (*Entry)->attrIR.idtype->More.body;
					while (aa != nullptr)
					{
						if (aa->idname == t->child[0]->name[0])
							return aa->unitType;
					}
					cout << t->lineno << "record��û�д�Ԫ��" << endl;
				}
			}
			else 
				return (*Entry)->attrIR.idtype;
		}
	}
	else if (t->kind.exp == ConstK) {
		(*Ekind) = dir;
		return intPtr;
	}
	else {
		AccessKind* Ekindt = new AccessKind;
		typeIR* a = Expr(t->child[0], Ekindt);
		typeIR* b = Expr(t->child[1], Ekindt);
		if (a == b) {
			(*Ekind) = dir;
			return a;
		}

		else cout << t->lineno << "���ʽ������Ͳ�ƥ��" << endl;
	}
	return nullptr;
}

//��ֵ����������
void assignstatement(TreeNode* t) {
	if (t->child[0]->kind.exp != IdEK)
		cout << t->lineno << "��ֵ�����˲��Ǳ�ʶ��" << endl;
	else {
		symbtable** Entry = new symbtable*;
		bool flag = FindEntry(t->child[0]->name[0], true, Entry);
		//t->table.push_back(*Entry);
		if (!flag)
			cout << t->lineno << "��ʶ��" + t->child[0]->name[0] + "δ����" << endl;
		else {
			TreeNode* nowt = t->child[0];
			symbtable* findt = *Entry;
			//typeIR* kindleft = findt->attrIR.idtype;
			AccessKind* Ekind0 = new AccessKind;
			typeIR* kindleft = Expr(t->child[0], Ekind0);
			if (nowt->child[0] != nullptr) {
				if (findt->attrIR.idtype->kind == arrayTy) {
					AccessKind* Ekind = new AccessKind;
					kindleft = Expr(nowt->child[0], Ekind);
				}
				else if ((*Entry)->attrIR.idtype->kind == recordTy) {
					fieldChain* aa = (*Entry)->attrIR.idtype->More.body;
					bool flagg = true;
					while (aa != nullptr)
					{
						if (aa->idname == nowt->child[0]->name[0]) {
							kindleft = aa->unitType;
							flagg = false;
							break;
						}
						aa = aa->next;
					}
					if (flagg) cout << t->lineno << "record��û�д�Ԫ��" << endl;
				}
			}
			AccessKind* Ekind = new AccessKind;
			typeIR* kindright = Expr(t->child[1], Ekind);
			if (kindleft != kindright)
				cout << t->lineno << "��ֵ����������Ͳ�ƥ��" << endl;
		}
	}
}

//���̵���������������
void callstatement(TreeNode* t) {
	symbtable** Entry = new symbtable*;
	*Entry = nullptr;
	bool flag = FindEntry(t->child[0]->name[0], true, Entry);
	t->child[0]->table.push_back(*Entry);
	if (!flag)
		cout << t->lineno << "����" + t->child[0]->name[0] + "δ����" << endl;
	else {
		if ((*Entry)->attrIR.kind != procKind)
			cout << t->lineno << "��ʶ�����Ǻ������ͣ��޷�����" << endl;
		else {
			if (t->child[1] == nullptr) {
				if ((*Entry)->attrIR.More.ProcAttr.param == nullptr)
					return;
				else cout << t->lineno << "������ƥ��" << endl;
			}
			ParamTable* a = (*Entry)->attrIR.More.ProcAttr.param;
			TreeNode* b = t->child[1];
			while (a != nullptr && b != nullptr) {
				AccessKind* Ekind = new AccessKind;
				typeIR* bt = Expr(b, Ekind);
				if (a->entry->attrIR.idtype != bt) {
					if (bt->kind == arrayTy && a->entry->attrIR.idtype->kind == arrayTy) {
						if (bt->size == a->entry->attrIR.idtype->size && bt->More.ArrayAttr.elemTy == a->entry->attrIR.idtype->More.ArrayAttr.elemTy)
							;
						else cout << t->lineno << "������ƥ��" << endl;
					}
					else cout << t->lineno << "������ƥ��" << endl;
				}

				b = b->sibling;
				a = a->next;
			}
			if (a != nullptr || b != nullptr) {
				cout << t->lineno << "������ƥ��" << endl;
			}

			return;
		}
	}
}

//����������������                                                                         
void ifstatement(TreeNode* t) {
	//if�﷨��child[0]Ϊif���ʽ,��Ϊ������
	TreeNode* ift = t->child[0];
	AccessKind* Ekind = new AccessKind;
	Expr(ift, Ekind);
	TreeNode* elset = t->child[1];
	while (elset != nullptr) {
		statement(elset);
		elset = elset->sibling;
	}TreeNode* thent = t->child[2];
	while (thent != nullptr) {
		statement(thent);
		thent = thent->sibling;
	}
	return;

}

//ѭ��������������
void whilestatement(TreeNode* t) {
	//while�﷨��child[0]Ϊif���ʽ,ͬif
	TreeNode* whilet = t->child[0];
	AccessKind* Ekind = new AccessKind;
	Expr(whilet, Ekind);
	TreeNode* dot = t->child[1];
	while (dot != nullptr) {
		statement(dot);
		dot = dot->sibling;
	}
}
//��������������
void readstatement(TreeNode* t) {
	symbtable** Entry = new symbtable*;
	*Entry = nullptr;
	//for (int i = 0; i < t->name.size(); i++) {}
	bool flag = FindEntry(t->name[0], true, Entry);
	t->table.push_back(*Entry);
	if (!flag)
		cout << t->lineno << "������ʶ��" + t->name[0] + "δ����" << endl;
	return;
}

//д������������
void writestatement(TreeNode* t) {
	AccessKind* Ekind = new AccessKind;
	Expr(t->child[0], Ekind);
	return;
}
//�����������������
void returnstatement(TreeNode* t) {
	return;
}
//������з���������
void statement(TreeNode* t) {
	if (t->kind.stmt == IfK) {
		ifstatement(t);
	}
	else if (t->kind.stmt == WhileK) {
		whilestatement(t);
	}
	else if (t->kind.stmt == AssignK) {
		assignstatement(t);
	}
	else if (t->kind.stmt == ReadK) {
		readstatement(t);
	}
	else if (t->kind.stmt == WriteK) {
		writestatement(t);
	}
	else if (t->kind.stmt == CallK) {
		callstatement(t);
	}
	else if (t->kind.stmt == ReturnK) {
		returnstatement(t);
	}
	return;
}
//ִ���岿�ַ�����������
void Body(TreeNode* t) {
	if (t == nullptr) return;
	while (t != nullptr) {
		statement(t);
		t = t->sibling;
	}
}

//��������Ĵ����������
typeIR* arrayVar(TreeNode* t) {

	return nullptr;
}
//��¼������������ķ���������
typeIR* recordVar(TreeNode* t) {
	return nullptr;
}

//�����﷨����������Ӧ�ĺ��������﷨�����
void bianli(TreeNode* t) {
	if (t == nullptr)
		return;
	if (t->nodekind == ProK) {
		bianli(t->child[0]);
		bianli(t->child[1]);
		bianli(t->child[2]);
		return;
	}if (t->nodekind == PheadK)
		return;
	if (t->nodekind == TypeK) {
		TypeDecPart(t->child[0]);
		bianli(t->sibling);
		return;
	}if (t->nodekind == VarK) {
		VarDecList(t->child[0]);
		bianli(t->sibling);
		return;
	}if (t->nodekind == ProcDecK) {
		ProcDecPart(t);
		return;
	}if (t->nodekind == StmLK) {
		Body(t->child[0]);
		return;
	}if (t->nodekind == StmtK) {
		Body(t->child[0]);
		return;
	}
}