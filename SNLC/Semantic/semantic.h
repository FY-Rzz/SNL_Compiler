#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "../Grammar/grammar.h"

using namespace std;

//-------------------------------
// 标识符类别
enum IdKind { typeKind, varKind, procKind };// 类型、变量、过程

//
enum AccessKind { dir, indir }; // 直接变量、间接变量

// 类型枚举
enum TypeKind { intTy, charTy, arrayTy, recordTy, boolTy }; // 


//形参信息表声明
struct ParamTable;

//标识符信息项
typedef struct
{
	struct typeIR* idtype; /*指向标识符的类型内部表示*/
	IdKind kind; /*标识符的类型*/  //typeKind, varKind, procKind
	union
	{
		struct
		{
			//Access表示是直接变量还是间接变量：access = （dir,indir）indir表示是间接变量（变参属于间接变量），dir表示直接变量
			AccessKind access;
			int level;
			int off;
		}VarAttr; /*变量标识符的属性*/
		struct
		{
			int level;
			ParamTable* param; /*参数表*/ //形参信息表地址
			int code;
			int size;
		}ProcAttr; /*过程名标识符的属性*/
	}More; /*标识符的不同类型有不同的属性*/
}AttributeIR;

//符号表定义
struct symbtable
{
	string idname;
	AttributeIR attrIR;
	struct symbtable* next = nullptr;
};

struct ParamTable {
	symbtable* entry;                 //形参标识符在符号表中的位置
	ParamTable* next = nullptr;
};

//记录域表
struct fieldChain
{
	string idname; //记录域中的标识符；
	typeIR* unitType; //指向域类型的内部表示；                                                               //这里有错误
	int offset; //表示当前标识符在记录中的偏移；
	fieldChain* next; //指向下一个域。
};

//标识符内部表示
struct typeIR
{
	int size; /*类型所占空间大小*/
	TypeKind kind;  //intTy, charTy, arrayTy, recordTy, boolTy
	union
	{
		struct
		{
			struct typeIR* indexTy;
			struct typeIR* elemTy;
		} ArrayAttr;
		fieldChain* body; /*记录类型中的域链*/
	} More;
};

// 变量声明
const int initOff = 7;
extern int level;
extern int off;
//symbtable Scope[10000];//符号表
//symbtable DestroyScope[10000];//撤销符号表
//int destroylevel = 0;
//int level = 0;
//bool flagtree = false;//用于判断自定义类型是类型声明还是变量声明，变量声明为false
//symbtable* table = nullptr;

//-------------------------------函数声明--------------------------------

/*---symbTable.cpp---*/

// 创建当前空符号表
void NewTable();

// 创建新的局部化单位的空符号表
void CreateTable();

// 撤销当前符号表
void DestroyTable();

// 登记标识符和属性
bool Enter(string Id, AttributeIR* AttribP, symbtable** Entry);

// 寻找表项地址
bool FindEntry(string id, bool flag, symbtable** Entry);

// 从表头开始，依次将节点中的标识符名字和 id 比较是否相同
bool SearchoneTable(string id, int currentLevel, symbtable** Entry);

//在域表中查找域名
bool FindField(string Id, fieldChain* head, fieldChain** Entry);

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
void printSymbTable(string path);


// 打印符号表的一层
void printOneLayer();

/*---Analyze.cpp---*/

// 初始化基本类型
void initialize();

// 处理类型分析
typeIR* TypeProcess(TreeNode* t);

// 处理自定义类型名
typeIR* nameType(TreeNode* t);

// 处理数组类型的内部表示
typeIR* arrayType(TreeNode* t);

// 处理记录类型的内部表示
typeIR* recordType(TreeNode* t);

// 处理类型声明部分
void TypeDecPart(TreeNode* t);

// 处理变量声明部分
void VarDecList(TreeNode* t);

// 处理过程声明部分
void ProcDecPart(TreeNode* t);

// 处理过程头的语义分析
void HeadProcess();

// 处理参数声明的语义分析
ParamTable* ParaDecList(TreeNode* t);

// 处理执行体部分的语义分析
void Body(TreeNode* t);

// 处理语句状态
void statement(TreeNode* t);

// 处理表达式
typeIR* Expr(TreeNode* t, AccessKind* Ekind);

// 处理下标变量
typeIR* arrayVar(TreeNode* t);

// 处理域变量
typeIR* recordVar(TreeNode* t);

// 处理赋值语句
void assignstatement(TreeNode* t);

// 处理调用语句
void callstatement(TreeNode* t);

// 处理条件语句
void ifstatement(TreeNode* t);

// 处理循环语句
void whilestatement(TreeNode* t);

// 处理读语句
void readstatement(TreeNode* t);

// 处理写语句
void writestatement(TreeNode* t);

// 处理返回语句
void returnstatement(TreeNode* t);

// 语法分析主程序
void Analyze(TreeNode *currentP);

//遍历语法树，调用相应的函数处理语法树结点
void bianli(TreeNode* t);