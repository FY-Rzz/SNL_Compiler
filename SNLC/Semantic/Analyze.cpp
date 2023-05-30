#include "semantic.h"

typeIR* intPtr, * charPtr, * boolPtr;
bool flagtree = false;

// 语义分析主函数
void Analyze(TreeNode* root)
{
	cout << "进入语义分析..." << endl;
	CreateTable();
	initialize();
	// 循环处理主程序声明
	// 循环处理主程序体
	bianli(root);
	// 打印符号表
	printSymbTable("./Docs/symbtable.txt");
	cout << "语义分析完成" << endl;
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

//自定义类型内部结构分析函数
typeIR* nameType(TreeNode* t) {
	symbtable** Entry = new symbtable*;
	*Entry = nullptr;
	int i = 0;
	if (flagtree)i++;
	bool flag = FindEntry(t->name[i], false, Entry);
	t->table.push_back(*Entry);
	if (!flag) 
		cout << t->lineno << "自定义类型" + t->name[i] + "未声明" << endl;
	else 
		return (*Entry)->attrIR.idtype;

	return nullptr;
}
//数组类型内部表示处理函数
typeIR* arrayType(TreeNode* t) {
	if (t->attr.arrayAttr.low > t->attr.arrayAttr.up) {
		cout << "数组下表越界" << endl;
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
//处理记录类型的内部表示函数
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
					cout << t->lineno << "record重命名" << endl;
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

//类型分析处理函数：类型分析处理。处理语法树的当前节点类型，构造当前类型的内部
//表示，并将其地址返回给Ptr类型内部表示的地址。
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

//类型声明部分分析处理函数
void TypeDecPart(TreeNode* t) {
	if (t == nullptr)
		return;

	//下面写处理过程
	symbtable** Entry = new symbtable*;
	*Entry = nullptr;
	int i = 0;
	if (t->kind.dec == IdK) i++;
	for (; i < t->name.size(); i++) {
		bool flag = FindEntry(t->name[0], false, Entry);
		if (flag)
			cout << t->lineno << "类型标识符" + t->name[0] + "重复声明" << endl;
		else {
			AttributeIR* a = new AttributeIR;
			a->kind = typeKind;
			flagtree = true;
			a->idtype = TypeProcess(t);
			flagtree = false;
			Enter(t->name[0], a, Entry);
			// 回填
			t->table.push_back(*Entry);
		}
	}TypeDecPart(t->sibling);
	return;
}
//变量声明部分分析处理函数
void VarDecList(TreeNode* t) {
	if (t == nullptr)
		return;
	//下面写处理过程
	symbtable** Entry = new symbtable*;
	*Entry = nullptr;
	int i = 0;
	if (t->kind.dec == IdK) i++;
	for (; i < t->name.size(); i++) {
		bool flag = FindEntry(t->name[i], false, Entry);
		if (flag)
			cout << t->lineno << "变量标识符" + t->name[i] + "重复声明" << endl;
		else {
			AttributeIR* a = new AttributeIR;
			a->kind = varKind;
			a->idtype = TypeProcess(t);
			a->More.VarAttr.access = dir;
			a->More.VarAttr.level = level;
			if (a->idtype != nullptr)
				a->More.VarAttr.off = off + a->idtype->size;
			Enter(t->name[i], a, Entry);
			// 符号表回填TreeNode
			t->table.push_back(*Entry);
		}
	}
	VarDecList(t->sibling);
	return;
}
//形参分析处理函数
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
			cout << t->lineno << "形参标识符" + t->name[i] + "重复声明" << endl;
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
//过程声明部分分析处理函数
void ProcDecPart(TreeNode* t) {
	if (t == nullptr)
		return;
	//将函数添加进符号表
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
	//处理形参，并将形参表给函数的符号表内的param项
	(*Entry)->attrIR.More.ProcAttr.param = ParaDecList(t->child[0]);

	bianli(t->child[1]);                       //声明                
	Body(t->child[2]->child[0]);               //语句
	DestroyTable();
	return;
}

//表达式分析处理函数
typeIR* Expr(TreeNode* t, AccessKind* Ekind) {
	if (t->kind.exp == IdEK) {
		symbtable** Entry = new symbtable*;
		*Entry = nullptr;
		bool flag = FindEntry(t->name[0], true, Entry);
		t->table.push_back(*Entry);
		if (!flag)
			cout << t->lineno << "表达式语句中" + t->name[0] + "未声明" << endl;
		else {
			if (t->child[0] != nullptr) {
				if ((*Entry)->attrIR.idtype->kind == arrayTy)                //数组类型返回元素的内部表示的kind
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
					cout << t->lineno << "record中没有此元素" << endl;
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

		else cout << t->lineno << "表达式语句类型不匹配" << endl;
	}
	return nullptr;
}

//赋值语句分析函数
void assignstatement(TreeNode* t) {
	if (t->child[0]->kind.exp != IdEK)
		cout << t->lineno << "赋值语句左端不是标识符" << endl;
	else {
		symbtable** Entry = new symbtable*;
		bool flag = FindEntry(t->child[0]->name[0], true, Entry);
		//t->table.push_back(*Entry);
		if (!flag)
			cout << t->lineno << "标识符" + t->child[0]->name[0] + "未声明" << endl;
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
					if (flagg) cout << t->lineno << "record中没有此元素" << endl;
				}
			}
			AccessKind* Ekind = new AccessKind;
			typeIR* kindright = Expr(t->child[1], Ekind);
			if (kindleft != kindright)
				cout << t->lineno << "赋值语句左右类型不匹配" << endl;
		}
	}
}

//过程调用语句分析处理函数
void callstatement(TreeNode* t) {
	symbtable** Entry = new symbtable*;
	*Entry = nullptr;
	bool flag = FindEntry(t->child[0]->name[0], true, Entry);
	t->child[0]->table.push_back(*Entry);
	if (!flag)
		cout << t->lineno << "函数" + t->child[0]->name[0] + "未声明" << endl;
	else {
		if ((*Entry)->attrIR.kind != procKind)
			cout << t->lineno << "标识符不是函数类型，无法调用" << endl;
		else {
			if (t->child[1] == nullptr) {
				if ((*Entry)->attrIR.More.ProcAttr.param == nullptr)
					return;
				else cout << t->lineno << "参数不匹配" << endl;
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
						else cout << t->lineno << "参数不匹配" << endl;
					}
					else cout << t->lineno << "参数不匹配" << endl;
				}

				b = b->sibling;
				a = a->next;
			}
			if (a != nullptr || b != nullptr) {
				cout << t->lineno << "参数不匹配" << endl;
			}

			return;
		}
	}
}

