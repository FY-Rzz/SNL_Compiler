// scanner.cpp : �ʷ��������ļ���Դ���򾭹��ʷ��������������Token����

#include "lexical.h"

using namespace std;

const string word_symbol[] = { "id","if","begin","intc","end","+","-","*","/","=","<","[","]" ,"." ,
"type","var", "procedure","program",";","integer","char","array","record",".." ,
"of",",","(",")","endwh","while","return","read","write",":=","then","fi" ,"else","do" };// ��һ���ַ�������洢���еı����ֺ�������ţ�������Һ����
//id, if, begin, intc, end, plus, minus, times, over, eq, lt, lmidparen, rmidparen, dot
// type var procedure program semi integer char array record underange
// of comma lparen rparen endwh while return read write assign then fi else do



void Lexer(string inputFile, string outputFile) {
	ifstream fin(inputFile);  // �������ļ�
	ofstream fout(outputFile);  // ������ļ�
	string query;  // �洢ÿ�ж�ȡ�Ĳ�ѯ���
	string imbuffer;  // ��ʱ�洢����
	Token tok;  // �洢�ʷ���������Ľṹ��
	int linenum = 0;  // ��¼��ǰ�к�
	int longquery;  // ��ѯ���ĳ���
	enum state { START, INID, INNUM, INCHAR, INCOMMENT, INASSIGN, INRANGE, DONE, FINISH };  // ö��״̬

	// ѭ����ȡÿ�в�ѯ���
	while (getline(fin, query)) {
		linenum++;  // �кż�1
		longquery = query.length();  // ��ѯ���ĳ���

		// ������ѯ����ÿ���ַ�
		for (int i = 0; i < longquery; i++) {
			state sta = START;  // ��ʼ��״̬ΪSTART

			// �����ַ����ͽ��дʷ�����
			while (i < longquery && sta != FINISH) {
				if (query[i] == ' ' || query[i] == '\t') {
					i++;  // ���Կո���Ʊ��
				}
				else {
					if (isalpha(query[i])) {  // �ж��Ƿ�Ϊ��ĸ
						if (sta == START) {
							sta = INID;  // ������ĸ״̬
						}
						while (isalpha(query[i]) || isdigit(query[i])) {  // ������ȡ��ĸ������
							imbuffer = imbuffer + query[i];
							i++;
							if (i == longquery)
								break;
						}
						while (isalpha(query[i])) {  // ����ʶ��֮���Ƿ������ĸ
							sta = START;  // ���������ĸ����״̬�ص�START
							i++;
							if (i == longquery)
								break;
						}
						if (sta == INID) {
							tok.lineShow = linenum;  // ��¼�к�
							tok.lex = ID;  // ���ôʷ�����Ϊ��ʶ��
							tok.sem = imbuffer;  // ���ôʷ�ֵΪʶ����ı�ʶ��
							fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // ����ʷ��������
							imbuffer.clear();  // �����ʱ�洢�Ĵ���
							sta = START;  // ״̬�ص�START
							break;
						}
						if (sta == START) {
							string a = to_string(linenum) + "���д���";
							InputError(a, LEX_ERROR);  // ���������
							exit(0);
							break;
						}
					}
					if (isdigit(query[i])) {  // �ж��Ƿ�Ϊ����
						if (sta == START) {
							sta = INNUM;  // ��������״̬
						}
						while (isdigit(query[i]) || query[i] == '.') {  // ������ȡ���ֻ�С����
							imbuffer = imbuffer + query[i];
							i++;
							if (i == longquery)
								break;
						}
						while (isdigit(query[i])) {  // �������֮���Ƿ��������
							sta = START;  // ����������֣���״̬�ص�START
							i++;
							if (i == longquery)
								break;
						}
						if (sta == INNUM) {
							tok.lineShow = linenum;  // ��¼�к�
							tok.lex = INTC;  // ���ôʷ�����Ϊ����
							tok.sem = imbuffer;  // ���ôʷ�ֵΪʶ���������
							fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // ����ʷ��������
							imbuffer.clear();  // �����ʱ�洢�Ĵ���
							sta = START;  // ״̬�ص�START
							break;
						}
						if (sta == START) {
							string a = to_string(linenum) + "���д���";
							InputError(a, LEX_ERROR);  // ���������
							exit(0);
							break;
						}
					}
					if (query[i] == '\'') {  // �ж��Ƿ�Ϊ�ַ�
						if (sta == START) {
							sta = INCHAR;  // �����ַ�״̬
							i++;
						}
						while (sta == INCHAR) {
							if (query[i] == '\'') {  // ���������ĵ�����
								if (i < longquery - 1 && query[i + 1] == '\'') {  // �ж��Ƿ�Ϊת���ַ�
									imbuffer = imbuffer + query[i] + query[i + 1];
									i += 2;
								}
								else {  // ����ת���ַ��������ַ�״̬
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
								string a = to_string(linenum) + "���д���";
								InputError(a, LEX_ERROR);  // ���������
								exit(0);
								break;
							}
						}
						if (sta == START) {
							tok.lineShow = linenum;  // ��¼�к�
							tok.lex = CHAR;  // ���ôʷ�����Ϊ�ַ�
							tok.sem = imbuffer;  // ���ôʷ�ֵΪʶ������ַ�
							fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // ����ʷ��������
							imbuffer.clear();  // �����ʱ�洢�Ĵ���
							break;
						}
					}
					if (query[i] == ':') {
						if (i < longquery - 1 && query[i + 1] == '=') {  // �ж��Ƿ�Ϊ��ֵ�����
							if (sta == START)
								sta = INASSIGN;  // ���븳ֵ�����״̬
							else {
								string a = to_string(linenum) + "���д���";
								InputError(a, LEX_ERROR);  // ���������
								exit(0);
							}
							tok.lineShow = linenum;  // ��¼�к�
							tok.lex = ASSIGN;  // ���ôʷ�����Ϊ��ֵ�����
							tok.sem = ":=";  // ���ôʷ�ֵΪ��ֵ�����
							fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // ����ʷ��������
							sta = START;  // ״̬�ص�START
							i++;
							break;
						}
					}
					if (query[i] == ',') {
						tok.lineShow = linenum;  // ��¼�к�
						tok.lex = COMMA;  // ���ôʷ�����Ϊ����
						tok.sem = ",";  // ���ôʷ�ֵΪ����
						fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // ����ʷ��������
						sta = START;  // ״̬�ص�START
						break;
					}
					if (query[i] == ';') {
						tok.lineShow = linenum;  // ��¼�к�
						tok.lex = SEMI;  // ���ôʷ�����Ϊ�ֺ�
						tok.sem = ";";  // ���ôʷ�ֵΪ�ֺ�
						fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // ����ʷ��������
						sta = START;  // ״̬�ص�START
						break;
					}
					if (query[i] == '(') {
						tok.lineShow = linenum;  // ��¼�к�
						tok.lex = LPAREN;  // ���ôʷ�����Ϊ������
						tok.sem = "(";  // ���ôʷ�ֵΪ������
						fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // ����ʷ��������
						sta = START;  // ״̬�ص�START
						break;
					}
					if (query[i] == ')') {
						tok.lineShow = linenum;  // ��¼�к�
						tok.lex = RPAREN;  // ���ôʷ�����Ϊ������
						tok.sem = ")";  // ���ôʷ�ֵΪ������
						fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // ����ʷ��������
						sta = START;  // ״̬�ص�START
						break;
					}
					if (query[i] == '[') {
						sta = INRANGE;  // ��������״̬
						tok.lineShow = linenum;  // ��¼�к�
						tok.lex = LMIDPAREN;  // ���ôʷ�����Ϊ��������
						tok.sem = "[";  // ���ôʷ�ֵΪ��������
						fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // ����ʷ��������
						i++;
						break;
					}
					if (query[i] == ']') {
						sta = START;  // ״̬�ص�START
						tok.lineShow = linenum;  // ��¼�к�
						tok.lex = RMIDPAREN;  // ���ôʷ�����Ϊ��������
						tok.sem = "]";  // ���ôʷ�ֵΪ��������
						fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // ����ʷ��������
						i++;
						break;
					}
					/*
					if (query[i] == '+' || query[i] == '-' || query[i] == '*' || query[i] == '/') {
						tok.lineShow = linenum;  // ��¼�к�
						tok.lex = OP;  // ���ôʷ�����Ϊ������
						tok.sem = query[i];  // ���ôʷ�ֵΪ������
						fout << tok.lineShow << ' ' << tok.lex << ' ' << tok.sem << endl;  // ����ʷ��������
						sta = START;  // ״̬�ص�START 
						i++;
						break;
					}*/
					if (query[i] == '<' || query[i] == '>' || query[i] == '=' || query[i] == '#' || query[i] == ':') {
						if (sta == START)
							sta = INCOMMENT;  // ����ע��״̬
						else {
							string a = to_string(linenum) + "���д���";
							InputError(a, LEX_ERROR);  // ���������
							exit(0);
						}
						i++;
						break;
					}
					else {
						string a = to_string(linenum) + "���д���";
						InputError(a, LEX_ERROR);  // ���������
						exit(0);
						break;
					}
				}
			}

			// ����ע��
			if (sta == INCOMMENT) {
				while (i < longquery && query[i] != '\n') {  // ��ȡע������ֱ�����з�
					i++;
				}
				sta = START;  // ״̬�ص�START
			}
		}
	}

	fin.close();  // �ر������ļ�
	fout.close();  // �ر�����ļ�
}


void getNextChar()
{
	
	
}

void ungetNextChar()
{
}

int reservedLookup(string s)
{
	// �����ֱ�Ĵ�С
	const int reserved_size = sizeof(reserved_word) / sizeof(reserved_word[0]);

	// ���������ֱ��Ƚ�ÿ��Ԫ�غ�s�Ƿ����
	for (int i = 0; i < reserved_size; i++) {
		if (reserved_word[i] == s) {
			// �����ȣ������±�i
			return i;
		}
	}
	// ���û���ҵ�������-1
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
		string a = "�ļ���ʧ��";
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
							string a = to_string(linenum) + "���д���";
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

								string a = to_string(linenum) + "���д���";
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
						//cout << "ע�Ϳ�ʼ" << endl;
						while (i < longquery && query[i] != '}')
							i++;
						if (query[i] == '}') {
							i++;
							sta = START;
							//cout << "ע�ͽ���" << endl;
							break;
						}
					}
					if (sta != INCOMMENT && sta != FINISH && i < longquery) {

						string a = to_string(linenum) + "���д���";
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



