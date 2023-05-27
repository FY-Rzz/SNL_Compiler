#include<midcode_g.h>

int tmp_num;
int label_num;
CodeFile* head, tail;
ArgRecord* NewTemp(AccessKind access) {
	ArgRecord* tmp = new ArgRecord();
	tmp->form = AddrForm;
	tmp->Attr.Addr.name = to_string(tmp_num); //???
	tmp->Attr.Addr.dataLevel = -1;
	tmp->Attr.Addr.dataOff = tmp_num;
	tmp->Attr.Addr.access = access;
	tmp_num++;

	return tmp;
}

// 新建数值类 ARG 结构  ????
ArgRecord* ARGValue(int value) { 
	ArgRecord* tmp = new ArgRecord();
	tmp->form = ValueForm;
	tmp->Attr.value = value;

	return tmp;
}

// 产生一个新的标号
int NewLabel() {
	label_num++;
	return label_num;
}

// 新建标号类 ARG 结构
ArgRecord* ARGLabel(int label) {
	ArgRecord* tmp = new ArgRecord();
	tmp->form = LabelForm;
	tmp->Attr.label = NewLabel();

	return tmp;
}

// 创建地址类 ARG 结构
ArgRecord* ARGAddr(string id, int level, int off, AccessKind access) { //修改string
	ArgRecord* tmp = new ArgRecord();
	tmp->form = AddrForm;
	tmp->Attr.Addr.name = id; //???
	tmp->Attr.Addr.dataLevel = level;
	tmp->Attr.Addr.dataOff = off;
	tmp->Attr.Addr.access = access;

	return tmp;
}

// 输出中间代码
void PrintMidCode(CodeFile* firstCode) {
	while (firstCode) {
		cout << "(";
		cout << firstCode->onecode->codekind;

		if (firstCode->onecode->arg1 != NULL) {
			switch (firstCode->onecode->arg1->form) {
				case ValueForm:
					cout << firstCode->onecode->arg1->Attr.value;
					break;
				case LabelForm:
					cout << firstCode->onecode->arg1->Attr.label;
					break;
				case AddrForm:
					cout << firstCode->onecode->arg1->Attr.Addr.name;
					break;
			}
		}
		else {
			cout << "_";
		}
		cout << ", ";

		if (firstCode->onecode->arg2 != NULL) {
			switch (firstCode->onecode->arg2->form) {
			case ValueForm:
				cout << firstCode->onecode->arg2->Attr.value;
				break;
			case LabelForm:
				cout << firstCode->onecode->arg2->Attr.label;
				break;
			case AddrForm:
				cout << firstCode->onecode->arg2->Attr.Addr.name;
				break;
			}
		}
		else {
			cout << "_";
		}
		cout << ", ";

		if (firstCode->onecode->arg3 != NULL) {
			switch (firstCode->onecode->arg3->form) {
			case ValueForm:
				cout << firstCode->onecode->arg3->Attr.value;
				break;
			case LabelForm:
				cout << firstCode->onecode->arg3->Attr.label;
				break;
			case AddrForm:
				cout << firstCode->onecode->arg3->Attr.Addr.name;
				break;
			}
		}
		else {
			cout << "_";
		}
		cout << ")" << endl;

		firstCode = firstCode->next;
	}
}

// 生成中间代码
CodeFile* GenCode(CodeKind codekind, ArgRecord* Arg1, ArgRecord
	* Arg2, ArgRecord* Arg3) {
	CodeFile* c = new GenCode();
	if (head == NULL) {
		head = tail = c;
		c->former = c->next = NULL;
	}
	else {
		c->former = tail;
		c->next = NULL;
		tail = c;
	}
	c->onecode->codekind = codekind;
	c->onecode->Arg1 = Arg1;
	c->onecode->Arg2 = Arg2;
	c->onecode->Arg3 = Arg3;

	return c;
}



