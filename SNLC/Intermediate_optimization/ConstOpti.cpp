#include "midcodeopt.h"

vector<vector<CodeFile>> BaseBlock;
ConstDefT * Chead = nullptr, * Ctail = nullptr;



CodeFile* ConstOptimize() {
    

	// ���ֻ�����
    CodeFile* firstCode = new CodeFile;
    firstCode->next = nullptr;
	BaseBlock = DivBaseBlock(head);
    // ����ֿ�
    //for (int i = 0; i < BaseBlock.size(); i++) {
    //    for (int j = 0; j < BaseBlock[i].size(); j++) {
    //        PrintMidCode();
    //    }
    //}


    // ѭ������
    for (int i = 0; i < BaseBlock.size(); i++) {
        // ����ConsDef��
        Chead = new ConstDefT;
        Chead->former = nullptr;
        Chead->next = nullptr;
        Chead->variable = nullptr;
        Ctail = Chead;

        CodeFile* nextHead = OptiBlock(i);
        if (firstCode->next == nullptr) {
            firstCode = nextHead;
        }
        else {
            CodeFile* tmpCode = firstCode;
            while (tmpCode->next != nullptr) {
                tmpCode = tmpCode->next;
            }
            tmpCode->next = nextHead;
        }
        
    }

    return firstCode;
}

CodeFile* OptiBlock(int i)
{
    // ����������
    CodeFile* bhead = new CodeFile;
    bhead->former = nullptr;
    //bhead->onecode = BaseBlock[i][0].onecode;
    bhead->next = nullptr;
    CodeFile* bptr = bhead;
    for (int j = 0; j < BaseBlock[i].size(); j++) {
        CodeFile* btmp = new CodeFile;
        bptr->next = btmp;
        btmp->former = bptr;
        bptr = bptr->next;
        btmp->onecode = BaseBlock[i][j].onecode;
        btmp->next = nullptr;
    }
    //PrintMidCode(bhead->next);
    // ��������
    bptr = bhead->next;
    while (bptr != nullptr) {
        MidCode* mcode = bptr->onecode;
        if (mcode->codekind == ADD || mcode->codekind == SUB || mcode->codekind == MULT
            || mcode->codekind == DIV || mcode->codekind == LTC || mcode->codekind == EQC) {
            if (ArithC(bptr)) {
                // ɾ��
                bptr->former->next = bptr->next;
                bptr->next->former = bptr->former;
            }
        }
        else if (mcode->codekind == ASSIG) { // ��ֵ���
            SubstiArg(bptr, 2);
            if (bptr->onecode->arg2->form == ValueForm) {
                AppendTable(bptr->onecode->arg1, bptr->onecode->arg2->value);
            }
            else {
                DelConst(bptr->onecode->arg1);
            }
        }
        else if (mcode->codekind == JUMP0 || mcode->codekind == WRITEC) { //
            SubstiArg(bptr, 1);
        }
        else if (mcode->codekind == AADD) {
            SubstiArg(bptr, 2);
        }
        bptr = bptr->next;
    }

    // ���ش������CodeFile��
    return bhead->next;
}

bool ArithC(CodeFile* code)
{
    bool delCode = false;
    SubstiArg(code, 1);
    SubstiArg(code, 2);
    if ((code->onecode->arg1->form == ValueForm) && (code->onecode->arg2->form == ValueForm)) { // ������Ϊ����
        int tmp = code->onecode->arg1->value + code->onecode->arg2->value;
        AppendTable(code->onecode->arg3, tmp);
        delCode = true;
    }
    return delCode;
}

void SubstiArg(CodeFile* code, int i)
{
    MidCode* subcode = code->onecode;
    ArgRecord* subarg = nullptr;
    int tag = 0;
    if (i == 1) {
        subarg = subcode->arg1;
        tag = 1;
    }
    else if (i == 2) {
        subarg = subcode->arg2;
        tag = 2;
    }
    else if (i == 3) {
        subarg = subcode->arg3;
        tag = 3;
    }

    if (subarg->form == AddrForm) {
        ConstDefT** Entry = new ConstDefT*;
        (*Entry) = nullptr;
        if (FindConstT(subarg, Entry)) {
            ArgRecord* newA = new ArgRecord;
            newA->form = ValueForm;
            newA->value = (*Entry)->constValue;
            // �滻
            (*subarg) = (*newA);
        }
    }
}

