#include"LoopOpti.h"


 ArgRecord* VarDefSet[100000];  //������ֵ��
 LoopInfo* LoopInfoStack[100000];  //ѭ����Ϣջ
 int setNum;
 int IStop ;


//ѭ������ʽ�Ż�������
CodeFile* LoopOpti()
{
	setNum = 0;
	IStop = -1;
	CodeFile* CurrentCode = head;  //��ֵ��һ���м����ָ��
	while (CurrentCode)
	{
		if (CurrentCode->onecode->codekind == AADD || CurrentCode->onecode->codekind == ADD ||
			CurrentCode->onecode->codekind == SUB || CurrentCode->onecode->codekind == MULT ||
			CurrentCode->onecode->codekind == DIV || CurrentCode->onecode->codekind == EQC || CurrentCode->onecode->codekind == LTC)
			AddTable(CurrentCode->onecode->arg3);
		else if (CurrentCode->onecode->codekind == ASSIG)  AddTable(CurrentCode->onecode->arg2);
		else if (CurrentCode->onecode->codekind == WHILESTART) whileEntry(CurrentCode);
		else if (CurrentCode->onecode->codekind == ENDWHILE)  whileEnd(CurrentCode);
		else if (CurrentCode->onecode->codekind == CALL) call(CurrentCode);
		CurrentCode = CurrentCode->next;
	}
	cout << endl << endl;
	PrintMidCode(head);
	return head; 
}

//ѭ����ڵĴ�����
void whileEntry(CodeFile* code)
{
	LoopInfo* info = new LoopInfo();
	info->state = 1;
	info->varDef = setNum;
	info->whileEntry = code;
	LoopInfoStack[++IStop] = info;
}

//���̵������Ĵ�����
void call(CodeFile* code)
{
	int Item = IStop;
	while (Item != -1)
	{
		LoopInfoStack[Item]->state = 0;
		Item--;
	}
}

//ѭ�����ڴ�����
void whileEnd(CodeFile* code)
{
	LoopInfo* info = LoopInfoStack[IStop];
	if (info->state == 1)
	{

		info->whileEnd = code;
		
		CodeFile* entry = info->whileEntry;
		LoopOutSide(entry);
	}
	IStop--;
}

//������ֵ����Һ���
int SearchTable(ArgRecord* arg, int head) {
	int _level = arg->Addr.dataLevel;
	int _off = arg->Addr.dataOff;
	for (int i = head; i < setNum; i++) {
		if (VarDefSet[i] && VarDefSet[i]->Addr.dataLevel == _level && VarDefSet[i]->Addr.dataOff == _off) {
			return i;
		}
	}
	return -1;
}

//ɾ��������ֵ���е�һ��
void DelItem(ArgRecord* arg, int head) {
	int tmp = SearchTable(arg, head);
	if (tmp != -1) {
		VarDefSet[tmp] = NULL;
	}
}

//��ӱ�����ֵ���е�һ��
void AddTable(ArgRecord* arg) {
	int head=0;
	if (IStop != -1) {
		head = LoopInfoStack[IStop]->varDef;
	}
	int loc = SearchTable(arg, head);
	if (loc == -1) {
		VarDefSet[setNum++] = arg;
	}
}

//ѭ�����ᴦ����
void LoopOutSide(CodeFile* entry)
{
	int level = 0;
	CodeFile* f = entry;
	f = f->next;
	while (f->onecode->codekind != ENDWHILE && level != 0) // ? ? ? ?
	{
		if (f->onecode->codekind == WHILESTART) level++;
		else if (f->onecode->codekind == ENDWHILE)  level--;
		else if (f->onecode->codekind == AADD || f->onecode->codekind == ADD ||
			f->onecode->codekind == SUB || f->onecode->codekind == MULT)
		{
			if (SearchTable(f->onecode->arg1, LoopInfoStack[IStop]->varDef) == -1 && SearchTable(f->onecode->arg2, LoopInfoStack[IStop]->varDef) == -1) // ? ? ?
				LoopOutSide(f);
			else
			{
				if (SearchTable(f->onecode->arg1, LoopInfoStack[IStop]->varDef) == -1) AddTable(f->onecode->arg1);
				if (SearchTable(f->onecode->arg2, LoopInfoStack[IStop]->varDef) == -1) AddTable(f->onecode->arg2);
			}
		}
		f = f->next;
	}
}
