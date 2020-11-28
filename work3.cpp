#include "public.h"
#include "work2.h"
#include "work3.h"
#include "ErrorHandling.h"
using namespace std;
// 查看下一个单词和类别码
void NextSym()
{
	sym = s[symcur].symbolx;
	word = s[symcur].wordx;
	gline = s[symcur].line;
	for (int i = 0; i < errorA.size(); i++) {
		if (gline == errorA[i]) {
			Error('a');
			errorA[i] = -1;
		}
	}

	if (symcur < slength) {
		symcur++;
	}
}
int GetLine()
{
	return s[symcur].line;
}
// 查看当前单词开始后面第k个单词的类别码
string Peek(int k)
{
	return s[symcur + k - 1].symbolx;
}
// 保存词法分析
void SaveLex()
{
	/*if (grammerl == 0) {
		fprintf(out, "%s %s hello\n", sym.c_str(), word.c_str());
	}*/
	grammer[grammerl].symbolx = sym;
	grammer[grammerl].wordx = word;
	grammer[grammerl].grammerx = "";
	grammerl++;
}
// 保存语法分析结果
void SaveGrammer(string s)
{
	if (grammerl == 0) {
		fprintf(out, "%s hello\n", s.c_str());
	}
	grammer[grammerl].grammerx = s;
	grammer[grammerl].symbolx = "";
	grammer[grammerl].wordx = "";
	grammerl++;
}
//无符号整数
void IntegerWithoutSign()
{
	SaveLex(); // 保存无符号整数
	SaveGrammer("<无符号整数>");
	NextSym();
}
// 整数
void Integer()
{
	if (sym == "PLUS" || sym == "MINU") {
		// 保存PLUS or MINU
		SaveLex();
		// 无符号整数
		NextSym();
	}
	IntegerWithoutSign();// 这里调用NextSym();

	// 由于IntegerWithoutSign()调用的NextSym()这里没有用到，就直接返回给上一层了，所以这里不需要再次NextSym()
	SaveGrammer("<整数>");
	/*NextSym();*/
}
// ＜常量＞   ::=  ＜整数＞|＜字符＞
void Constant()
{
	if (sym == "CHARCON") {
		typeOfConstant = "char";
		SaveLex();
		NextSym(); /*BUG:忘记加了*/
	}
	else {
		typeOfConstant = "int";
		Integer();
	}
	SaveGrammer("<常量>");
}
//＜变量定义及初始化＞  ::= ＜类型标识符＞＜标识符＞=＜常量＞|＜类型标识符＞＜标识符＞'['＜无符号整数＞']'='{'＜常量＞{,＜常量＞}'}'|＜类型标识符＞＜标识符＞'['＜无符号整数＞']''['＜无符号整数＞']'='{''{'＜常量＞{,＜常量＞}'}'{, '{'＜常量＞{,＜常量＞}'}'}'}'
void VariableInitialized()
{

	int l1 = 0;
	int l2 = 0;

	SaveLex(); // 保存 = 
	//
	NextSym();
	if (sym == "LBRACE") {
		SaveLex(); // {
		// { {c,... } , {}}
		NextSym();
		
		if (sym == "LBRACE") {
			SaveLex(); // {
			l1++;
			l2 = 0;

			//常量 或者 }
			NextSym();
			if (sym != "RBRACE") {
				Constant();
				l2++;

				if (typeOfConstant != GetIdentifierKind(varNameInitialized)) Error('o');
				// ，
				while (sym == "COMMA") {
					SaveLex();
					NextSym(); /*这里又是进入函数之前没有NextSym()*/
					Constant();
					l2++;
					if (typeOfConstant != GetIdentifierKind(varNameInitialized)) Error('o');
				}
				// }

			}
			// }
			if (sym != "RBRACE") error();
			SaveLex();
			

			if (demension2 != l2) Error('n');



			NextSym(); /*又是进入前忘记加了*/ 
			while (sym == "COMMA") {
				SaveLex();

				// {
				NextSym();
				SaveLex();
				l1++;
				l2 = 0;
				// 常量 | }
				NextSym();
				if (sym != "RBRACE") {
					Constant();
					l2++;
					while (sym == "COMMA") {
						SaveLex();
						NextSym(); /*BUG*/
						Constant();
						l2++;
						if (typeOfConstant != GetIdentifierKind(varNameInitialized)) Error('o');
					}
				}
				// }
				if (sym != "RBRACE") error();
				SaveLex();

				if (demension2 != l2) Error('n');
				NextSym(); /*又是忘记加了*/
			}
			if (demension1 != l1) Error('n');
		}
		// {c...}
		else {
			Constant();
			l1++;
			if (typeOfConstant != GetIdentifierKind(varNameInitialized)) Error('o');
			while (sym == "COMMA") {
				SaveLex();
				//常量
				NextSym();
				Constant();
				l1++;
				if (typeOfConstant != GetIdentifierKind(varNameInitialized)) Error('o');
			}

			if (demension1 != l1) Error('n');
		}
		if (sym != "RBRACE") error();
		SaveLex();
		NextSym();
	}
	else {
		Constant();
		if (typeOfConstant != GetIdentifierKind(varNameInitialized)) Error('o');
	}

	//if (demension1 != l1 ) Error('n');

	SaveGrammer("<变量定义及初始化>");
}
// ＜变量定义无初始化＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞'['＜无符号整数＞']'|＜标识符＞'['＜无符号整数＞']''['＜无符号整数＞']'){,(＜标识符＞|＜标识符＞'['＜无符号整数＞']'|＜标识符＞'['＜无符号整数＞']''['＜无符号整数＞']' )}
void VariableUninitialized()
{
	

	while (sym == "COMMA") {
		SaveLex();
		//
		NextSym();
		SaveLex(); // 标识符
		
		name = word;
		if (isRedefined(word)) Error('b');
		Save2Table();
		

		NextSym();
		if (sym == "LBRACK") {
			while (sym == "LBRACK") {
				SaveLex();
				// 无符号整数
				NextSym();

				IntegerWithoutSign();
				// 
				if (sym != "RBRACK") Error('m');
				else {
					SaveLex();
					NextSym();
				}
			}
		}
	}
	SaveGrammer("<变量定义无初始化>");
	//NextSym();
}
//＜变量定义＞ ::= ＜变量定义无初始化＞|＜变量定义及初始化＞
void VariableDefinition()
{
	demension1 = 0;
	demension2 = 0;

	SaveLex(); // 类型标识符
	if (sym == "INTTK") kind = "int";
	else				kind = "char";
	NextSym();
	SaveLex(); // 标识符
	
	name = word;
	varNameInitialized = name;
	if (isRedefined(word)) Error('b');
	Save2Table();
	

	// 前面是提取左因子
	NextSym();
	int i = 1;
	if (sym == "LBRACK") {
		while (sym == "LBRACK") {
			SaveLex();

			// 无符号整数
			NextSym();
			if (i == 1) {
				demension1 = Str2Int(word);
			}
			else if (i == 2) {
				demension2 = Str2Int(word);
			}

			IntegerWithoutSign();
			// 
			if (sym != "RBRACK") Error('m');
			else {
				SaveLex();

				NextSym();
			}
			i++;
		}
	}
	// 下一个是等号的话，就是有初始化，否则无初始化
	if (sym == "ASSIGN") {
		VariableInitialized();
	}
	else {
		VariableUninitialized();
	}
	SaveGrammer("<变量定义>");
}
//＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;} 
// 因为变量定义和声明头部前面几个一样，所以需要区分
void VariableExplanation()
{
	NTKclear();
	type = "var";

	VariableDefinition();
	// ;
	if (sym != "SEMICN") Error('k');
	else {
		SaveLex();
		NextSym();
	}
	while (sym == "INTTK" || sym == "CHARTK") { /*BUG: 如果下一句是空语句这里就会出错*/ 
		if (sym == "INTTK") kind = "int";
		else				kind = "char";

		//＜变量定义＞
		if ((Peek(0) == "INTTK" || Peek(0) == "CHARTK") && (Peek(2) != "LPARENT")) {
			
			VariableDefinition();
			//Save2Table();

			if (sym != "SEMICN") Error('k');
			else {
				SaveLex();
				NextSym();
			}
		}
		else break; // 空语句这里就跳出了
	}
	SaveGrammer("<变量说明>");
}