// ����
bool FindConstT(ArgRecord* arg, ConstDefT** Entry)
{
    ConstDefT* cptr = Chead->next;
    while (Chead->next!=nullptr && cptr!=nullptr )
    {
        if ((cptr->variable->Addr.dataLevel == arg->Addr.dataLevel) && (cptr->variable->Addr.dataOff == arg->Addr.dataOff)) {
            (*Entry) = cptr;
            return true;
        }
        cptr = cptr->next;
    }
    return false;
}

// ���
void AppendTable(ArgRecord* arg, int result)
{
    ConstDefT** Entry = new ConstDefT*;
    (*Entry) = nullptr;
    if (FindConstT(arg, Entry)) { // �ڱ���
        (*Entry)->constValue = result;
    }
    else {
        // �½�����
        ConstDefT* newA = new ConstDefT;
        newA->constValue = result;
        newA->variable = arg;
        newA->next = nullptr;
        // ���ӱ��� 
        Ctail->next = newA;
        newA->former = Ctail;
        Ctail = newA;

    }
}

void DelConst(ArgRecord* arg)
{
    ConstDefT** Entry = new ConstDefT*;
    (*Entry) = nullptr;
    if (FindConstT(arg, Entry)) { // �ڱ���
        (*Entry)->former->next = (*Entry)->next;
        (*Entry)->next->former = (*Entry)->former;
    }
    return;
}

// �м��������黮�ֺ���
vector<vector<CodeFile>> DivBaseBlock(CodeFile* head) {
    vector<vector<CodeFile>> baseBlock;
    // �ж�ͷ�ڵ��Ƿ�Ϊ��
    if (head == NULL) 
        return baseBlock;
    // ��ʼ����ǰ�������ͷ�ڵ��β�ڵ�
    CodeFile* blockHead = head;
    CodeFile* blockTail = head;
    vector<CodeFile> tmpBlock;
    // �����м��������
    while (blockTail != NULL) {
        
        // ��ȡ��ǰ���������
        CodeKind codekind = blockTail->onecode->codekind;
        // �ж��Ƿ���Ҫ������ǰ������
        if (codekind == JUMP0 || codekind == JUMP || codekind == RETURN || codekind == ENDPROC || codekind == VARACT) {
            // ����ǰ��������뵽tmpBlock��
            do {
                tmpBlock.push_back(*blockHead);
                blockHead = blockHead->next;
            } while (blockHead != blockTail->next);
            baseBlock.push_back(tmpBlock);
            tmpBlock.clear();
            // ����һ��������Ϊ�»������ͷ�ڵ�
            blockHead = blockTail->next;
        }
        // �ж��Ƿ���Ҫ��ʼ�»�����
        else if (codekind == LABEL || codekind == PENTRY || codekind == MENTRY) {
            // �����ǰ�����鲻Ϊ�գ�������뵽baseBlock��
            if (blockHead != blockTail) {
                while (blockHead != blockTail) {
                    tmpBlock.push_back(*blockHead);
                    blockHead = blockHead->next;
                }
                baseBlock.push_back(tmpBlock);
                tmpBlock.clear();
            }
            // ����ǰ������Ϊ�»������ͷ�ڵ�
            blockHead = blockTail;
        }
        // �ƶ�����һ������
        blockTail = blockTail->next;
    }
    // ������һ�������鲻Ϊ�գ�������뵽baseBlock��
    while (blockHead != NULL) {
        tmpBlock.push_back(*blockHead);
        blockHead = blockHead->next;
    }
    baseBlock.push_back(tmpBlock);
    return baseBlock;
}


