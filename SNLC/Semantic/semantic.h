#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "../Grammar/grammar.h"

using namespace std;

//-------------------------------函数声明--------------------------------

/*---symbTable.cpp---*/

// 创建当前空符号表
void NewTable();

// 创建新的局部化单位的空符号表
void CreateTable();

// 撤销当前符号表
void DestroyTable();

// 登记标识符和属性
void Enter();

// 寻找表项地址
void FindEnter();

// 属性查询
void FindAttr();

// 判断类型是否相容
void Compat();

// 创建当前空类型内部表示
void NewTy();

// 创建当前空记录类型中..
void NewBody();

// 创建当前空形参链表
void NewParam();

// 错误提示
void ErrorPrompt();

// 打印空格
void printTab();

// 打印符号表
void printSymbTable();


// 打印符号表的一层
void printOneLayer();

/*---Analyze.cpp---*/

// 初始化基本类型
void initialize();

// 处理类型分析
void TypeProcess();

// 处理自定义类型名
void nameType();

// 处理数组类型的内部表示
void arrayType();

// 处理记录类型的内部表示
void recordType();

// 处理类型声明部分
void TypeDecPart();

// 处理变量声明部分
void VarDecList();

// 处理过程声明部分
void ProcDecPart();

// 处理过程头的语义分析
void HeadProcess();

// 处理参数声明的语义分析
void ParaDecList();

// 处理执行体部分的语义分析
void Body();

// 处理语句状态
void statement();

// 处理表达式
void Expr();

// 处理下标变量
void arrayVar();

// 处理域变量
void recordVar();

// 处理赋值语句
void Assignstatement();

// 处理调用语句
void callstatement();

// 处理条件语句
void ifstatement();

// 处理循环语句
void Whilestatement();

// 处理读语句
void readstatement();

// 处理写语句
void Writestatement();

// 处理返回语句
void Returnstatement();

// 语法分析主程序
void Analyze();