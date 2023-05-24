// Util_Lex.cpp : 语法分析工具程序

#include "lexical.h"
using namespace std;

void printTokenlist(vector<Token> tokenlist)
{
    for (const auto& token : tokenlist)
    {
        printToken(token);
    }
}

void printToken(Token token)
{
    cout << "Line: " << token.lineShow << " Lex: " << token.lex << " Sem: " << token.sem << endl;
}

void ChainToFile(vector<Token> tokenlist, string outputfile)
{
    ofstream outputFile(outputfile);
    if (outputFile.is_open())
    {
        for (const auto& token : tokenlist)
        {
            outputFile << "Line: " << token.lineShow << " Lex: " << token.lex << " Sem: " << token.sem << endl;
        }
        outputFile.close();
        cout << "Tokens written to file: " << outputfile << endl;
    }
    else
    {
        cout << "Unable to open output file: " << outputfile << endl;
    }
}

// 输出错误信息
void InputError(string info, const string file)
{
    ofstream ouput;
    ouput.open(file);
    ouput << info;
    ouput.close();
}