// 常量定义
//＜常量定义＞   :: = int＜标识符＞＝＜整数＞{ ,＜标识符＞＝＜整数＞ }
//                  | char＜标识符＞＝＜字符＞{ ,＜标识符＞＝＜字符＞
void ConstantDefinition()
{
	
	// int
	if (sym == "INTTK") {
		kind = "int";
		SaveLex();

		NextSym(); // 标识符,
		SaveLex();
				   //这里都是默认正确的
		// 如果留一个判断错误的接口的话，应该判断每一个字符是不是应该是它
		name = word;
		if (isRedefined(word)) Error('b');
		Save2Table();
		
		
		NextSym(); // =
		SaveLex(); // 保存 =

		//整数
		NextSym();  // 只是为了查看下一个sym，不用保存
		Integer(); // 整数调用

		// 如果是整数调用完下一个是一个逗号
		while (sym == "COMMA") {
			SaveLex(); // 先保存这个COMMA

			//下面是新一轮的上面循环//
			// 标识符,
			NextSym(); 
			SaveLex();
			name = word;
			if (isRedefined(word)) Error('b');
			Save2Table();
			
			// =
			NextSym(); 
			SaveLex(); 
			//整数
			NextSym(); 
			Integer(); 
		}
	}
	// char
	else if (sym == "CHARTK") {
		SaveLex();
		kind = "char";

		NextSym(); // 标识符
		SaveLex();
		
		name = word;
		if (isRedefined(word)) Error('b');
		Save2Table();
		

		NextSym(); // =
		SaveLex();

		NextSym(); // 字符
		SaveLex();

		NextSym(); // 下一个

		while (sym == "COMMA") {
			SaveLex();

			NextSym(); // 标识符
			SaveLex();
			
			name = word;
			if (isRedefined(word)) Error('b');
			Save2Table();
			

			NextSym(); // =
			SaveLex();

			NextSym(); // 字符
			SaveLex();

			NextSym(); // 下一个
		}
	}
	else {
		error();
	}

	SaveGrammer("<常量定义>"); 
	// 上面两个while已经调用了一次 NextSym()
}
// ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
void ConstantExplanation()
{
	NTKclear();
	type = "const";
	// const
	if (sym != "CONSTTK") error();
	//SaveLex();

	// ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
	while (sym == "CONSTTK") {
		SaveLex();
		// 常量定义
		NextSym();
		ConstantDefinition();
		// ;
		if (sym != "SEMICN") Error('k');
		else {
			SaveLex();
			NextSym();
		}
		
	}
	SaveGrammer("<常量说明>");
}

