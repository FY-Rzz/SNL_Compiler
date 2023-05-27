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

// �½���ֵ�� ARG �ṹ  ????
ArgRecord* ARGValue(int value) { 
	ArgRecord* tmp = new ArgRecord();
	tmp->form = ValueForm;
	tmp->Attr.value = value;

	return tmp;
}

// ����һ���µı��
int NewLabel() {
	label_num++;
	return label_num;
}

// �½������ ARG �ṹ
ArgRecord* ARGLabel(int label) {
	ArgRecord* tmp = new ArgRecord();
	tmp->form = LabelForm;
	tmp->Attr.label = NewLabel();

	return tmp;
}

// ������ַ�� ARG �ṹ
ArgRecord* ARGAddr(string id, int level, int off, AccessKind access) { //�޸�string
	ArgRecord* tmp = new ArgRecord();
	tmp->form = AddrForm;
	tmp->Attr.Addr.name = id; //???
	tmp->Attr.Addr.dataLevel = level;
	tmp->Attr.Addr.dataOff = off;
	tmp->Attr.Addr.access = access;

	return tmp;
}

// ����м����
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

// �����м����
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



// �м��������������
CodeFile* GenMidCode(TreeNode* t) {
	TreeNode* tmp = t;
	tmp = t->child[1];
	while (tmp->nodekind != ProcDecK) {
		tmp = tmp->sibling;
	}
	if (tmp != NULL) tmp = tmp->child[0];
	while (tmp != NULL) {
		GenProcDec(tmp);
		tmp = tmp->sibling;

	}
	//������ڴ���
	ArgRecord* arg2 = ARGValue(0);
	ArgRecord* arg3 = ARGValue(0);
	CodeFile* c = GenCode(MENTRY, NULL, arg2, arg3);

	tmp_num = ;//���¼��һ����ʱ������ƫ��

	GenBody(t);
	//���¼�Ĵ�С����������arg2

	return head;
}

// ���������м�������ɺ���
void GenProcDec(TreeNode* t) {
	int ProcEny = NewLabel();
	//������ű�
	ArgRecord* arg1 = ARGValue(ProcEny);
	ArgRecord* arg2 = ARGValue(0);//����
	ArgRecord* arg3 = ARGValue(0);//ƫ����


	CodeFile* c = GenCode(PENTRY, arg1, arg2, arg3);

	//��ʼ��

	GenBody(t);
	//����  ͬ��

	CodeFile* c = GenCode(ENDPROC, NULL, NULL, NULL);
}
