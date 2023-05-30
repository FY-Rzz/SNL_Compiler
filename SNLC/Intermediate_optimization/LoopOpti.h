#pragma once

#include<iostream>
#include "../Intermediate_generation/midcode_g.h"


struct LoopInfo     //循环信息表
{
	int state;             //循环状态（0，1）
	CodeFile* whileEntry;  //指向循环入口中间代码
	int varDef;            //变量定值表的下标值，即本层的开始位置
	CodeFile* whileEnd;    //指向循环出口中间代码
};

extern ArgRecord* VarDefSet[100000];  //变量定值表
extern LoopInfo* LoopInfoStack[100000];  //循环信息栈
extern int setNum;
extern int IStop;


//循环不变式优化主函数
CodeFile* LoopOpti();

//循环入口的处理函数
void whileEntry(CodeFile* code);

//过程调用语句的处理函数
void call(CodeFile* code);

//循环出口处理函数
void whileEnd(CodeFile* code);

//变量定值表查找函数
int SearchTable(ArgRecord* arg, int head);

//删除变量定值表中的一项
void DelItem(ArgRecord* arg, int head);

//添加变量定值表中的一项
void AddTable(ArgRecord* arg);

//循环外提处理函数
void LoopOutSide(CodeFile* entry);

//循环外提优化后结果输出
void PrintOptMidCode(CodeFile* code);