#include "grammar.h"

/*******************变量声明************************/
string arrayLexTypeD_hyf[] = { "ID","IF","BEGIN","INTC","END","PLUS","MINUS","TIMES","OVER","EQ","LT","LMIDPAREN","RMIDPAREN" ,"DOT"
	,"TYPE","VAR", "PROCEDURE","PROGRAM","SEMI","INTEGER","CHAR","ARRAY","RECORD","UNDERANGE"
	,"OF","COMMA","LPAREN","RPAREN","ENDWH","WHILE","RETURN","READ","WRITE","ASSIGN","THEN","FI"
	,"ELSE","DO"
};
string temp_name;
Token token;		//存储当前Token
ifstream in("./Docs/token.txt");
/*******************函数声明************************/
TreeNode* DesParse() {
	//读入token
	ReadToken();
	TreeNode* t = Program();
	if (in.eof()) {
		Input(t, "./Docs/treeDes.txt");
	}
	else {
		string a = "文件提前结束";
		InputError(a, "./Docs/error.txt");
		exit(0);
	}
	return t;
}
TreeNode* Program() {
	TreeNode* root = new TreeNode();
	(*root).nodekind = ProK;
	(*root).lineno = token.lineShow ;
	TreeNode* t = ProgramHead();
	TreeNode* q = DeclarePart();
	TreeNode* s = ProgramBody();
	if (t == NULL) {
		string a = "没有程序头";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
	}
	else {
		(*root).child[0] = t;
	}
	if (q == NULL) {
		string a = "没有声明";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		//exit(0);
	}
	else {
		(*root).child[1] = q;
	}
	if (s == NULL) {
		string a = "没有程序体";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		//exit(0);
	}
	else {
		(*root).child[2] = s;
	}
	Match(DOT);
	return root;
}
TreeNode* ProgramHead() {
	TreeNode* t = new TreeNode();
	(*t).nodekind = PheadK;
	(*t).lineno = token.lineShow ;
	Match(PROGRAM);
	if (token.lex == ID) {
		(*t).idnum++;
		(*t).name.push_back(token.sem);
	}
	Match(ID);
	return t;
}
TreeNode* DeclarePart() {
	TreeNode* typeP = NULL, * varP = NULL;
	int line = token.lineShow ;
	TreeNode* tp1 = TypeDec();
	if (tp1 != NULL) {
		typeP = new TreeNode();
		(*typeP).nodekind = TypeK;
		(*typeP).child[0] = tp1;
		(*typeP).lineno = line;
	}
	line = token.lineShow ;
	TreeNode* tp2 = VarDec();
	if (tp2 != NULL) {
		varP = new TreeNode();
		(*varP).nodekind = VarK;
		(*varP).child[0] = tp2;
		(*varP).lineno = line;
	}
	TreeNode* s = ProcDec();
	if (varP == NULL) {
		if (typeP == NULL) {
			typeP = s;
		}
		else {
			(*typeP).sibling = s;
		}
	}
	else {
		(*varP).sibling = s;
		if (typeP == NULL) {
			typeP = varP;
		}
		else {
			(*typeP).sibling = varP;
		}
	}
	return typeP;
}
TreeNode* TypeDec() {
	TreeNode* t = NULL;
	if (token.lex == TYPE) {
		t = TypeDeclaration();
	}
	else if (token.lex == VAR || token.lex == PROCEDURE || token.lex == BEGIN) {}
	else {
		ReadToken();
		string a = to_string(token.lineShow ) + ":当前单词" + arrayLexTypeD_hyf[(int)token.lex] + "不合格，跳过";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
	}
	return t;
}
TreeNode* TypeDeclaration() {
	Match(TYPE);
	TreeNode* t = TypeDecList();
	if (t == NULL) {
		//显示提示信息
	}
	return t;
}
TreeNode* TypeDecList() {
	TreeNode* t = new TreeNode();
	(*t).nodekind = DecK;
	(*t).lineno = token.lineShow ;
	TypeId(t);
	Match(EQ);
	TypeDef(t);
	Match(SEMI);
	TreeNode* p = TypeDecMore();
	if (p != NULL)(*t).sibling = p;
	return t;
}
TreeNode* TypeDecMore() {
	TreeNode* t = NULL;
	if (token.lex == ID) {
		t = TypeDecList();
	}
	else if (token.lex == VAR || token.lex == PROCEDURE || token.lex == BEGIN) {}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
	}
	return t;
}
void TypeId(TreeNode* t) {
	if (token.lex == ID && t != NULL) {
		(*t).idnum++;
		(*t).name.push_back(token.sem);
	}
	Match(ID);
}
void TypeDef(TreeNode* t) {
	if (t != NULL) {
		if (token.lex == INTEGER || token.lex == CHAR) {
			BaseType(t);
		}
		else if (token.lex == ARRAY || token.lex == RECORD) {
			StructureType(t);
		}
		else if (token.lex == ID) {
			(*t).kind.dec = IdK;
			(*t).idnum++;
			(*t).name.push_back(token.sem);
			(*t).type_name = token.sem;
			Match(ID);
		}
		else {
			string a = to_string(token.lineShow ) + "行有错误";
			InputError(a, "./Docs/error.txt");
			//cout << "文件提前结束";
			exit(0);
		}
	}
}
void BaseType(TreeNode* t) {
	if (token.lex == INTEGER) {
		Match(INTEGER);
		(*t).kind.dec = IntegerK;
	}
	else if (token.lex == CHAR) {
		Match(CHAR);
		(*t).kind.dec = CharK;
	}
}
void StructureType(TreeNode* t) {
	if (token.lex == ARRAY) {
		ArrayType(t);
	}
	else if (token.lex == RECORD) {
		(*t).kind.dec = RecordK;
		RecType(t);
	}
}
void ArrayType(TreeNode* t) {
	Match(ARRAY);
	Match(LMIDPAREN);
	if (token.lex == INTC) {
		(*t).attr.arrayAttr.low = stoi(token.sem);
	}
	Match(INTC);
	Match(UNDERANGE);
	if (token.lex == INTC) {
		(*t).attr.arrayAttr.up = stoi(token.sem);
	}
	Match(INTC);
	Match(RMIDPAREN);
	Match(OF);
	BaseType(t);
	(*t).attr.arrayAttr.childType = (*t).kind.dec;
	(*t).kind.dec = ArrayK;
}
void RecType(TreeNode* t) {
	Match(RECORD);
	TreeNode* p = FieldDecList();
	if (p != NULL) {
		(*t).child[0] = p;
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//错误信息提示
	}
	Match(END);
}
TreeNode* FieldDecList() {
	TreeNode* t = new TreeNode();
	(*t).nodekind = DecK;
	(*t).lineno = token.lineShow ;
	TreeNode* p = NULL;
	if (token.lex == INTEGER || token.lex == CHAR) {
		BaseType(t);
		IdList(t);
		Match(SEMI);
		p = FieldDecMore();
	}
	else if (token.lex == ARRAY) {
		ArrayType(t);
		IdList(t);
		Match(SEMI);
		p = FieldDecMore();
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//读入下一个单词，但是现在都没有匹配的了，读入下一个单词不就是有错误吗
	}
	(*t).sibling = p;
	return t;
}
TreeNode* FieldDecMore() {
	TreeNode* t = NULL;
	if (token.lex != END) {
		if (token.lex == INTEGER || token.lex == CHAR || token.lex == ARRAY) {
			t = FieldDecList();
		}
		else {
			string a = to_string(token.lineShow ) + "行有错误";
			InputError(a, "./Docs/error.txt");
			//cout << "文件提前结束";
			exit(0);
			//读入下一个单词
		}
	}
	return t;
}
void IdList(TreeNode* t) {
	if (token.lex == ID) {
		(*t).idnum++;
		(*t).name.push_back(token.sem);
	}
	Match(ID);
	IdMore(t);
}
void IdMore(TreeNode* t) {
	if (token.lex == SEMI) {}
	else if (token.lex == COMMA) {
		Match(COMMA);
		IdList(t);
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//读入下一个单词
	}
}
TreeNode* VarDec() {
	TreeNode* t = NULL;
	if (token.lex == PROCEDURE || token.lex == BEGIN) {}
	else if (token.lex == VAR) {
		t = VarDeclaration();
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "error.txt");
		//cout << "文件提前结束";
		exit(0);
		//读入下一个单词
	}
	return t;
}
TreeNode* VarDeclaration() {
	Match(VAR);
	TreeNode* t = VarDecList();
	if (t == NULL) {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//语法错误信息提示
	}
	return t;
}
TreeNode* VarDecList() {
	TreeNode* t = new TreeNode();
	(*t).nodekind = DecK;
	(*t).lineno = token.lineShow ;
	TreeNode* p = NULL;
	TypeDef(t);
	VarIdList(t);
	Match(SEMI);
	p = VarDecMore();
	(*t).sibling = p;
	return t;
}
TreeNode* VarDecMore() {
	TreeNode* t = NULL;
	if (token.lex == PROCEDURE || token.lex == BEGIN) {}
	else if (token.lex == INTEGER || token.lex == CHAR ||
		token.lex == ARRAY || token.lex == RECORD || token.lex == ID) {
		t = VarDecList();
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//读入下一个单词
	}
	return t;
}
void VarIdList(TreeNode* t) {
	if (token.lex == ID) {
		(*t).idnum++;
		(*t).name.push_back(token.sem);
		Match(ID);
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//错误提示，读入下一个单词
	}
	VarIdMore(t);
}
void VarIdMore(TreeNode* t) {
	if (token.lex == SEMI) {}
	else if (token.lex == COMMA) {
		Match(COMMA);
		VarIdList(t);
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//读入下一个单词
	}
}
TreeNode* ProcDec() {
	TreeNode* t = NULL;
	if (token.lex == BEGIN) {}
	else if (token.lex == PROCEDURE) {
		t = ProcDeclaration();
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//读入下一个单词
	}
	return t;
}
TreeNode* ProcDeclaration() {
	TreeNode* t = new TreeNode();
	(*t).nodekind = ProcDecK;
	(*t).lineno = token.lineShow ;
	Match(PROCEDURE);
	if (token.lex == ID) {
		(*t).idnum++;
		(*t).name.push_back(token.sem);
		Match(ID);
	}
	Match(LPAREN);
	ParamList(t);
	Match(RPAREN);
	Match(SEMI);
	(*t).child[1] = ProcDecPart();
	(*t).child[2] = ProcBody();
	(*t).sibling = ProcDecMore();
	return t;
}
TreeNode* ProcDecMore() {
	TreeNode* t = NULL;
	if (token.lex == PROCEDURE || token.lex == BEGIN) {}
	else if (token.lex == INTEGER || token.lex == CHAR ||
		token.lex == ARRAY || token.lex == RECORD || token.lex == ID) {
		t = ProcDeclaration();
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//读入下一个单词
	}
	return t;
}
void ParamList(TreeNode* t) {
	TreeNode* p = NULL;
	if (token.lex == RPAREN) {}
	else if (token.lex == INTEGER || token.lex == CHAR || token.lex == ARRAY
		|| token.lex == RECORD || token.lex == ID || token.lex == VAR) {
		p = ParamDecList();
		(*t).child[0] = p;
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//读入下一个单词
	}
}
TreeNode* ParamDecList() {
	TreeNode* t = Param();
	TreeNode* p = ParamMore();
	if (p != NULL) {
		(*t).sibling = p;
	}
	return t;
}
TreeNode* ParamMore() {
	TreeNode* t = NULL;
	if (token.lex == RPAREN) {}
	else if (token.lex == SEMI) {
		Match(SEMI);
		t = ParamDecList();
		if (t == NULL) {
			//错误提示
			string a = to_string(token.lineShow ) + "行有错误";
			InputError(a, "./Docs/error.txt");
			//cout << "文件提前结束";
			exit(0);
		}
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//读入下一个单词
	}
	return t;
}
TreeNode* Param() {
	TreeNode* t = new TreeNode();
	(*t).nodekind = DecK;
	(*t).lineno = token.lineShow ;
	if (token.lex == INTEGER || token.lex == CHAR || token.lex == RECORD
		|| token.lex == ARRAY || token.lex == ID) {
		(*t).attr.procAttr.paramt = Valparamtype;
		TypeDef(t);
		FormList(t);
	}
	else if (token.lex == VAR) {
		Match(VAR);
		(*t).attr.procAttr.paramt = Varparamtype;
		TypeDef(t);
		FormList(t);
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//读入下一个单词
	}
	return t;
}
void FormList(TreeNode* t) {
	if (token.lex == ID) {
		(*t).idnum++;
		(*t).name.push_back(token.sem);
		Match(ID);
	}
	FidMore(t);
}
void FidMore(TreeNode* t) {
	if (token.lex == SEMI || token.lex == RPAREN) {}
	else if (token.lex == COMMA) {
		Match(COMMA);
		FormList(t);
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//读取下一个单词
	}
}
TreeNode* ProcDecPart() {
	return DeclarePart();
}
TreeNode* ProcBody() {
	TreeNode* t = ProgramBody();
	if (t == NULL) {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//错误信息提示
	}
	return t;
}
TreeNode* ProgramBody() {  //创建语句序列标志节点StmLK,匹配BEGIN，处理语句函数，匹配END
	TreeNode* t = new TreeNode();
	(*t).nodekind = StmLK;
	(*t).lineno = token.lineShow ;
	Match(BEGIN);
	(*t).child[0] = StmList();
	Match(END);
	return t;
}
TreeNode* StmList() { //处理语句函数，循环
	TreeNode* t = Stm();
	TreeNode* p = StmMore();
	(*t).sibling = p;
	return t;
}
TreeNode* StmMore() { //执行完stm（）后紧接着调用，如果是逗号则继续执行StmList，
	TreeNode* t = NULL;
	if (token.lex == END || token.lex == ENDWH || token.lex == ELSE || token.lex == FI) {}
	else if (token.lex = SEMI) {
		Match(SEMI);
		t = StmList();
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//读入下一个单词
	}
	return t;
}
TreeNode* Stm() { //根据语句的lex不同，判断调用哪个语句处理函数
	TreeNode* t = NULL;
	if (token.lex == IF) {
		t = ConditionalStm();
	}
	else if (token.lex == WHILE) {
		t = LoopStm();
	}
	else if (token.lex == RETURN) {
		t = ReturnStm();
	}
	else if (token.lex == READ) {
		t = InputStm();
	}
	else if (token.lex == WRITE) {
		t = OutputStm();
	}
	else if (token.lex == ID) {  //如果语句以标识符开始，先创建语句节点，创建它的0号儿子节点（表达式语句-》标识符类型），它的1号儿子节点由asscall函数决定
		TreeNode* f = new TreeNode();
		(*f).nodekind = StmtK;
		(*f).lineno = token.lineShow ;
		TreeNode* t1 = new TreeNode();
		t1->nodekind = ExpK;
		t1->lineno = token.lineShow ;
		t1->kind.exp = IdEK;
		t1->attr.expAttr.varkind = IdV;
		t1->idnum++;
		t1->name.push_back(token.sem);
		f->child[0] = t1;
		temp_name = token.sem;
		AssCall(f);
		t = f;
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//当前单词为其他单词，非期望单词语法错误，
		//跳过当前单词，读入下一个单词
	}
	return t;
}
void AssCall(TreeNode* t) { //语句以标识符开始，大致分成2种情况：非函数和函数；
	Match(ID);
	if (token.lex == ASSIGN || token.lex == LMIDPAREN || token.lex == DOT) { //非函数，f的节点赋予赋值语句类型
		AssignmentRest(t);
		t->kind.stmt = AssignK;

	}
	else if (token.lex == LPAREN) {
		CallStmRest(t);
		t->kind.stmt = CallK;
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//读入下一个单词 
	}
}
void AssignmentRest(TreeNode* t) {  //处理赋值语句后续
	VariMore(t->child[0]);
	Match(ASSIGN);
	t->child[1] = Exp();
}
TreeNode* ConditionalStm() { //创建语句节点，类型为判断语句，匹配IF，处理之后表达式，匹配THEN，stmList处理之后一系列语句，判断是否有ELSE，有则继续处理，没有匹配FI
	TreeNode* t = new TreeNode();
	(*t).nodekind = StmtK;
	(*t).kind.stmt = IfK;
	(*t).lineno = token.lineShow ;
	Match(IF);
	(*t).child[0] = Exp();
	Match(THEN);
	(*t).child[1] = StmList();
	if (token.lex == ELSE) {
		Match(ELSE);
		(*t).child[2] = StmList();
	}
	Match(FI);
	return t;
}
TreeNode* LoopStm() {////创建语句节点，类型为循环语句，
	TreeNode* t = new TreeNode();
	(*t).nodekind = StmtK;
	(*t).kind.stmt = WhileK;
	(*t).lineno = token.lineShow ;
	Match(WHILE);
	(*t).child[0] = Exp();
	Match(DO);
	(*t).child[1] = StmList();
	Match(ENDWH);
	return t;
}
TreeNode* InputStm() {//创建语句节点，类型为循环语句，
	TreeNode* t = new TreeNode();
	(*t).nodekind = StmtK;
	(*t).kind.stmt = ReadK;
	(*t).lineno = token.lineShow ;
	Match(READ);
	Match(LPAREN);
	if (token.lex == ID) {
		(*t).idnum++;
		(*t).name.push_back(token.sem);
	}
	Match(ID);
	Match(RPAREN);
	return t;
}
TreeNode* OutputStm() {//创建语句节点，类型为输出语句，
	TreeNode* t = new TreeNode();
	(*t).nodekind = StmtK;
	(*t).kind.stmt = WriteK;
	(*t).lineno = token.lineShow ;
	Match(WRITE);
	Match(LPAREN);
	(*t).child[0] = Exp();
	Match(RPAREN);
	return t;
}
TreeNode* ReturnStm() {//创建语句节点，类型为返回语句，
	TreeNode* t = new TreeNode();
	(*t).nodekind = StmtK;
	(*t).kind.stmt = ReturnK;
	(*t).lineno = token.lineShow ;
	Match(RETURN);
	return t;
}
void CallStmRest(TreeNode* t) {
	Match(LPAREN);
	(*t).child[1] = ActParamList();
	Match(RPAREN);
}
TreeNode* ActParamList() {
	TreeNode* t = NULL;
	if (token.lex == RPAREN) {}
	else if (token.lex == ID || token.lex == INTC) {
		t = Exp();
		if (t != NULL) {
			(*t).sibling = ActParamMore();
		}
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//读入下一个单词
	}
	return t;
}
TreeNode* ActParamMore() {
	TreeNode* t = NULL;
	if (token.lex == RPAREN) {}
	else if (token.lex == COMMA) {
		Match(COMMA);
		t = ActParamList();
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//读入下一个单词
	}
	return t;
}
TreeNode* Exp() {
	int line = token.lineShow ;
	TreeNode* t = Simple_exp();
	if (token.lex == LT || token.lex == EQ) {
		TreeNode* p = new TreeNode();
		(*p).lineno = line;
		(*p).nodekind = ExpK;
		(*p).kind.exp = OpK;
		(*p).child[0] = t;
		(*p).attr.expAttr.op = token.lex;
		t = p;
		Match(token.lex);
		if (t != NULL) {
			(*t).child[1] = Simple_exp();
		}
	}
	return t;
}
TreeNode* Simple_exp() {
	int line = token.lineShow ;
	TreeNode* t = Term();
	while (token.lex == PLUS || token.lex == MINUS) {
		TreeNode* p = new TreeNode();
		(*p).lineno = line;
		(*p).nodekind = ExpK;
		(*p).kind.exp = OpK;
		(*p).child[0] = t;
		(*p).attr.expAttr.op = token.lex;
		t = p;
		Match(token.lex);
		(*t).child[1] = Term();
	}
	return t;
}
TreeNode* Term() {
	int line = token.lineShow ;
	TreeNode* t = Factor();
	while (token.lex == TIMES || token.lex == OVER) {
		TreeNode* p = new TreeNode();
		(*p).lineno = line;
		(*p).nodekind = ExpK;
		(*p).kind.exp = OpK;
		(*p).child[0] = t;
		(*p).attr.expAttr.op = token.lex;
		t = p;
		Match(token.lex);
		(*t).child[1] = Factor();
	}
	return t;
}
TreeNode* Factor() {
	TreeNode* t = NULL;
	if (token.lex == INTC) {
		t = new TreeNode();
		(*t).lineno = token.lineShow ;
		(*t).nodekind = ExpK;
		(*t).kind.exp = ConstK;
		(*t).attr.expAttr.val = stoi(token.sem);
		Match(INTC);
	}
	else if (token.lex == ID) {
		t = Variable();
	}
	else if (token.lex == LPAREN) {
		Match(LPAREN);
		t = Exp();
		Match(RPAREN);
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//当前单词为其他单词，
		//非期望单词错误,显示出错单词和信息.并读入下一单词
	}
	return t;
}
TreeNode* Variable() {
	TreeNode* t = new TreeNode();
	(*t).nodekind = ExpK;
	(*t).kind.exp = IdEK;
	(*t).lineno = token.lineShow ;
	if (token.lex == ID) {
		(*t).idnum++;
		(*t).name.push_back(token.sem);
		Match(ID);
		VariMore(t);
	}
	return t;
}
void VariMore(TreeNode* t) {
	if (token.lex == ASSIGN || token.lex == TIMES || token.lex == EQ || token.lex == LT
		|| token.lex == PLUS || token.lex == MINUS || token.lex == OVER || token.lex == RPAREN
		|| token.lex == SEMI || token.lex == COMMA || token.lex == THEN || token.lex == RMIDPAREN
		|| token.lex == ELSE || token.lex == FI || token.lex == DO || token.lex == ENDWH || token.lex == END) {
	}
	else if (token.lex == LMIDPAREN) {
		Match(LMIDPAREN);
		(*t).child[0] = Exp();
		(*t).attr.expAttr.varkind = ArrayMembV;
		(*(*t).child[0]).attr.expAttr.varkind = IdV;
		Match(RMIDPAREN);
	}
	else if (token.lex == DOT) {
		Match(DOT);
		(*t).child[0] = FieldVar();
		(*t).attr.expAttr.varkind = FieldMembV;
		(*(*t).child[0]).attr.expAttr.varkind = IdV;
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//读入下一个token，并提示错误信息
	}
}
TreeNode* FieldVar() {
	TreeNode* t = new TreeNode();
	(*t).nodekind = ExpK;
	(*t).kind.exp = IdEK;
	(*t).lineno = token.lineShow ;
	if (token.lex == ID) {
		(*t).idnum++;
		(*t).name.push_back(token.sem);
		Match(ID);
		FieldVarMore(t);
	}
	return t;
}
void FieldVarMore(TreeNode* t) {
	if (token.lex == ASSIGN || token.lex == TIMES || token.lex == EQ || token.lex == LT
		|| token.lex == PLUS || token.lex == MINUS || token.lex == OVER || token.lex == RPAREN
		|| token.lex == SEMI || token.lex == COMMA || token.lex == THEN || token.lex == RMIDPAREN
		|| token.lex == ELSE || token.lex == FI || token.lex == DO || token.lex == ENDWH || token.lex == END) {
	}
	else if (token.lex == LMIDPAREN) {
		Match(LMIDPAREN);
		(*t).child[0] = Exp();
		(*(*t).child[0]).attr.expAttr.varkind = ArrayMembV;
		Match(RMIDPAREN);
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		//cout << "文件提前结束";
		exit(0);
		//读入下一个token，并提示错误信息
	}
}
void Match(LexType expected) {
	if (token.lex == expected) {
		ReadToken();
	}
	else {
		string a = to_string(token.lineShow ) + "行有错误";
		InputError(a, "./Docs/error.txt");
		exit(0);
	}
}

