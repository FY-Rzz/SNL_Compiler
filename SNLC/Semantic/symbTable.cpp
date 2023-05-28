#include "semantic.h"

symbtable Scope[10000]; // 符号表
symbtable DestroyScope[10000];// 被撤销符号表
int destroylevel = 0;
int level = 0;
int off = 0, off2 = 0;
//void NewTable()
//{
//
//}

void CreateTable() {
	off2 = initOff;
	off = 0;
	level++;
}

//撤销一个符号表
void DestroyTable() {
	//DestroyScope[destroylevel].next = Scope[level].next;
	//destroylevel++;
	//Scope[level].next = nullptr;
	//off = off2;
	//level = level - 1;
	DestroyScope[destroylevel].next = Scope[level].next;
	destroylevel++;
	Scope[level].next = nullptr;
	off = off2;
	level = level - 1;
}

bool Enter(string Id, AttributeIR* AttribP, symbtable** Entry) {
	bool present = FindEntry(Id, false, Entry);
	if (present == true) {
		cout << "标识符重复声明" << endl;
	}
	else {                                                //插入符号表

		symbtable* newsymbtable = new symbtable;
		newsymbtable->idname = Id;
		newsymbtable->attrIR = *AttribP;          //标识符信息项
		/*cout << "入表" + Id ;
		cout <<"层数"<< AttribP->More.VarAttr.level<<" " << AttribP->More.ProcAttr.level<<endl;*/
		if (Scope[level].next == nullptr)
			Scope[level].next = newsymbtable;
		else {
			symbtable* t = Scope[level].next;
			while (t->next != nullptr)
			{
				t = t->next;
			}
			t->next = newsymbtable;
		}
		(*Entry) = newsymbtable;
	}
	return present;
}

bool FindEntry(string id, bool flag, symbtable** Entry) {
	if (flag) {
		for (int i = level; i > 0; i--)
			if (SearchoneTable(id, i, Entry))
				return true;
	}
	else return SearchoneTable(id, level, Entry);
	return false;
}

bool SearchoneTable(string id, int currentLevel, symbtable** Entry) {
	symbtable* t = Scope[currentLevel].next;
	while (t != nullptr) {                    //判断是否相同
		if (t->idname == id) {
			(*Entry) = t;
			return true;
		}
		else t = t->next;
	}
	return false;
}

bool FindField(string Id, fieldChain* head, fieldChain** Entry) {
	fieldChain* r = head;
	while (r != nullptr) {
		if (Id == r->idname)
		{
			(*Entry) = r;
			return true;
		}
		r = r->next;
	}
	return false;
}

void printSymbTable(string path)
{
	ofstream ouput;
	ouput.open(path);
	for (int i = 1; i < level + 1; i++) {
		symbtable* t = Scope[i].next;
		while (t != nullptr) {
			//有选择地输出一些符号表信息项
			if (t->attrIR.kind == typeKind)
				ouput << t->idname << " " << t->attrIR.kind << " " << &t->attrIR << " " << t->attrIR.idtype << endl;
			else if (t->attrIR.kind == varKind)
			{
				if (t->attrIR.idtype != nullptr)
					ouput << t->idname << " " << t->attrIR.kind << " " << &t->attrIR << " " << t->attrIR.idtype << " " << t->attrIR.More.VarAttr.level << " " << t->attrIR.idtype->kind << " " << t->attrIR.idtype->size << endl;
				else ouput << t->idname << " " << t->attrIR.kind << " " << &t->attrIR << " " << endl;
			}
			else ouput << t->idname << " " << t->attrIR.kind << " " << &t->attrIR << " " << t->attrIR.idtype << " " << t->attrIR.More.ProcAttr.level << " " << t->attrIR.More.ProcAttr.param << endl;

			t = t->next;
		}
	}
	ouput.close();
}