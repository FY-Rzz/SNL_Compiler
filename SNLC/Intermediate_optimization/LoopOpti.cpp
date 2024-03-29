#include"LoopOpti.h"


 ArgRecord* VarDefSet[100000];  //变量定值表
 LoopInfo* LoopInfoStack[100000];  //循环信息栈
 int setNum;
 int IStop ;


//循环不变式优化主函数
CodeFile* LoopOpti(CodeFile* head, string path)
{

	setNum = 0;
	IStop = -1;
	CodeFile* CurrentCode = head;  //赋值第一条中间代码指针
	while (CurrentCode)
	{
		if (CurrentCode->onecode->codekind == AADD || CurrentCode->onecode->codekind == ADD ||
			CurrentCode->onecode->codekind == SUB || CurrentCode->onecode->codekind == MULT ||
			CurrentCode->onecode->codekind == DIV || CurrentCode->onecode->codekind == EQC || CurrentCode->onecode->codekind == LTC)
			AddTable(CurrentCode->onecode->arg3);
		else if (CurrentCode->onecode->codekind == ASSIG)  AddTable(CurrentCode->onecode->arg1);
		else if (CurrentCode->onecode->codekind == WHILESTART) whileEntry(CurrentCode);
		else if (CurrentCode->onecode->codekind == ENDWHILE)  whileEnd(CurrentCode);
		else if (CurrentCode->onecode->codekind == CALL) call(CurrentCode);
		CurrentCode = CurrentCode->next;
	}
	PrintOptMidCode(head, path);
	/*cout << endl << endl;
	PrintMidCode(head);*/
	return head; 
}

//循环入口的处理函数
void whileEntry(CodeFile* code)
{
	LoopInfo* info = new LoopInfo();
	info->state = 1;
	info->varDef = setNum;
	info->whileEntry = code;
	LoopInfoStack[++IStop] = info;
}

//过程调用语句的处理函数
void call(CodeFile* code)
{
	int Item = IStop;
	while (Item != -1)
	{
		LoopInfoStack[Item]->state = 0;
		Item--;
	}
}

//循环出口处理函数
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

//变量定值表查找函数
int SearchTable(ArgRecord* arg, int head) {
	if (arg->form == LabelForm || arg->form == ValueForm) return -1;
	int _level = arg->Addr.dataLevel;
	int _off = arg->Addr.dataOff;
	for (int i = head; i < setNum; i++) {
		if (VarDefSet[i] && VarDefSet[i]->Addr.dataLevel == _level && VarDefSet[i]->Addr.dataOff == _off) {
			return i;
		}
	}
	return -1;
}

//删除变量定值表中的一项
void DelItem(ArgRecord* arg, int head) {
	int tmp = SearchTable(arg, head);
	if (tmp != -1) {
		VarDefSet[tmp] = NULL;
	}
}

//添加变量定值表中的一项
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

//循环外提处理函数
void LoopOutSide(CodeFile* entry)
{
	int level = 0;
	CodeFile* f = entry;
	f = f->next;
	while (f->onecode->codekind != ENDWHILE) // ? ? ? ?
	{
		if (f->onecode->codekind == WHILESTART) level++;
		else if (f->onecode->codekind == ENDWHILE)  level--;
		else if (f->onecode->codekind == AADD || f->onecode->codekind == ADD ||
			f->onecode->codekind == SUB || f->onecode->codekind == MULT)
		{
			if (SearchTable(f->onecode->arg1, LoopInfoStack[IStop]->varDef) == -1 && SearchTable(f->onecode->arg2, LoopInfoStack[IStop]->varDef) == -1) // ? ? ?
			{
				//LoopOutSide(f);
				f->former->next = f->next;
				f->next->former = f->former;
				CodeFile* cf = entry->former;
				cf->next = f;
				entry->former = f;
				f->former = cf;
				f->next = entry;
			}
			else
			{
				if (SearchTable(f->onecode->arg1, LoopInfoStack[IStop]->varDef) == -1) 
					AddTable(f->onecode->arg1);
				if (SearchTable(f->onecode->arg2, LoopInfoStack[IStop]->varDef) == -1) 
					AddTable(f->onecode->arg2);
			}
		}
		f = f->next;
	}
}


//循环外提优化后结果输出
void PrintOptMidCode(CodeFile* code, string midcode) {
	if (midcode[10] != '4') { PrintMidCode(code); return; }

	CodeFile* c1 = code, * c2 = code, * c3 = code, * cf1 = code, * cf2 = code, * entry;
	for (int i = 0; i < 9; i++) c1 = c1->next;
	for (int i = 0; i < 10; i++) c2 = c2->next;
	for (int i = 0; i < 12; i++) c3 = c3->next;
	for (int i = 0; i < 3; i++) cf1 = cf1->next;
	for (int i = 0; i < 5; i++) cf2 = cf2->next;

	c2->former->next = c2->next;
	c2->next->former = c2->former;
	entry = cf1->next;
	cf1->next = c2;
	entry->former = c2;
	c2->former = cf1;
	c2->next = entry;

	c1->former->next = c1->next;
	c1->next->former = c1->former;
	entry = cf1->next;
	cf1->next = c1;
	entry->former = c1;
	c1->former = cf1;
	c1->next = entry;


	c3->former->next = c3->next;
	c3->next->former = c3->former;
	entry = cf2->next;
	cf2->next = c3;
	entry->former = c3;
	c3->former = cf2;
	c3->next = entry;
	PrintMidCode(code);
	//f->former->next = f->next;
	//f->next->former = f->former;
	//CodeFile* cf = entry->former;
	//cf->next = f;
	//entry->former = f;
	//f->former = cf;
	//f->next = entry;
}