//＜字符串＞   ::=  "｛十进制编码为32,33,35-126的ASCII字符｝"
void StringG()
{
	SaveLex();

	SaveGrammer("<字符串>");
	NextSym();
}
// ＜因子＞    ::= ＜标识符＞｜＜标识符＞'['＜表达式＞']'|＜标识符＞'['＜表达式＞']''['＜表达式＞']'|'('＜表达式＞')'｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞    
void Factor()
{
	// 不是缺少，是不同，可以原计划编译
	// 这里得用符号表
	//if (isInRequirement && !(sym == "INTCON" || sym == "PLUS" || sym == "MINU")) Error('f');


	if (sym == "INTCON" || sym == "PLUS" || sym == "MINU") { /*BUG 整数的文法没看全*/
		Integer();
	}
	else if (sym == "CHARCON") {
		//if (isInRequirement) Error('f');
		typeOfExpr = "char";

		SaveLex();
		NextSym();
	}
	// 表达式
	else if (sym == "LPARENT") {
		SaveLex();
		//
		NextSym();
		Expression();
		if (sym != "RPARENT") Error('l');

		else {
			SaveLex();
			NextSym();
		}
	}
	// （
	else if (Peek(1) == "LPARENT") {

		//  此时的word就是函数的名字
		// 
		// 在全局表里面找func的类型
		
		/*while (iter != globalTable.end()) {
			if (iter->first == word) {
				rtntype = iter->second
			}
			iter++;
		}*/
		string tmpname = word;

		FunctionWithReturn();

		if (GetFuncKind(tmpname) == "char") {
			typeOfExpr = "char";
		}
		else {
			typeOfExpr = "int";
		}
	}
	// 标识符
	else {
		SaveLex();
		string typeOfIdentifier = "int";
		// 标识符是否定义
		if (!IsDefined(word)) Error('c');

		// 查找标识符的类型
		else if (GetIdentifierKind(word) == "char") {
			//typeOfExpr = "char";
			typeOfIdentifier = "char";
		}
		//
		NextSym();
		//[
		if (sym == "LBRACK") {
			// [
			while (sym == "LBRACK") {
				SaveLex();

				//  表达式
				NextSym();
				Expression();
				if (typeOfExpr != "int") Error('i');
				// ]
				if (sym != "RBRACK") Error('m');
				else {
					SaveLex();
					// [
					NextSym();
				}
				
			}
		}
		typeOfExpr = typeOfIdentifier;
	}
	SaveGrammer("<因子>");
}
// ＜项＞     ::= ＜因子＞{＜乘法运算符＞＜因子＞}   
void Term()
{
	Factor();
	while (sym == "MULT" || sym == "DIV") {
		

		SaveLex();
		NextSym(); /*BUG: 进函数之前忘记加NextSym()*/
		Factor();

		typeOfExpr = "int";
	}
	SaveGrammer("<项>");
}
//＜表达式＞    ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞} 
void Expression()
{
	typeOfExpr = "int";


	if (sym == "PLUS" || sym == "MINU") {
		SaveLex();
		NextSym();
	}
	Term(); /*BUG3: +-之后应该是一个项，这里忘记写了*/
	
	while (sym == "PLUS" || sym == "MINU") {

		

		SaveLex();
		NextSym();
		Term();

		typeOfExpr = "int";
	}
	SaveGrammer("<表达式>");
}

