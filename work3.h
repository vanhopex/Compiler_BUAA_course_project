#ifndef  __WORK3_
#define __WORK3_
#include "public.h"
#include "work2.h"
#include "work3.h"
using namespace std;

// 查看下一个单词和类别码
void NextSym();
// 查看当前单词开始后面第k个单词的类别码
string Peek(int k);
// 保存词法分析
void SaveLex();
// 保存语法分析结果
void SaveGrammer(string s);
//无符号整数
void IntegerWithoutSign();
// 整数
void Integer();
// ＜常量＞   ::=  ＜整数＞|＜字符＞
void Constant();
//＜变量定义及初始化＞  ::= ＜类型标识符＞＜标识符＞=＜常量＞|＜类型标识符＞＜标识符＞'['＜无符号整数＞']'='{'＜常量＞{,＜常量＞}'}'|＜类型标识符＞＜标识符＞'['＜无符号整数＞']''['＜无符号整数＞']'='{''{'＜常量＞{,＜常量＞}'}'{, '{'＜常量＞{,＜常量＞}'}'}'}'
void VariableInitialized();
// ＜变量定义无初始化＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞'['＜无符号整数＞']'|＜标识符＞'['＜无符号整数＞']''['＜无符号整数＞']'){,(＜标识符＞|＜标识符＞'['＜无符号整数＞']'|＜标识符＞'['＜无符号整数＞']''['＜无符号整数＞']' )}
void VariableUninitialized();
//＜变量定义＞ ::= ＜变量定义无初始化＞|＜变量定义及初始化＞
void VariableDefinition();
//＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;} 
// 因为变量定义和声明头部前面几个一样，所以需要区分
void VariableExplanation();
// 常量定义
//＜常量定义＞   :: = int＜标识符＞＝＜整数＞{ ,＜标识符＞＝＜整数＞ }
//                  | char＜标识符＞＝＜字符＞{ ,＜标识符＞＝＜字符＞
void ConstantDefinition();
// ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
void ConstantExplanation();

//＜字符串＞   ::=  "｛十进制编码为32,33,35-126的ASCII字符｝"
void StringG();
// ＜因子＞    ::= ＜标识符＞｜＜标识符＞'['＜表达式＞']'|＜标识符＞'['＜表达式＞']''['＜表达式＞']'|'('＜表达式＞')'｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞    
void Factor();
// ＜项＞     ::= ＜因子＞{＜乘法运算符＞＜因子＞}   
void Term();
//＜表达式＞    ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞} 
void Expression();
void Expression(string& result);
// ＜读语句＞    ::=  scanf '('＜标识符＞')' 
void ReadStatement();

//＜写语句＞    ::= printf '(' ＜字符串＞,＜表达式＞ ')'| printf '('＜字符串＞ ')'| printf '('＜表达式＞')' 
void WriteStatement();

//＜缺省＞   ::=  default :＜语句＞ 
void DefaultStatement();
//＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}   
void CaseTable();
//＜情况子语句＞  ::=  case＜常量＞：＜语句＞   
void CaseSubStatement();
//＜情况语句＞  ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞＜缺省＞‘}’  
void SwitchStatement();

// ＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞|＜标识符＞'['＜表达式＞']''['＜表达式＞']' =＜表达式＞
void AssignStatement();
//＜返回语句＞   ::=  return['('＜表达式＞')']   
void ReturnStatement();

//＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞
void ValueParameterTable();
//＜有返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'   
void FunctionWithReturn();
void FunctionWithReturn(string& result);
// ＜无返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void FunctionWithoutReturn();
//＜条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞     
void Requirement();
//＜条件语句＞  ::= if '('＜条件＞')'＜语句＞［else＜语句＞］ 
void IfStatement();
//＜步长＞::= ＜无符号整数＞  
void Ilength();

//＜循环语句＞   ::=  while '('＜条件＞')'＜语句＞| for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞ 
void WhileStatement();
//＜语句＞    ::= ＜循环语句＞｜＜条件语句＞| ＜有返回值函数调用语句＞;  |＜无返回值函数调用语句＞;｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜情况语句＞｜＜空＞;|＜返回语句＞; | '{'＜语句列＞'}'    
void Statement();

//＜语句列＞   ::= ｛＜语句＞｝
void StatementList();

// ＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞ 
void  CompoundStatement();
// ＜主函数＞    ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’   
void MainFunction();
// ＜声明头部＞   ::=  int＜标识符＞ |char＜标识符＞
void DeclareHead();

//＜参数表＞    ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
void ParameterTable();

//＜有返回值函数定义＞  ::=  ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'  
void FuncDefWithReturn();

// ＜无返回值函数定义＞  ::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
void FuncDefWithoutReturn();

// ＜程序＞    ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞ 
void Program();

// 输出到文件
void Output2File();

int GetLine();

string GetVarOffset(string arr_name, string d1, string d2);
#endif // ! __WORK3_