//条件语句分析处理函数                                                                         
void ifstatement(TreeNode* t) {
	//if语法树child[0]为if表达式,且为操作符
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

//循环语句分析处理函数
void whilestatement(TreeNode* t) {
	//while语法树child[0]为if表达式,同if
	TreeNode* whilet = t->child[0];
	AccessKind* Ekind = new AccessKind;
	Expr(whilet, Ekind);
	TreeNode* dot = t->child[1];
	while (dot != nullptr) {
		statement(dot);
		dot = dot->sibling;
	}
}
//读语句分析处理函数
void readstatement(TreeNode* t) {
	symbtable** Entry = new symbtable*;
	*Entry = nullptr;
	//for (int i = 0; i < t->name.size(); i++) {}
	bool flag = FindEntry(t->name[0], true, Entry);
	t->table.push_back(*Entry);
	if (!flag)
		cout << t->lineno << "读语句标识符" + t->name[0] + "未声明" << endl;
	return;
}

//写语句分析处理函数
void writestatement(TreeNode* t) {
	AccessKind* Ekind = new AccessKind;
	Expr(t->child[0], Ekind);
	return;
}
//返回语句分析处理程序
void returnstatement(TreeNode* t) {
	return;
}
//语句序列分析处理函数
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
//执行体部分分析处理函数：
void Body(TreeNode* t) {
	if (t == nullptr) return;
	while (t != nullptr) {
		statement(t);
		t = t->sibling;
	}
}

//数组变量的处理分析函数
typeIR* arrayVar(TreeNode* t) {

	return nullptr;
}
//记录变量中域变量的分析处理函数
typeIR* recordVar(TreeNode* t) {
	return nullptr;
}

//遍历语法树，调用相应的函数处理语法树结点
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