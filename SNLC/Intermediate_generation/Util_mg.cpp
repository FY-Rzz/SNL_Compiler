#include "midcode_g.h"


ArgRecord* NewTemp(AccessKind access) {
	ArgRecord* tmp = new ArgRecord();
	tmp->form = AddrForm;
	tmp-> Addr.name = to_string(tmp_num); //???
	//cout << "temp" + tmp->Addr.name << endl;;
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
		switch (firstCode->onecode->codekind) {
			case 0:
				cout << "ADD";
				break;
			case 1:
				cout << "SUB";
				break;
			case 2:
				cout << "MULT";
				break;
			case 3:
				cout << "DIV";
				break;
			case 4:
				cout << "EQC";
				break;
			case 5:
				cout << "LTC";
				break;
			case 6:
				cout << "READC";
				break;
			case 7:
				cout << "WRITEC";
				break;
			case 8:
				cout << "RETURNC";
				break;
			case 9:
				cout << "ASSIG";
				break;
			case 10:
				cout << "AADD";
				break;
			case 11:
				cout << "LABEL";
				break;
			case 12:
				cout << "JUMP";
				break;
			case 13:
				cout << "JUMP0";
				break;
			case 14:
				cout << "CALL";
				break;
			case 15:
				cout << "VARACT";
				break;
			case 16:
				cout << "VALACT";
				break;
			case 17:
				cout << "PENTRY";
				break;
			case 18:
				cout << "ENDPROC";
				break;
			case 19:
				cout << "MENTRY";
				break;
			case 20:
				cout << "WHILESTART";
				break;
			case 21:
				cout << "ENDWHILE";
				break;
		}
		//cout << firstCode->onecode->codekind;
		cout << ", ";

		if (firstCode->onecode->arg1 != NULL) {
			switch (firstCode->onecode->arg1->form) {
				case ValueForm:
					cout << firstCode->onecode->arg1-> value;
					break;
				case LabelForm:
					cout << firstCode->onecode->arg1-> label;
					break;
				case AddrForm:
					if (firstCode->onecode->arg1->Addr.name[0] >= '0' && firstCode->onecode->arg1->Addr.name[0] <= '9') cout << "temp";
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
				if (firstCode->onecode->arg2->Addr.name[0] >= '0' && firstCode->onecode->arg2->Addr.name[0] <= '9') cout << "temp";
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
				if (firstCode->onecode->arg3->Addr.name[0] >= '0' && firstCode->onecode->arg3->Addr.name[0] <= '9') cout << "temp";
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
		tail->next = c;
		c->next = NULL;
		tail = c;
	}
	c->onecode = new MidCode();
	c->onecode->codekind = codekind;
	c->onecode->arg1 = Arg1;
	c->onecode->arg2 = Arg2;
	c->onecode->arg3 = Arg3;

	return c;
}