void ReadToken() {
	string line;
	getline(in, line);
	istringstream inword(line);
	inword >> token.lineShow ;
	int lex;
	inword >> lex;
	token.lex = (LexType)lex;
	inword >> token.sem;
	int lineno = token.lineShow ;
}

void Input(TreeNode* root, string path) {
	ofstream ouput;
	ouput.open(path);
	stack<TreeNode*> inputSort;
	inputSort.push(root);
	while (!inputSort.empty()) {
		TreeNode* t = inputSort.top();
		inputSort.pop();
		ouput << t->child[0] << " " << t->child[1] << " " << t->child[2] << " ";
		ouput << t->sibling << " ";
		ouput << t->lineno << " ";
		ouput << t->nodekind << " ";
		ouput << t->kind.dec << " ";
		ouput << t->idnum << " ";
		for (int i = 0; i < t->idnum; i++) {
			ouput << t->name[i] << " ";
		}
		ouput << t->type_name << " ";
		ouput << t->attr.arrayAttr.childType << " " << t->attr.arrayAttr.low << " " << t->attr.arrayAttr.up << " ";
		ouput << t->attr.expAttr.op << " " << t->attr.expAttr.type << " " << t->attr.expAttr.val << " " << t->attr.expAttr.varkind << " ";
		ouput << t->attr.procAttr.paramt << endl;
		if (t->sibling != NULL) {
			inputSort.push(t->sibling);
		}
		for (int i = 2; i >= 0; i--) {
			if (t->child[i] != NULL) {
				inputSort.push(t->child[i]);
			}
		}
	}
	ouput.close();
}
