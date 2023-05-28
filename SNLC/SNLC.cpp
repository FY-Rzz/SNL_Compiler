// SNLC.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "SNLC.h"

// 设计条件编译标志，满足不同编译功能需求
int main()
{
    getTokenList("./Docs/snls.txt", "./Docs/token.txt");
    cout << "1111" << endl;
    TreeNode* root = DesParse();
    cout << "2222" << endl;
    //Analyze(root);
    std::cout << "Hello World!\n";

    system("PAUSE");
}

/// vs
/// 批量注释 Ctrl+K+C
/// 取消注释 Ctrl+K+U
///