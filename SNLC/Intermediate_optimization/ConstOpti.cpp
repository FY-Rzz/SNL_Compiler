#include "midcodeopt.h"

vector<vector<CodeFile>> BaseBlock;
ConstDefT * Chead = nullptr, * Ctail = nullptr;



CodeFile* ConstOptimize() {
    

	// 划分基本块
    CodeFile* firstCode = new CodeFile;
    firstCode->next = nullptr;
	BaseBlock = DivBaseBlock(head);
    // 输出分块
    //for (int i = 0; i < BaseBlock.size(); i++) {
    //    for (int j = 0; j < BaseBlock[i].size(); j++) {
    //        PrintMidCode();
    //    }
    //}


    // 循环处理
    for (int i = 0; i < BaseBlock.size(); i++) {
        // 重置ConsDef表
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
    // 构建基本块
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
    // 遍历处理
    bptr = bhead->next;
    while (bptr != nullptr) {
        MidCode* mcode = bptr->onecode;
        if (mcode->codekind == ADD || mcode->codekind == SUB || mcode->codekind == MULT
            || mcode->codekind == DIV || mcode->codekind == LTC || mcode->codekind == EQC) {
            if (ArithC(bptr)) {
                // 删除
                bptr->former->next = bptr->next;
                bptr->next->former = bptr->former;
            }
        }
        else if (mcode->codekind == ASSIG) { // 赋值语句
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

    // 返回处理过的CodeFile链
    return bhead->next;
}

bool ArithC(CodeFile* code)
{
    bool delCode = false;
    SubstiArg(code, 1);
    SubstiArg(code, 2);
    if ((code->onecode->arg1->form == ValueForm) && (code->onecode->arg2->form == ValueForm)) { // 两分量为常数
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
            // 替换
            (*subarg) = (*newA);
        }
    }
}

// 查找
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

// 添加
void AppendTable(ArgRecord* arg, int result)
{
    ConstDefT** Entry = new ConstDefT*;
    (*Entry) = nullptr;
    if (FindConstT(arg, Entry)) { // 在表中
        (*Entry)->constValue = result;
    }
    else {
        // 新建表项
        ConstDefT* newA = new ConstDefT;
        newA->constValue = result;
        newA->variable = arg;
        newA->next = nullptr;
        // 连接表项 
        Ctail->next = newA;
        newA->former = Ctail;
        Ctail = newA;

    }
}

void DelConst(ArgRecord* arg)
{
    ConstDefT** Entry = new ConstDefT*;
    (*Entry) = nullptr;
    if (FindConstT(arg, Entry)) { // 在表中
        (*Entry)->former->next = (*Entry)->next;
        (*Entry)->next->former = (*Entry)->former;
    }
    return;
}

// 中间代码基本块划分函数
vector<vector<CodeFile>> DivBaseBlock(CodeFile* head) {
    vector<vector<CodeFile>> baseBlock;
    // 判断头节点是否为空
    if (head == NULL) 
        return baseBlock;
    // 初始化当前基本块的头节点和尾节点
    CodeFile* blockHead = head;
    CodeFile* blockTail = head;
    vector<CodeFile> tmpBlock;
    // 遍历中间代码序列
    while (blockTail != NULL) {
        
        // 获取当前代码的类型
        CodeKind codekind = blockTail->onecode->codekind;
        // 判断是否需要结束当前基本块
        if (codekind == JUMP0 || codekind == JUMP || codekind == RETURN || codekind == ENDPROC || codekind == VARACT) {
            // 将当前基本块加入到tmpBlock中
            do {
                tmpBlock.push_back(*blockHead);
                blockHead = blockHead->next;
            } while (blockHead != blockTail->next);
            baseBlock.push_back(tmpBlock);
            tmpBlock.clear();
            // 将下一个代码作为新基本块的头节点
            blockHead = blockTail->next;
        }
        // 判断是否需要开始新基本块
        else if (codekind == LABEL || codekind == PENTRY || codekind == MENTRY) {
            // 如果当前基本块不为空，将其加入到baseBlock中
            if (blockHead != blockTail) {
                while (blockHead != blockTail) {
                    tmpBlock.push_back(*blockHead);
                    blockHead = blockHead->next;
                }
                baseBlock.push_back(tmpBlock);
                tmpBlock.clear();
            }
            // 将当前代码作为新基本块的头节点
            blockHead = blockTail;
        }
        // 移动到下一个代码
        blockTail = blockTail->next;
    }
    // 如果最后一个基本块不为空，将其加入到baseBlock中
    while (blockHead != NULL) {
        tmpBlock.push_back(*blockHead);
        blockHead = blockHead->next;
    }
    baseBlock.push_back(tmpBlock);
    return baseBlock;
}