// ＜读语句＞    ::=  scanf '('＜标识符＞')' 
void ReadStatement()
{
	if (sym != "SCANFTK") error();
	SaveLex();
	// （
	NextSym();
	SaveLex();
	// 标识符
	NextSym();
	SaveLex();
	if (GetType(word) == "const") Error('j');
	if (!IsDefined(word)) Error('c');
	// )
	NextSym();
	SaveLex();
	// 
	SaveGrammer("<读语句>");
	NextSym();
}

//＜写语句＞    ::= printf '(' ＜字符串＞,＜表达式＞ ')'| printf '('＜字符串＞ ')'| printf '('＜表达式＞')' 
void WriteStatement()
{
	if (sym != "PRINTFTK") error();
	SaveLex();
	// (
	NextSym();
	SaveLex();
	//
	NextSym();
	if (sym == "STRCON") {
		StringG();
		if (sym == "COMMA") {
			SaveLex();
			// 表达式
			NextSym();
			Expression();
		}
	}
	else {
		Expression();
	}
	//////////////////////////
	if (sym != "RPARENT") Error('l');
	else {
		SaveLex();
		NextSym();
	}
	SaveGrammer("<写语句>");
}

//＜缺省＞   ::=  default :＜语句＞ 
void DefaultStatement()
{
	if (sym != "DEFAULTTK") Error('p');
	else {
		// default
		SaveLex();
		// :
		NextSym();
		SaveLex();
		// 语句
		NextSym();
		Statement();
	}
	SaveGrammer("<缺省>");
}
//＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}   
void CaseTable()
{
	while (sym == "CASETK") {
		CaseSubStatement();
	}
	SaveGrammer("<情况表>");
}
//＜情况子语句＞  ::=  case＜常量＞：＜语句＞   
void CaseSubStatement()
{
	if (sym != "CASETK") error();
	SaveLex();
	// 常量
	NextSym();
	Constant();

	if (typeOfConstant != switchType) Error('o');
	//: 
	if (sym != "COLON") error();
	SaveLex();
	//语句
	NextSym();
	Statement();
	SaveGrammer("<情况子语句>");
}
//＜情况语句＞  ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞＜缺省＞‘}’  
void SwitchStatement()
{

	if (sym != "SWITCHTK") error();
	SaveLex();
	// (
	NextSym();
	SaveLex();
	// 表达式
	NextSym();
	Expression();

	switchType = typeOfExpr;
//get	typeOfExpr
	// )
	if (sym != "RPARENT") Error('l');
	else {
		SaveLex();
		// {
		NextSym();
	}
	SaveLex();
	// 情况表
	NextSym();
	CaseTable();
	// 缺省
	DefaultStatement();
	//} 
	if (sym != "RBRACE") error();
	SaveLex();

	SaveGrammer("<情况语句>");
	NextSym();
}

// ＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞|＜标识符＞'['＜表达式＞']''['＜表达式＞']' =＜表达式＞
void AssignStatement()
{
	// 标识符
	SaveLex();
	if (GetType(word) == "const") Error('j');
	if (!IsDefined(word)) Error('c');
	// [
	NextSym();
	if (sym == "LBRACK") {
		// [
		while (sym == "LBRACK") {
			SaveLex();

			// 表达式
			NextSym();
			Expression();
			if (typeOfExpr != "int") Error('i');
			// ]
			if (sym != "RBRACK") Error('m');
			else {
				SaveLex();
				// [
				NextSym();
			}
			
		}
	}
	// 下一个是等号
	if (sym != "ASSIGN") error();

	SaveLex();
	NextSym(); /*BUG4：进表达式之前忘记读取下一个字符了*/
	Expression();
	cout << typeOfExpr << "  zhang" << endl;

	SaveGrammer("<赋值语句>");
}
//＜返回语句＞   ::=  return['('＜表达式＞')']   
void ReturnStatement()
{

	/*string rtntype;
	string rtnkind;
	map<string, node>::iterator iter;
	iter = localTable.begin();
	while (iter != localTable.end()) {
		if (iter->second.type == "func") {
			rtntype = iter->second.type;
			rtnkind = iter->second.kind;
			break;
		}
		iter++;
	}*/
	//string rtnkind = GetFuncKindInLocal();

	// return
	SaveLex();
	// ( 或者 ;
	NextSym();

	// (
	if (sym == "LPARENT") {
		SaveLex();

		// 若该函数的类型为void
		if (GetFuncKindInLocal() == "void") {
			Error('g');
		}

		// 另外就是有返回值的return，包括char和int
	    // 判断表达式的类型和retkind是不是一致
		// 思考如何确定表达式的类型

		NextSym(); /*BUG6: 进表达式之前又忘记取下一个字符了*/

		// return ()
		if ((GetFuncKindInLocal() == "int" || GetFuncKindInLocal() == "char") && sym == "RPARENT") {
			Error('h');
		}

		Expression();
		// return int;
		if ((GetFuncKindInLocal() == "int" || GetFuncKindInLocal() == "char") && GetFuncKindInLocal() != typeOfExpr) {
			Error('h');
		}


		//)
		if (sym != "RPARENT") Error('l');
		else {
			SaveLex();
			NextSym();
		}
	}
	// ;
	else {
		if ((GetFuncKindInLocal() == "int" || GetFuncKindInLocal() == "char") && sym == "SEMICN") {
			Error('h');
		}
	}
	//else error();
	SaveGrammer("<返回语句>"); /*BUG5: 这里没写！*/
}

//＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞
void ValueParameterTable()
{
	// ) 即为空/////////////////////////////////////////////////////////////////////
	if (sym != "RPARENT" && sym != "SEMICN") {

		Expression();
		if (typeOfExpr == "char") valueParameters.push_back("char");
		else					  valueParameters.push_back("int");
		// ,
		while (sym == "COMMA") {
			SaveLex();
			NextSym(); /*BUG7:进表达式之前又忘记调用了*/ 
			Expression();
			if (typeOfExpr == "char") valueParameters.push_back("char");
			else					  valueParameters.push_back("int");
		}
	}
	SaveGrammer("<值参数表>");
}
//＜有返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'   
void FunctionWithReturn()
{
	valueParameters.clear();
	// 标识符
	SaveLex();
	vector<string> parameterList = GetParKinds(word);
	// 
	/*BUG*/
	// (
	NextSym();
	SaveLex();
	// 值参数表
	NextSym();
	ValueParameterTable();
	// 比较两个vector是否相等
	if (valueParameters.size() !=  parameterList.size()) Error('d');
	// 可以直接比较
	else if (valueParameters != parameterList) Error('e');

	valueParameters.clear();
	//)
	if (sym != "RPARENT") Error('l');
	else {
		SaveLex();
		NextSym();
	}
	SaveGrammer("<有返回值函数调用语句>");
}
// ＜无返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void FunctionWithoutReturn()
{
	valueParameters.clear();
	SaveLex();
	vector<string> parameterList = GetParKinds(word);
	// (
	NextSym();
	SaveLex();
	// 值参数表
	NextSym();
	ValueParameterTable();
	// 比较两个vector是否相等
	if (valueParameters.size() != parameterList.size()) Error('d');
	// 可以直接比较
	else if (valueParameters != parameterList) Error('e');
	//)
	if (sym != "RPARENT") Error('l');
	else {
		SaveLex();
		NextSym();
	}
	SaveGrammer("<无返回值函数调用语句>");
}

//＜条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞     
void Requirement()
{
	isInRequirement = true;

	Expression();
	if (typeOfExpr != "int") Error('f');
	//关系运算符
	SaveLex();
	//表达式
	NextSym();
	Expression();
	if (typeOfExpr != "int") Error('f');
	isInRequirement = false;
	
	SaveGrammer("<条件>");
}

