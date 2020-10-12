#include "public.h"
#include "work2.h"
#include "work3.h"
using namespace std;

// 查看下一个单词和类别码
void NextSym()
{
	sym = s[symcur].symbolx;
	word = s[symcur].wordx;
	if (symcur < slength) {
		symcur++;
	}
}
// 查看当前单词开始后面第k个单词的类别码
string Peek(int k)
{
	return s[symcur + k - 1].symbolx;
}
// 保存词法分析
void SaveLex()
{
	grammer[grammerl].symbolx = sym;
	grammer[grammerl].wordx = word;
	grammer[grammerl].grammerx = "";
	grammerl++;
}
// 保存语法分析结果
void SaveGrammer(string s)
{
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
	if (sym == "CHARTK") {
		SaveLex();
	}
	else {
		Integer();
	}
}
//＜变量定义及初始化＞  ::= ＜类型标识符＞＜标识符＞=＜常量＞|＜类型标识符＞＜标识符＞'['＜无符号整数＞']'='{'＜常量＞{,＜常量＞}'}'|＜类型标识符＞＜标识符＞'['＜无符号整数＞']''['＜无符号整数＞']'='{''{'＜常量＞{,＜常量＞}'}'{, '{'＜常量＞{,＜常量＞}'}'}'}'
void VariableInitialized()
{
	SaveLex(); // 保存 = 
	//
	NextSym();

	if (sym == "LBRACE") {
		SaveLex(); // {

		// { {c,... } , {}}
		NextSym();

		if (sym == "LBRACE") {
			SaveLex(); // {
			//常量
			NextSym();
			Constant();
			// ，
			while (sym == "COMMA") {
				SaveLex();
				Constant();
			}
			// }
			if (sym != "RBRACE") error();
			SaveLex();

			while (sym == "COMMA") {
				SaveLex();

				// {
				NextSym();
				SaveLex();

				// 常量
				NextSym();
				Constant();

				while (sym == "COMMA") {
					SaveLex();
					Constant();
				}
				// }
				if (sym != "RBRACE") error();
				SaveLex();
			}
		}
		// {c...}
		else {
			Constant();
			while (sym == "COMMA") {
				SaveLex();
				//常量
				NextSym();
				Constant();
			}
		}
		if (sym != "RBRACE") error();
		SaveLex();
		NextSym();
	}
	else {
		Constant();
	}
	SaveGrammer("＜变量定义及初始化＞");
}
// ＜变量定义无初始化＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞'['＜无符号整数＞']'|＜标识符＞'['＜无符号整数＞']''['＜无符号整数＞']'){,(＜标识符＞|＜标识符＞'['＜无符号整数＞']'|＜标识符＞'['＜无符号整数＞']''['＜无符号整数＞']' )}
void VariableUninitialized()
{

	SaveGrammer("＜变量定义无初始化＞");
	//NextSym();
}
//＜变量定义＞ ::= ＜变量定义无初始化＞|＜变量定义及初始化＞
void VariableDefinition()
{
	SaveLex(); // 类型标识符

	NextSym();
	SaveLex(); // 标识符

	// 前面是提取左因子
	NextSym();


	if (sym == "LBRACK") {
		while (sym == "LBRACK") {
			SaveLex();

			// 无符号整数
			NextSym();
			IntegerWithoutSign();
			// 
			if (sym != "RBRACK") error();
			SaveLex();

			NextSym();
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
	VariableDefinition();
	while (sym == "SEMICN") {
		SaveLex();
		NextSym();
		//＜变量定义＞
		if ((Peek(0) == "INTTK" || Peek(0) == "CHARTK") && (Peek(2) != "LPARENT"))
			VariableDefinition();
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
		SaveLex();

		NextSym(); // 标识符,
		SaveLex();
				   //这里都是默认正确的
		// 如果留一个判断错误的接口的话，应该判断每一个字符是不是应该是它

		
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

		NextSym(); // 标识符
		SaveLex();

		NextSym(); // =
		SaveLex();

		NextSym(); // 字符
		SaveLex();

		NextSym(); // 下一个

		while (sym == "COMMA") {
			SaveLex();

			NextSym(); // 标识符
			SaveLex();

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
		if (sym != "SEMICN") error();
		SaveLex();

		NextSym();
	}
	SaveGrammer("<常量说明>");
}

//＜字符串＞   ::=  "｛十进制编码为32,33,35-126的ASCII字符｝"
void StringG()
{
	SaveLex();

	SaveGrammer("＜字符串＞");
	NextSym();
}
// ＜因子＞    ::= ＜标识符＞｜＜标识符＞'['＜表达式＞']'|＜标识符＞'['＜表达式＞']''['＜表达式＞']'|'('＜表达式＞')'｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞    
void Factor()
{
	if (sym == "INTCON") {
		Integer();
	}
	else if (sym == "CHARCON") {
		SaveLex();
		NextSym();
	}
	// （
	else if (Peek(1) == "LPARENT") {
		FunctionWithReturn();
	}
	// 标识符
	else {
		SaveLex();
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
				// ]
				if (sym != "RBRACK") error();
				SaveLex();
				// [
				NextSym();
			}
		}
	}
	SaveGrammer("＜因子＞");
}
// ＜项＞     ::= ＜因子＞{＜乘法运算符＞＜因子＞}   
void Term()
{
	Factor();
	while (sym == "MULT" || sym == "DIV") {
		SaveLex();
		Factor();
	}
	SaveGrammer("＜项＞");
}
//＜表达式＞    ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞} 
void Expression()
{
	if (sym == "PLUS" || sym == "MINU") {
		SaveLex();
		NextSym();
	}
	Term(); /*BUG3: +-之后应该是一个项，这里忘记写了*/
	while (sym == "PLUS" || sym == "MINU") {
		SaveLex();
		NextSym();
		Term();
	}
	SaveGrammer("＜表达式＞");
}

// ＜读语句＞    ::=  scanf '('＜标识符＞')' 
void ReadStatement()
{
	if (sym != "SCANFTK") error();
	SaveLex();
	// （
	NextSym();
	SaveLex();
	// 
	NextSym();
	SaveLex();
	// 
	NextSym();
	SaveLex();
	// 

	SaveGrammer("＜读语句＞");
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
	if (sym != "RPARENT") error();
	SaveLex();

	SaveGrammer("＜写语句＞");
	NextSym();
}

//＜缺省＞   ::=  default :＜语句＞ 
void DefaultStatement()
{
	if (sym != "DEFAULTTK") error();
	SaveLex();
	// :
	NextSym();
	SaveLex();
	// 语句
	NextSym();
	Statement();

	SaveGrammer("＜缺省＞");
}
//＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}   
void CaseTable()
{
	while (sym == "CASETK") {
		CaseSubStatement();
	}
	SaveGrammer("＜情况表＞");
}
//＜情况子语句＞  ::=  case＜常量＞：＜语句＞   
void CaseSubStatement()
{
	if (sym != "CASETK") error();
	SaveLex();
	// 常量
	NextSym();
	Constant();
	//: 
	if (sym != "COLON") error();
	SaveLex();
	//语句
	NextSym();
	Statement();
	SaveGrammer("＜情况子语句＞");
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
	// )
	if (sym != "RPARENT") error();
	SaveLex();
	// {
	NextSym();
	SaveLex();
	// 情况表
	NextSym();
	CaseTable();
	// 缺省
	DefaultStatement();
	//} 
	if (sym != "RBRACE") error();
	SaveLex();

	SaveGrammer("＜情况语句＞");
	NextSym();
}

// ＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞|＜标识符＞'['＜表达式＞']''['＜表达式＞']' =＜表达式＞
void AssignStatement()
{
	// 标识符
	SaveLex();
	// [
	NextSym();
	if (sym == "LBRACK") {
		// [
		while (sym == "LBRACK") {
			SaveLex();

			// 表达式
			NextSym();
			Expression();
			// ]
			if (sym != "RBRACK") error();
			SaveLex();

			// [
			NextSym();
		}
	}
	// 下一个是等号
	if (sym != "ASSIGN") error();

	SaveLex();
	NextSym(); /*BUG4：进表达式之前忘记读取下一个字符了*/
	Expression();

	SaveGrammer("＜赋值语句＞");
}
//＜返回语句＞   ::=  return['('＜表达式＞')']   
void ReturnStatement()
{
	if (sym != "RETURNTK") error();
	SaveLex();
	// (
	NextSym();
	if (sym == "LPARENT") {
		SaveLex();
		NextSym(); /*BUG6: 进表达式之前又忘记取下一个字符了*/
		Expression();
		//)
		if (sym != "RPARENT") error();
		SaveLex();
		NextSym();
	}
	else error();
	SaveGrammer("＜返回语句＞"); /*BUG5: 这里没写！*/
}

//＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞
void ValueParameterTable()
{
	// ) 即为空
	if (sym != "RPARENT") {

		Expression();
		// ,
		while (sym == "COMMA") {
			SaveLex();
			NextSym(); /*BUG7:进表达式之前又忘记调用了*/ 
			Expression();
		}
	}
	SaveGrammer("＜值参数表＞");
}
//＜有返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'   
void FunctionWithReturn()
{
	SaveLex();
	defType[word] = true;
	// (
	NextSym();
	SaveLex();
	// 值参数表
	NextSym();
	ValueParameterTable();
	//)
	if (sym != "RPARENT") error();
	SaveLex();
	SaveGrammer("＜有返回值函数调用语句＞");
	NextSym();
}
// ＜无返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void FunctionWithoutReturn()
{
	SaveLex();
	defType[word] = false;
	// (
	NextSym();
	SaveLex();
	// 值参数表
	NextSym();
	ValueParameterTable();
	//)
	if (sym != "RPARENT") error();
	SaveLex();
	SaveGrammer("＜无返回值函数调用语句＞");
	NextSym();
}

//＜条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞     
void Requirement()
{
	Expression();
	//关系运算符
	NextSym();
	SaveLex();
	//表达式
	NextSym();
	Expression();

	SaveGrammer("＜条件＞");
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
	if (sym != "RPARENT") error();
	SaveLex();
	// 语句
	NextSym();
	Statement();
	//
	if (sym == "ELSETK") {
		SaveLex();
		// 语句
		NextSym();
		Statement();
	}
	SaveGrammer("＜条件语句＞");
}
//＜步长＞::= ＜无符号整数＞  
void Ilength()
{
	IntegerWithoutSign();
	SaveGrammer("＜步长＞");
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
		if (sym != ")") error();
		SaveLex();
		//语句
		NextSym();
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
		// =
		NextSym();
		SaveLex();
		// 表达式
		NextSym();
		Expression();
		//;
		if (sym != "SEMICN") error();
		SaveLex();
		// 条件
		NextSym();
		Requirement();
		//;
		if (sym != "SEMICN") error();
		SaveLex();
		//标识符
		NextSym();
		SaveLex();
		// =
		NextSym();
		SaveLex();
		//标识符
		NextSym();
		SaveLex();
		// （+|-）
		NextSym();
		SaveLex();
		//
		NextSym();
		Ilength();
		// )
		if (sym != "RPARENT") error();
		SaveLex();
		// 语句
		NextSym();
		Statement();
	}
	SaveGrammer("＜循环语句＞");
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

	else if (Peek(1) == "ASSIGN" || Peek(4) == "ASSIGN" || Peek(7) == "ASSIGN") {
		AssignStatement();
		// ;
		if (sym != "SEMICN")  error();
		SaveLex();
		NextSym();
	}
	else if (sym == "SCANFTK") {
		ReadStatement();
		if (sym != "SEMICN")  error();
		SaveLex();
		NextSym();
	}
	else if (sym == "PRINTFTK") {
		WriteStatement();
		if (sym != "SEMICN")  error();
		SaveLex();
		NextSym();
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
		if (sym != "SEMICN")  error();
		SaveLex();
		NextSym();
	}
	else if (sym == "LBRACE") {
		SaveLex();/*BUG2：这里要先将{存起来在调用StatementList,不然死循环了！！！*/
		StatementList();
		if (sym != "RBRACE")  error();
		SaveLex();
		NextSym();
	}
	/*else {

	}*/
	// 有无返回值函数调用///////////////////要把所有函数的标识符都存起来/////这里用wordx判断//////////////这里写的不够好！！
	else if (Peek(1) == "LPARENT" ) {

		map<string, bool>::iterator iter;
		iter = defType.find(word);
		if (iter == defType.end()) error();
		else {
			// 有
			if (iter->second == true) {
				FuncDefWithReturn();
			}
			// 无返回
			else {
				FuncDefWithoutReturn();
			}
		}

		if (sym != "SEMICN") error();
		SaveLex();
		NextSym();
	}

	SaveGrammer("<语句>");

}

