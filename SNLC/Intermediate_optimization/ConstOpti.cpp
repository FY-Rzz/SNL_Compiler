#include "midcodeopt.h"

vector<CodeFile*> BaseBlock;



CodeFile* ConstOptimize() {
	// ���ֻ�����
	DivBaseBlock(head, BaseBlock);
    // ѭ������
    

}

void OptiBlock(int i)
{

}

// �м��������黮�ֺ���
vector<CodeFile*> DivBaseBlock(CodeFile* head, vector<CodeFile*> baseBlock) {
    // �ж�ͷ�ڵ��Ƿ�Ϊ��
    if (head == NULL) return;
    // ��ʼ����ǰ�������ͷ�ڵ��β�ڵ�
    CodeFile* blockHead = head;
    CodeFile* blockTail = head;
    // �����м��������
    while (blockTail != NULL) {
        // ��ȡ��ǰ���������
        CodeKind codekind = blockTail->onecode->codekind;
        // �ж��Ƿ���Ҫ������ǰ������
        if (codekind == JUMP0 || codekind == JUMP || codekind == RETURN || codekind == ENDPROC) {
            // ����ǰ��������뵽baseBlock��
            baseBlock.push_back(blockHead);
            // ����һ��������Ϊ�»������ͷ�ڵ�
            blockHead = blockTail->next;
        }
        // �ж��Ƿ���Ҫ��ʼ�»�����
        else if (codekind == LABEL || codekind == PENTRY || codekind == MENTRY) {
            // �����ǰ�����鲻Ϊ�գ�������뵽baseBlock��
            if (blockHead != blockTail) {
                baseBlock.push_back(blockHead);
            }
            // ����ǰ������Ϊ�»������ͷ�ڵ�
            blockHead = blockTail;
        }
        // �ƶ�����һ������
        blockTail = blockTail->next;
    }
    // ������һ�������鲻Ϊ�գ�������뵽baseBlock��
    if (blockHead != NULL) {
        baseBlock.push_back(blockHead);
    }
    return baseBlock;
}