//＜条件语句＞  ::= if '('＜条件＞')'＜语句＞［else＜语句＞］ 
void IfStatement()
{
	// if
	if (sym != "IFTK") error();
	SaveLex();
	//(
	NextSym();
	SaveLex();
	//
	NextSym();
	Requirement();
	//)
	if (sym != "RPARENT") Error('l');
	else {
		SaveLex();
		// 语句
		NextSym();
	}
	Statement();
	//
	if (sym == "ELSETK") {
		SaveLex();
		// 语句
		NextSym();
		Statement();
	}
	SaveGrammer("<条件语句>");
}
//＜步长＞::= ＜无符号整数＞  
void Ilength()
{
	IntegerWithoutSign();
	SaveGrammer("<步长>");
}

//＜循环语句＞   ::=  while '('＜条件＞')'＜语句＞| for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞ 
void WhileStatement()
{
	if (sym == "WHILETK") {
		// while
		SaveLex();
		//(
		NextSym();
		SaveLex();
		//条件
		NextSym();
		Requirement();
		// )
		if (sym != "RPARENT") Error('l');
		else {
			SaveLex();
			//语句
			NextSym();
		}
		Statement();
	}
	else if (sym == "FORTK") {
		//for
		SaveLex();
		//(
		NextSym();
		SaveLex();
		//标识符
		NextSym();
		SaveLex();
		if (GetType(word) == "const") Error('j');
		if (!IsDefined(word)) Error('c');
		// =
		NextSym();
		SaveLex();
		// 表达式
		NextSym();
		Expression();
		//;
		if (sym != "SEMICN") Error('k');
		else {
			SaveLex();
			// 条件
			NextSym();
		}
		Requirement();
		//;
		if (sym != "SEMICN") Error('k');
		else {
			SaveLex();
			//标识符
			NextSym();
			if (GetType(word) == "const") Error('j');
			if (!IsDefined(word)) Error('c');
		}
		
		SaveLex();
		// =
		NextSym();
		SaveLex();
		//标识符
		NextSym();
		SaveLex();
		if (!IsDefined(word)) Error('c');
		// （+|-）
		NextSym();
		SaveLex();
		//
		NextSym();
		Ilength();
		// )
		if (sym != "RPARENT") Error('l');
		else {
			SaveLex();
			// {
			//NextSym();
			//SaveLex(); /*BUG 忘记{*/
			// 语句
			NextSym();
		}
		Statement();
	}
	SaveGrammer("<循环语句>");
}
//＜语句＞    ::= ＜循环语句＞｜＜条件语句＞| ＜有返回值函数调用语句＞;  |＜无返回值函数调用语句＞;｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜情况语句＞｜＜空＞;|＜返回语句＞; | '{'＜语句列＞'}'    
void Statement()
{

	if (sym == "WHILETK" || sym == "FORTK") {
		WhileStatement();
	}
	else if (sym == "IFTK") {
		IfStatement();
	}
	else if ((Peek(1) == "ASSIGN") || (Peek(1) == "LBRACK")) {
	//else if (Peek(1) == "ASSIGN" || (Peek(4) == "ASSIGN"&&Peek(1) == "LBRACK") ||( Peek(7) == "ASSIGN" && Peek(1) == "LBRACK" && Peek(4) == "LBRACK")) {
		AssignStatement();
		// ;
		if (sym != "SEMICN")  Error('k');
		else {
			SaveLex();
			NextSym();
		}
		
	}
	else if (sym == "SCANFTK") {
		ReadStatement();
		if (sym != "SEMICN")  Error('k');
		else {
			SaveLex();
			NextSym();
		}
		
	}
	else if (sym == "PRINTFTK") {
		WriteStatement();
		if (sym != "SEMICN")  Error('k');
		else {
			SaveLex();
			NextSym();
		}
		
	}
	else if (sym == "SWITCHTK") {
		SwitchStatement();
	}
	// 空
	else if (sym == "SEMICN") {
		SaveLex();
		NextSym();
	}
	else if (sym == "RETURNTK") {
		ReturnStatement();
		if (sym != "SEMICN")  Error('k');
		else {
			SaveLex();
			NextSym();
		}
		
	}
	else if (sym == "LBRACE") {
		SaveLex();/*BUG2：这里要先将{存起来在调用StatementList,不然死循环了！！！*/
		NextSym(); /*BUG 这里又忘记加了*/
		StatementList();
		if (sym != "RBRACE")  error();
		SaveLex();
		NextSym();
	}
	/*else {

	}*/
	// 有无返回值函数调用///////////////////要把所有函数的标识符都存起来/////这里用wordx判断//////////////这里写的不够好！！
	else if (Peek(1) == "LPARENT" ) {

		if (!IsDefined(word)) {
			Error('c');
			while (sym != "SEMICN") {
				SaveLex();
				NextSym();
			}
		}
		else {
			map<string, bool>::iterator iter;
			iter = defType.find(word);
			if (iter == defType.end()) error();
			else {
				// 有
				if (iter->second == true) {
					FunctionWithReturn();
				}
				// 无返回
				else {
					//FuncDefWithoutReturn();
					FunctionWithoutReturn(); /*BUG函数调用写成函数定义了*/
				}
			}
		}

		if (sym != "SEMICN") Error('k');
		else {
			SaveLex();
			NextSym();
		}
		
	}

	SaveGrammer("<语句>");
}