//＜语句列＞   ::= ｛＜语句＞｝
void StatementList()
{
	while ((sym == "WHILETK") || (sym == "FORTK") || (sym == "IFTK") || (sym == "SEMICN") || (sym == "RETURNTK")
		|| (sym == "SCANFTK") || (sym == "PRINTFTK") || (sym == "SWITCHTK") || (sym == "LBRACE") || (Peek(1) == "LPARENT")
		|| ((Peek(1) == "ASSIGN" || Peek(4) == "ASSIGN") || (Peek(7) == "ASSIGN"))) {
		Statement();
	}
	/*BUG1:这里没写完整只写了一半*/
	/*while (sym == "LPARENT") {
		StatementList();
	}*/
	
	SaveGrammer("＜语句列＞");
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

	SaveGrammer("＜复合语句＞");
}
// ＜主函数＞    ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’   
void MainFunction()
{
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
	SaveLex();
	//{
	NextSym();
	SaveLex();
	//复合语句
	NextSym();
	CompoundStatement();
	//}
	if (sym != "RBRACE") error();
	SaveLex();

	SaveGrammer("＜主函数＞");
	NextSym();

}
// ＜声明头部＞   ::=  int＜标识符＞ |char＜标识符＞
void DeclareHead()
{
	SaveLex();
	// 标识符
	NextSym();
	SaveLex();
	SaveGrammer("＜声明头部＞");
	NextSym();
}

