// scanner.cpp : 词法分析主文件，源程序经过词法分析程序后生成Token序列

#include "lexical.h"

using namespace std;

const string word_symbol[] = { "id","if","begin","intc","end","+","-","*","/","=","<","[","]" ,"." ,
"type","var", "procedure","program",";","integer","char","array","record",".." ,
"of",",","(",")","endwh","while","return","read","write",":=","then","fi" ,"else","do" };// 用一个字符串数组存储所有的保留字和特殊符号，方便查找和输出
//id, if, begin, intc, end, plus, minus, times, over, eq, lt, lmidparen, rmidparen, dot
// type var procedure program semi integer char array record underange
// of comma lparen rparen endwh while return read write assign then fi else do



void Lexer(string inputFile, string outputFile) {
	ifstream fin(inputFile);  // 打开输入文件
	ofstream fout(outputFile);  // 打开输出文件
	string query;  // 存储每行读取的查询语句
	string imbuffer;  // 临时存储词素
	Token tok;  // 存储词法分析结果的结构体
	int linenum = 0;  // 记录当前行号
	int longquery;  // 查询语句的长度
	enum state { START, INID, INNUM, INCHAR, INCOMMENT, INASSIGN, INRANGE, DONE, FINISH };  // 枚举状态

	// 循环读取每行查询语句
	while (getline(fin, query)) {
		linenum++;  // 行号加1
		longquery = query.length();  // 查询语句的长度

		// 遍历查询语句的每个字符
		for (int i = 0; i < longquery; i++) {
			state sta = START;  // 初始化状态为START

			// 根据字符类型进行词法分析
			while (i < longquery && sta != FINISH) {
				if (query[i] == ' ' || query[i] == '\t') {
					i++;  // 忽略空格和制表符
				}
				else {
					if (isalpha(query[i])) {  // 判断是否为字母
						if (sta == START) {
							sta = INID;  // 进入字母状态
						}
						while (isalpha(query[i]) || isdigit(query[i])) {  // 继续读取字母或数字
							imbuffer = imbuffer + query[i];
							i++;
							if (i == longquery)
								break;
						}
						while (isalpha(query[i])) {  // 检查标识符之后是否跟着字母
							sta = START;  // 如果跟着字母，则状态回到START
							i++;
							if (i == longquery)
								break;
						}
						if (sta == INID) {
							tok.lineShow = linenum;  // 记录行号
							tok.lex = ID;  // 设置词法类型为标识符
							tok.sem = imbuffer;  // 设置词法值为识别出的标识符
							fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // 输出词法分析结果
							imbuffer.clear();  // 清空临时存储的词素
							sta = START;  // 状态回到START
							break;
						}
						if (sta == START) {
							string a = to_string(linenum) + "行有错误";
							InputError(a, LEX_ERROR);  // 输入错误处理
							exit(0);
							break;
						}
					}
					if (isdigit(query[i])) {  // 判断是否为数字
						if (sta == START) {
							sta = INNUM;  // 进入数字状态
						}
						while (isdigit(query[i]) || query[i] == '.') {  // 继续读取数字或小数点
							imbuffer = imbuffer + query[i];
							i++;
							if (i == longquery)
								break;
						}
						while (isdigit(query[i])) {  // 检查数字之后是否跟着数字
							sta = START;  // 如果跟着数字，则状态回到START
							i++;
							if (i == longquery)
								break;
						}
						if (sta == INNUM) {
							tok.lineShow = linenum;  // 记录行号
							tok.lex = INTC;  // 设置词法类型为数字
							tok.sem = imbuffer;  // 设置词法值为识别出的数字
							fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // 输出词法分析结果
							imbuffer.clear();  // 清空临时存储的词素
							sta = START;  // 状态回到START
							break;
						}
						if (sta == START) {
							string a = to_string(linenum) + "行有错误";
							InputError(a, LEX_ERROR);  // 输入错误处理
							exit(0);
							break;
						}
					}
					if (query[i] == '\'') {  // 判断是否为字符
						if (sta == START) {
							sta = INCHAR;  // 进入字符状态
							i++;
						}
						while (sta == INCHAR) {
							if (query[i] == '\'') {  // 遇到结束的单引号
								if (i < longquery - 1 && query[i + 1] == '\'') {  // 判断是否为转义字符
									imbuffer = imbuffer + query[i] + query[i + 1];
									i += 2;
								}
								else {  // 不是转义字符，结束字符状态
									imbuffer = imbuffer + query[i];
									i++;
									sta = START;
								}
							}
							else {
								imbuffer = imbuffer + query[i];
								i++;
							}
							if (i == longquery) {
								string a = to_string(linenum) + "行有错误";
								InputError(a, LEX_ERROR);  // 输入错误处理
								exit(0);
								break;
							}
						}
						if (sta == START) {
							tok.lineShow = linenum;  // 记录行号
							tok.lex = CHAR;  // 设置词法类型为字符
							tok.sem = imbuffer;  // 设置词法值为识别出的字符
							fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // 输出词法分析结果
							imbuffer.clear();  // 清空临时存储的词素
							break;
						}
					}
					if (query[i] == ':') {
						if (i < longquery - 1 && query[i + 1] == '=') {  // 判断是否为赋值运算符
							if (sta == START)
								sta = INASSIGN;  // 进入赋值运算符状态
							else {
								string a = to_string(linenum) + "行有错误";
								InputError(a, LEX_ERROR);  // 输入错误处理
								exit(0);
							}
							tok.lineShow = linenum;  // 记录行号
							tok.lex = ASSIGN;  // 设置词法类型为赋值运算符
							tok.sem = ":=";  // 设置词法值为赋值运算符
							fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // 输出词法分析结果
							sta = START;  // 状态回到START
							i++;
							break;
						}
					}
					if (query[i] == ',') {
						tok.lineShow = linenum;  // 记录行号
						tok.lex = COMMA;  // 设置词法类型为逗号
						tok.sem = ",";  // 设置词法值为逗号
						fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // 输出词法分析结果
						sta = START;  // 状态回到START
						break;
					}
					if (query[i] == ';') {
						tok.lineShow = linenum;  // 记录行号
						tok.lex = SEMI;  // 设置词法类型为分号
						tok.sem = ";";  // 设置词法值为分号
						fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // 输出词法分析结果
						sta = START;  // 状态回到START
						break;
					}
					if (query[i] == '(') {
						tok.lineShow = linenum;  // 记录行号
						tok.lex = LPAREN;  // 设置词法类型为左括号
						tok.sem = "(";  // 设置词法值为左括号
						fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // 输出词法分析结果
						sta = START;  // 状态回到START
						break;
					}
					if (query[i] == ')') {
						tok.lineShow = linenum;  // 记录行号
						tok.lex = RPAREN;  // 设置词法类型为右括号
						tok.sem = ")";  // 设置词法值为右括号
						fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // 输出词法分析结果
						sta = START;  // 状态回到START
						break;
					}
					if (query[i] == '[') {
						sta = INRANGE;  // 进入区间状态
						tok.lineShow = linenum;  // 记录行号
						tok.lex = LMIDPAREN;  // 设置词法类型为左中括号
						tok.sem = "[";  // 设置词法值为左中括号
						fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // 输出词法分析结果
						i++;
						break;
					}
					if (query[i] == ']') {
						sta = START;  // 状态回到START
						tok.lineShow = linenum;  // 记录行号
						tok.lex = RMIDPAREN;  // 设置词法类型为右中括号
						tok.sem = "]";  // 设置词法值为右中括号
						fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // 输出词法分析结果
						i++;
						break;
					}
					/*
					if (query[i] == '+' || query[i] == '-' || query[i] == '*' || query[i] == '/') {
						tok.lineShow = linenum;  // 记录行号
						tok.lex = OP;  // 设置词法类型为操作符
						tok.sem = query[i];  // 设置词法值为操作符
						fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // 输出词法分析结果
						sta = START;  // 状态回到START 
						i++;
						break;
					}*/
					if (query[i] == '<' || query[i] == '>' || query[i] == '=' || query[i] == '#' || query[i] == ':') {
						if (sta == START)
							sta = INCOMMENT;  // 进入注释状态
						else {
							string a = to_string(linenum) + "行有错误";
							InputError(a, LEX_ERROR);  // 输入错误处理
							exit(0);
						}
						i++;
						break;
					}
					else {
						string a = to_string(linenum) + "行有错误";
						InputError(a, LEX_ERROR);  // 输入错误处理
						exit(0);
						break;
					}
				}
			}

			// 处理注释
			if (sta == INCOMMENT) {
				while (i < longquery && query[i] != '\n') {  // 读取注释内容直到换行符
					i++;
				}
				sta = START;  // 状态回到START
			}
		}
	}

	fin.close();  // 关闭输入文件
	fout.close();  // 关闭输出文件
}