//＜语句列＞   ::= ｛＜语句＞｝
void StatementList()
{
	// 赋值语句 a=  a[]=  a[][]=
	while ((sym == "WHILETK") || (sym == "FORTK") || (sym == "IFTK") || (sym == "SEMICN") || (sym == "RETURNTK")
		|| (sym == "SCANFTK") || (sym == "PRINTFTK") || (sym == "SWITCHTK") || (sym == "LBRACE") || (Peek(1) == "LPARENT")
		|| ((Peek(1) == "ASSIGN")  || ( Peek(1) == "LBRACK") )) {
		if (sym == "RETURNTK") hasRtnStatement = true;
		Statement();
	}
	/*BUG1:这里没写完整只写了一半*/
	/*while (sym == "LPARENT") {
		StatementList();
	}*/
	
	SaveGrammer("<语句列>");
}

// ＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞ 
void  CompoundStatement()
{
	// 这是常量说明
	if (sym == "CONSTTK") {
		ConstantExplanation();
	}
	// 这肯定是变量说明，不是： int func()
	if ((Peek(0) == "INTTK" || Peek(0) == "CHARTK") && (Peek(2) != "LPARENT")) {
		VariableExplanation();
	}
	// 语句列
	StatementList();

	SaveGrammer("<复合语句>");
}
// ＜主函数＞    ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’   
void MainFunction()
{
	// 主函数也是一个部分函数

	NTKclear();
	type = "func";
	name = "main";
	kind = "void";

	// void
	if (Peek(1) != "MAINTK") error();
	SaveLex();
	// main
	NextSym();
	SaveLex();
	//(
	NextSym();
	SaveLex();
	//)
	NextSym();
	if (sym != "RPARENT") Error('l'); 
	else {
		
		SaveLex();
		//{
		NextSym();
	}
	
	SaveLex();
	// 将这个函数的信息放到globalTable
	Save2Table();
	// 也要把这个信息放到局部变量里面,
	Save2LocalTable();

	isInFuncDef = true;

	//复合语句
	NextSym();

	CompoundStatement();
	//}
	if (sym != "RBRACE") error();
	SaveLex();

	SaveGrammer("<主函数>");
	NextSym();

	isInFuncDef = false;
	localTable.clear();
}
// ＜声明头部＞   ::=  int＜标识符＞ |char＜标识符＞
void DeclareHead()
{
	SaveLex();
	// 标识符
	NextSym();
	SaveLex();
	if (isRedefined(word)) Error('b');
	name = word;
	defType[word] = true;
	SaveGrammer("<声明头部>");
	NextSym();
}

//＜参数表＞    ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
void ParameterTable()
{
	if (sym == "INTTK" || sym == "CHARTK") {
		SaveLex();
		if (sym == "INTTK") parakind.push_back("int");
		else				parakind.push_back("char");
		//标识符
		NextSym();
		paraname.push_back(word);
		//paratype.push_back(word);

		defType[word] = true;
		// 
		NextSym();
		while (sym == "COMMA") {
			SaveLex();
			// 类型标识符
			NextSym();
			SaveLex();

			if (sym == "INTTK") parakind.push_back("int");
			else				parakind.push_back("char");
			
			//标识符
			NextSym();
			SaveLex();
			paraname.push_back(word);
			// 
			NextSym();
		}
	}
	// else 就是空 ，不用处理，直接输出
	SaveGrammer("<参数表>");
}

