// SNLC.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "SNLC.h"

// 设计条件编译标志，满足不同编译功能需求
int main()
{
    //Lexer("snls.txt", "token.txt");
    getTokenList("snls.txt", "token.txt");

    std::cout << "Hello World!\n";
}
