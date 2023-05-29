#include "midcodeopt.h"

vector<CodeFile*> BaseBlock;



CodeFile* ConstOptimize() {
	// 划分基本块
	DivBaseBlock(head, BaseBlock);
    // 循环处理
    

}

void OptiBlock(int i)
{

}

// 中间代码基本块划分函数
vector<CodeFile*> DivBaseBlock(CodeFile* head, vector<CodeFile*> baseBlock) {
    // 判断头节点是否为空
    if (head == NULL) return;
    // 初始化当前基本块的头节点和尾节点
    CodeFile* blockHead = head;
    CodeFile* blockTail = head;
    // 遍历中间代码序列
    while (blockTail != NULL) {
        // 获取当前代码的类型
        CodeKind codekind = blockTail->onecode->codekind;
        // 判断是否需要结束当前基本块
        if (codekind == JUMP0 || codekind == JUMP || codekind == RETURN || codekind == ENDPROC) {
            // 将当前基本块加入到baseBlock中
            baseBlock.push_back(blockHead);
            // 将下一个代码作为新基本块的头节点
            blockHead = blockTail->next;
        }
        // 判断是否需要开始新基本块
        else if (codekind == LABEL || codekind == PENTRY || codekind == MENTRY) {
            // 如果当前基本块不为空，将其加入到baseBlock中
            if (blockHead != blockTail) {
                baseBlock.push_back(blockHead);
            }
            // 将当前代码作为新基本块的头节点
            blockHead = blockTail;
        }
        // 移动到下一个代码
        blockTail = blockTail->next;
    }
    // 如果最后一个基本块不为空，将其加入到baseBlock中
    if (blockHead != NULL) {
        baseBlock.push_back(blockHead);
    }
    return baseBlock;
}


