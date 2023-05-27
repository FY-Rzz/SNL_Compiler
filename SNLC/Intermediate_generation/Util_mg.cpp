#include "midcode_g.h"

int tmp_num;
int label_num;
CodeFile* head, *tail;
ArgRecord* NewTemp(AccessKind access) {
	ArgRecord* tmp = new ArgRecord();
	tmp->form = AddrForm;
	tmp-> Addr.name = to_string(tmp_num); //???
	tmp-> Addr.dataLevel = -1;
	tmp-> Addr.dataOff = tmp_num;
	tmp-> Addr.access = access;
	tmp_num++;

	return tmp;
}

// 新建数值类 ARG 结构  ????
ArgRecord* ARGValue(int value) { 
	ArgRecord* tmp = new ArgRecord();
	tmp->form = ValueForm;
	tmp->value = value;

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
	tmp->label = label;

	return tmp;
}

// 创建地址类 ARG 结构
ArgRecord* ARGAddr(string id, int level, int off, AccessKind access) { //修改string
	ArgRecord* tmp = new ArgRecord();
	tmp->form = AddrForm;
	tmp-> Addr.name = id; //???
	tmp-> Addr.dataLevel = level;
	tmp-> Addr.dataOff = off;
	tmp-> Addr.access = access;

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
					cout << firstCode->onecode->arg1-> value;
					break;
				case LabelForm:
					cout << firstCode->onecode->arg1-> label;
					break;
				case AddrForm:
					cout << firstCode->onecode->arg1-> Addr.name;
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
				cout << firstCode->onecode->arg2-> value;
				break;
			case LabelForm:
				cout << firstCode->onecode->arg2-> label;
				break;
			case AddrForm:
				cout << firstCode->onecode->arg2-> Addr.name;
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
				cout << firstCode->onecode->arg3-> value;
				break;
			case LabelForm:
				cout << firstCode->onecode->arg3-> label;
				break;
			case AddrForm:
				cout << firstCode->onecode->arg3-> Addr.name;
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
	CodeFile* c = new CodeFile();
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
	c->onecode->arg1 = Arg1;
	c->onecode->arg2 = Arg2;
	c->onecode->arg3 = Arg3;

	return c;
}