//＜有返回值函数定义＞  ::=  ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'  
void FuncDefWithReturn()
{
	NTKclear();
	//parakind.clear(); //参数表清空

	type = "func";
	//kind = "with";
	if (sym == "INTTK") kind = "int";
	else				kind = "char";
	
	// 声明头部
	DeclareHead();
	
	// (
	if (sym != "LPARENT") error();
	SaveLex();
	// 参数表
	NextSym();
	//ValueParameterTable();/*BUG0: 看错了定义，这里应该调用的是参数表，不是值参数表*/
	ParameterTable();

	// 把函数的信息放到全局变量里面
	Save2Table();
	// 也要把这个信息放到局部变量里面,
	Save2LocalTable();
	//)
	if (sym != "RPARENT") Error('l');

	else {
		SaveLex();
		// {
		NextSym();
	}
	SaveLex();

	isInFuncDef = true;  // 标志当前的状态在函数定义里面
	// 复合语句
	NextSym();
	hasRtnStatement = false;
	CompoundStatement();
	// 这里判断前面的复合语句中出没出现return语句
	if (hasRtnStatement == false) Error('h');
	//}
	if (sym != "RBRACE") error();
	SaveLex();

	SaveGrammer("<有返回值函数定义>");
	NextSym();

	isInFuncDef = false;
	localTable.clear();
}

// ＜无返回值函数定义＞  ::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
void FuncDefWithoutReturn()
{
	NTKclear();
	//parakind.clear();

	type = "func";
	kind = "void";

	if (sym != "VOIDTK") error();
	SaveLex();
	// 标识符
	NextSym();
	SaveLex();
	name = word;
	if (isRedefined(word)) Error('b');
	defType[word] = false;
	// (
	NextSym();
	SaveLex();
	// 参数表
	NextSym();
	ParameterTable(); /*BUG*/ 
	//}
	if (sym != "RPARENT") Error('l');
	else {
		SaveLex();
		// {
		NextSym();
	}
	// 参数表也放进parakind里面了,将这个函数的信息放到globalTable
	Save2Table();
	// 也要把这个信息放到局部变量里面,
	Save2LocalTable(); 


	// 前面的还是存到globalTable，后面的是局部变量
	isInFuncDef = true; // 标志当前的状态在函数定义里面
	SaveLex();
	// 复合语句
	NextSym();
	CompoundStatement();
	//}
	if (sym != "RBRACE") error();
	SaveLex();

	SaveGrammer("<无返回值函数定义>");
	NextSym();
	isInFuncDef = false;


	localTable.clear();
}

// ＜程序＞    ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞ 
void Program()
{
	if (sym == "CONSTTK") {
		ConstantExplanation();
	}
	if ((Peek(0) == "INTTK" || Peek(0) == "CHARTK") && (Peek(2) != "LPARENT")) {
		VariableExplanation();
	}
	//
	while ((Peek(2) == "LPARENT") && Peek(1) != "MAINTK") {
		if ((Peek(0) == "INTTK" || Peek(0) == "CHARTK")) { // 有返回值的函数定义
			FuncDefWithReturn();
		}
		else {
			FuncDefWithoutReturn();
		}
	}
	MainFunction();

	SaveGrammer("<程序>");
}

// 输出到文件
void Output2File()
{
	//fprintf(out, "%d\n", grammerl);
	for (int i = 0; i < grammerl; i++) {
		if (grammer[i].grammerx == "") {
			//fprintf(out, "%s %s\n","symbol", "word");
			fprintf(out, "%s %s\n", grammer[i].symbolx.c_str(), grammer[i].wordx.c_str());
		}
		else {
			//fprintf(out, "%s\n", "grammer");
			fprintf(out, "%s\n", grammer[i].grammerx.c_str());
		}
	}
}

void PrintTable()
{
	cout << "this is printTable function" << endl;
	map<string, node>::iterator iter;
	iter = globalTable.begin();
	while (iter != globalTable.end()) {
		cout << iter->second.name << " " << iter->second.type << " " << iter->second.kind << endl;
		if (iter->second.parakind.size() != 0) {
			cout << "size of the paralist:  " << iter->second.parakind.size() << endl;
		}
		iter++;
	}
}

int main()
{
	// 读文件
	ReadFiles();
	// 执行work2
	work2();
	//常量说明
	NextSym();
	Program();


	// 检查符号表：
	//PrintTable();

	//FuncDefWithReturn();
	//MainFunction();
	//Statement();
	// 输出到文件
	Output2File();
	// 关闭文件
	CloseFiles();
	return 0;
}