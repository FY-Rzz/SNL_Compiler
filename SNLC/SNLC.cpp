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
    outputTreeNode(root);
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

/// vs
/// 批量注释 Ctrl+K+C
/// 取消注释 Ctrl+K+U
///