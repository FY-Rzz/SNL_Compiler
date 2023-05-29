#pragma once
#include "../Intermediate_generation/midcode_g.h"

struct ConstDefT {
	ConstDefT* former;
	ArgRecord* variable;
	int constValue; // 变量当前定值
	ConstDefT* next;
};

extern vector<CodeFile*> BaseBlock;

// 划分基本快
vector<CodeFile*> DivBaseBlock(CodeFile* head, vector<CodeFile*> baseBlock);

// 常表达式优化主函数
CodeFile* ConstOptimize();

// 基本块内的常表达式优化函数
void OptiBlock(int i);

// 算术和比较运算的优化处理
bool ArithC(CodeFile* code);

// 值替换函数
void SubstiArg(CodeFile* code, int i);

// 变量定值表查找函数
bool FindConstT(ArgRecord* arg, ConstDefT** Entry);

// 变量定值表添加函数
void AppendTable(ArgRecord* arg, int result);

// 删除常量定值表中的一项
void DelConst(ArgRecord* arg);