void getNextChar()
{
	
	
}

void ungetNextChar()
{
}

int reservedLookup(string s)
{
	// 保留字表的大小
	const int reserved_size = sizeof(reserved_word) / sizeof(reserved_word[0]);

	// 遍历保留字表，比较每个元素和s是否相等
	for (int i = 0; i < reserved_size; i++) {
		if (reserved_word[i] == s) {
			// 如果相等，返回下标i
			return i;
		}
	}
	// 如果没有找到，返回-1
	return -1;
}

void getTokenList(string inputFile, string outputFile)
{
	string query;
	int linenum = 0;
	state sta = START;
	int longquery;
	int i = 0;
	Token tok;
	ifstream fin(inputFile);
	ofstream fout(outputFile);
	if (!fin && !fout) {
		string a = "文件打开失败";
		InputError(a, LEX_ERROR);
		exit(0);
	}
	else
	{
		while ((getline(fin, query))) {
			linenum++;
			i = 0;
			string imbuffer;

			longquery = size(query);

			if (sta == FINISH) {
				break;
			}
			while (i < longquery) {
				if (sta == FINISH) {
					tok.lineShow = linenum;
					tok.lex = DOT;
					tok.sem = "Finish";
					fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;
					imbuffer.clear();
					break;
				}
				while ((sta == INCOMMENT || sta == START) && i < longquery) {
					if (query[i] == ' ') {
						while (query[i] == ' ') {
							i++;
						}
					}
					if (query[i] >= 'a' && query[i] <= 'z') {
						if (sta == START) {
							sta = INID;
						}
						while ((query[i] >= 'a' && query[i] <= 'z') || (query[i] >= '0' && query[i] <= '9')) {
							imbuffer = imbuffer + query[i];
							i++;
							if (i == longquery)
								break;
						}
						if (sta == INID) {
							tok.lineShow = linenum;
							tok.lex = ID;
							tok.sem = imbuffer;
							for (int a = 0; a <= 37; a++) {
								if (word_symbol[a] == imbuffer) {
									tok.lex = LexType(a);
									tok.sem = "ReseverWord";
									break;
								}
							}
							fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;
							imbuffer.clear();
							sta = START;
							break;
						}
					}
					if (query[i] >= '0' && query[i] <= '9') {
						if (sta == START) {
							sta = INNUM;
						}
						while (query[i] >= '0' && query[i] <= '9') {
							imbuffer = imbuffer + query[i];
							i++;
							if (i == longquery)
								break;
						}
						while (query[i] >= 'a' && query[i] <= 'z') {
							sta = START;
							i++;
							if (i == longquery)
								break;
						}
						if (sta == INNUM) {
							tok.lineShow = linenum;
							tok.lex = INTC;
							tok.sem = imbuffer;
							fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;
							imbuffer.clear();
							sta = START;
							break;
						}
						if (sta == START) {
							string a = to_string(linenum) + "行有错误";
							InputError(a, LEX_ERROR);
							exit(0);
							break;
						}
					}
					if (query[i] == '+' || query[i] == '-' || query[i] == '*' || query[i] == '/' || query[i] == '(' || query[i] == ')' || query[i] == ';' || query[i] == '[' || query[i] == ']' || query[i] == '=' || query[i] == '<') {
						if (sta == START)
							sta = DONE;
						if (sta == DONE) {
							tok.lineShow = linenum;
							imbuffer = query[i];
							for (int a = 0; a <= 37; a++) {
								if (word_symbol[a] == imbuffer) {
									tok.lex = LexType(a);
									tok.sem = "SingleSeparator";
									break;
								}
							}
							i++;
							fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;
							imbuffer.clear();
							sta = START;
							break;
						}
					}
					if (query[i] == ':') {
						if (i < longquery - 1 && query[i + 1] == '=') {
							if (sta == START)
								sta = INASSIGN;
						}

						if (sta == INASSIGN) {
							tok.lineShow = linenum;
							tok.lex = ASSIGN;
							tok.sem = "DobleSeparator";
							i = i + 2;

							fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;
							imbuffer.clear();
							sta = START;
							break;
						}

					}
					if (query[i] == ',') {
						if (sta == START)
							sta = INCHAR;
						if (sta == INCHAR) {
							tok.lineShow = linenum;
							tok.lex = COMMA;
							tok.sem = "SingleSeparator";
							i = i + 1;
							fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;
							imbuffer.clear();
							sta = START;
							while (query[i] == ' ') {
								i++;
							}
							if (i < longquery && ((query[i] >= 'a' && query[i] <= 'z') || (query[i] >= '0' && query[i] <= '9')));
							else {

								string a = to_string(linenum) + "行有错误";
								InputError(a, LEX_ERROR);
								exit(0);
							}

							break;
						}
					}
					if (query[i] == '.') {
						if (i < longquery - 1 && query[i + 1] == '.') {
							if (sta == START)
								sta = INRANGE;
						}
						else {
							if (sta == START && tok.lex == END)
								sta = FINISH;
							else
								if (sta == START) {
									i++;
									tok.lineShow = linenum;
									tok.lex = DOT;
									tok.sem = "Dot";
									fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;
									imbuffer.clear();
									break;
								}
						}
						if (sta == INRANGE) {
							tok.lineShow = linenum;
							tok.lex = UNDERANGE;
							tok.sem = "ArrayIndex";
							i = i + 2;
							fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;
							imbuffer.clear();
							sta = START;
							break;
						}
					}
					if (query[i] == '{') {
						if (sta == START)
							sta = INCOMMENT;
					}

					if (sta == INCOMMENT) {
						//cout << "注释开始" << endl;
						while (i < longquery && query[i] != '}')
							i++;
						if (query[i] == '}') {
							i++;
							sta = START;
							//cout << "注释结束" << endl;
							break;
						}
					}
					if (sta != INCOMMENT && sta != FINISH && i < longquery) {

						string a = to_string(linenum) + "行有错误";
						InputError(a, LEX_ERROR);
						exit(0);
						i++;
					}

				}
			}
		}
	}
	fin.close();
	fout.close();
}



