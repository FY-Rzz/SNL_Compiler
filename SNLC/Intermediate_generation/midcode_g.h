#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "../Semantic/semantic.h"

enum Form{ ValueForm, LabelForm, AddrForm };

enum CodeKind {
	// 算术运算
	ADD, SUB, MULT, DIV,
	// 关系运算
	EQC, LTC, 
	// 语句
	READC, WRITEC, RETURNC, ASSIG, AADD, LABEL, JUMP, JUMP0, CALL, VARACT, VALACT,
	// 定位
	PENTRY, ENDPROC, MENTRY
};

struct ArgRecord {
	Form form;
	union 
	{
		int value;
		int label;
		struct {
			string name;
			int dataLevel;
			int dataOff;
			AccessKind access; // dir or indir
		}Addr;
	} Attr;
};

// 中间代码结构
struct MidCode {
	CodeKind codekind;
	ArgRecord* arg1; // 操作分量1
	ArgRecord* arg2; // 操作分量2
	ArgRecord* arg3; // 结果
};

// 中间代码序列
struct CodeFile {
	CodeFile* former;
	MidCode* onecode;
	CodeFile* next;
};


//-------------------------------函数声明------------------------

/*---Util.cpp---*/
// 新建一个临时变量
ArgRecord* NewTemp(AccessKind access);

// 新建数值类 ARG 结构
ArgRecord* ARGValue(int value);

// 产生一个新的标号
int NewLabel();

// 新建标号类 ARG 结构
ArgRecord* ARGLabel(int label);

// 创建地址类 ARG 结构
ArgRecord* ARGAddr(char* id, int level, int off, AccessKind access);

// 输出中间代码
void PrintMidCode(CodeFile* firstCode);

// 生成中间代码
CodeFile* GenCode(CodeKind codekind, ArgRecord* Arg1, ArgRecord
	* Arg2, ArgRecord* Arg3);


/*---midcode.cpp---*/
// 中间代码生成主函数
CodeFile* GenMidCode(TreeNode* t);

// 过程声明中间代码生成函数
void GenProcDec(TreeNode* t);

// 语句体中间代码生成函数
void GenBody(TreeNode* t);

// 语句的中间代码生成函数
void GenStatement(TreeNode* t);

// 赋值语句中间代码生成函数
void GenAssignS(TreeNode* t);

// 变量中间代码生成函数
ArgRecord* GenVar(TreeNode* t);

//数组变量的中间代码生成函数
ArgRecord* GenArray(ArgRecord* Vlarg, TreeNode* t, int low, int size);

// 域成员变量的中间代码生成
ArgRecord* GenField(ArgRecord* V1arg, TreeNode* t, fieldChain* head);

// 表达式的中间代码生成函数
ArgRecord* GenExpr(TreeNode* t);

// 过程调用语句中间代码生成函数
void GenCallS(TreeNode* t);

// 读语句中间代码生成函数
void GenReadS(TreeNode* t);

// 条件语句中间代码生成函数
void GenIfS(TreeNode* t);

// 写语句中间代码生成函数
void GenWriteS(TreeNode* t);

// 循环语句中间代码生成函数
void GenWhileS(TreeNode* t);