//＜参数表＞    ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
void ParameterTable()
{
	if (sym == "INTTK" || sym == "CHARTK") {
		SaveLex();
		//标识符
		NextSym();
		SaveLex();
		// 
		NextSym();
		while (sym == "COMMA") {
			SaveLex();
			// 类型标识符
			NextSym();
			SaveLex();
			//标识符
			NextSym();
			SaveLex();
			// 
			NextSym();
		}
	}
	// else 就是空 ，不用处理，直接输出
	SaveGrammer("＜参数表＞");
}

//＜有返回值函数定义＞  ::=  ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'  
void FuncDefWithReturn()
{
	// 声明头部
	DeclareHead();
	// (
	if (sym != "LPARENT") error();
	SaveLex();
	// 参数表
	NextSym();

	//ValueParameterTable();/*BUG0: 看错了定义，这里应该调用的是参数表，不是值参数表*/
	ParameterTable();
	//)
	if (sym != "RPARENT") error();
	SaveLex();

	// {
	NextSym();
	SaveLex();

	// 复合语句
	NextSym();
	CompoundStatement();
	//}
	if (sym != "RBRACE") error();
	SaveLex();

	SaveGrammer("＜有返回值函数定义＞");
	NextSym();
}

// ＜无返回值函数定义＞  ::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
void FuncDefWithoutReturn()
{
	if (sym != "VOIDTK") error();
	SaveLex();
	// 标识符
	NextSym();
	SaveLex();
	// (
	NextSym();
	SaveLex();
	// 参数表
	NextSym();
	ValueParameterTable();
	//}
	if (sym != "RPARENT") error();
	SaveLex();

	// {
	NextSym();
	SaveLex();

	// 复合语句
	NextSym();
	CompoundStatement();
	//}
	if (sym != "RBRACE") error();
	SaveLex();

	SaveGrammer("＜无返回值函数定义＞");
	NextSym();
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
	for (int i = 0; i < grammerl; i++) {
		if (grammer[i].grammerx == "") {
			fprintf(out, "%s %s\n", grammer[i].symbolx.c_str(), grammer[i].wordx.c_str());
		}
		else {
			fprintf(out, "%s\n", grammer[i].grammerx.c_str());
		}
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
	//FuncDefWithReturn();
	//MainFunction();

	// 输出到文件
	Output2File();
	// 关闭文件
	CloseFiles();
	return 0;
}