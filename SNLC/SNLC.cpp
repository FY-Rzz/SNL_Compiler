// SNLC.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "SNLC.h"

// 设计条件编译标志，满足不同编译功能需求
int main()
{
    getTokenList("./Docs/snl2.txt", "./Docs/token.txt");
    TreeNode* root = DesParse();
    Analyze(root);
    outTree(root,0);
    std::cout << "Hello World!\n";
}

void outputTreeNode(TreeNode* root) {
    if (root == nullptr) {
        return;
    }

    // 输出当前节点的属性
    std::cout << "Line: " << root->lineno << std::endl;
    std::cout << "Node Kind: " << root->nodekind << std::endl;
    
    //std::cout << "Kind: " << root->kind << std::endl;
    std::cout << "Id Num: " << root->idnum << std::endl;

    std::cout << "Names: ";
    for (const auto& name : root->name) {
        std::cout << name << " ";
    }
    std::cout << std::endl;

    std::cout << "Tables: ";
    for (const auto& table : root->table) {
        std::cout << table << " ";
    }
    std::cout << std::endl;

    std::cout << "Type Name: " << root->type_name << std::endl;

    // 输出其他属性，根据需要进行适当的输出
    cout << "----------------------" << endl;

    // 递归遍历子节点
    for (int i = 0; i < 3; ++i) {
        outputTreeNode(root->child[i]);
    }

    // 遍历兄弟节点
    outputTreeNode(root->sibling);
}

void outTree(TreeNode* root, int level = 0) // level表示当前节点所在的层次，用于控制缩进
{
	if (root == NULL) return; // 如果节点为空，直接返回

	for (int i = 0; i < level; i++) // 根据层次输出缩进空格
	{
		cout << "  ";
	}

	cout << "("; // 输出左括号表示开始一个节点

	cout << "lineno: " << root->lineno << ", "; // 输出行号
	cout << "nodekind: " << root->nodekind << ", "; // 输出节点类型
	//cout << "kind: " << root->kind << ", "; // 输出具体类型
	cout << "idnum: " << root->idnum << ", "; // 输出标识符个数

	if (root->idnum > 0) // 如果有标识符，输出它们的名字
	{
		cout << "name: [";
		for (int i = 0; i < root->idnum; i++)
		{
			cout << root->name[i];
			if (i < root->idnum - 1) cout << ", ";
		}
		cout << "], ";
	}

	if (!root->type_name.empty()) // 如果有类型名，输出它
	{
		cout << "type_name: " << root->type_name << ", ";
	}

	//cout << "attr: " << root->attr; // 输出其他属性

	if (root->child[0] != NULL || root->child[1] != NULL || root->child[2] != NULL) // 如果有子节点，输出它们
	{
		cout << ", child: [";
		for (int i = 0; i < 3; i++)
		{
			outTree(root->child[i], level + 1); // 递归调用函数，层次加一
			if (i < 2 && root->child[i + 1] != NULL) cout << ", ";
		}
		cout << "]";
	}

	cout << ")"; // 输出右括号表示结束一个节点

	if (root->sibling != NULL) // 如果有兄弟节点，输出它们
	{
		cout << ", sibling: [";
		outTree(root->sibling, level); // 递归调用函数，层次不变
		cout << "]";
	}

	if (level == 0) cout << endl; // 如果是根节点，输出换行符

}

/// vs
/// 批量注释 Ctrl+K+C
/// 取消注释 Ctrl+K+U
///