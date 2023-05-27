// SNLC.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "SNLC.h"

// 设计条件编译标志，满足不同编译功能需求
int main()
{
    getTokenList("./Docs/snls.txt", "./Docs/token.txt");
    TreeNode* root = DesParse();
    Analyze(root);
    std::cout << "Hello World!\n";
}

/// vs
/// 批量注释 Ctrl+K+C
/// 取消注释 Ctrl+K+U
///