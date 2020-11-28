#ifndef __PUBLIC_H
#define __PUBLIC_H
#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <string>
#include <map>
#include <vector>
#include <cctype>//toupper/tolower
#include <algorithm>//transform
#define MAXLE 10000
using namespace std;
extern char text[10000]; // save the source code read from the testfile.txt
extern char token[1000]; // global, save the current string
extern char symbol[10];
extern char tmpChar;
extern int totalLen; // the length of the text
extern int cur; // global pointer
extern int slength;
//
extern FILE* fp, * out, *errorfile;
extern int MAX_LINE;
extern char buffer[100];
extern bool isFirstInLine;
// work3
struct Item {
	std::string symbolx;
	std::string wordx; // 词法分析的结果

	std::string grammerx; // 语法分析结果
	int line;
	bool isfirst = false;
};
extern struct Item s[1000];
extern std::string sym;
extern std::string word;
extern int symcur;
extern struct Item grammer[10000];
extern int grammerl;
extern std::map<std::string, bool> defType;
// 错误处理
struct SymbolTable {
	std::string name;
	std::string type;
	std::string level;
};
extern struct SymbolTable symtable[10000];
// 记录行数
extern int gline;
extern bool isInRequirement;
extern bool isInFuncDefWithReturn; // 是否在<有返回值函数定义>里面
extern bool isInFuncDefWithoutReturn; //是否在<无返回值函数定义>里面
extern bool isInFuncDef; // 是否是在函数声明里面
struct node {
	string name;
	string type; // const or var or func
	string kind; // int / char / 
				//  如果是func , with or without
	// 如果 type是 func
	// 记录其返回值类型，每个参数及类型
	//vector<string> paratype;
	vector<string> parakind;
	vector<string> paraname;
	//
};
//主map: 全局变量/常量 和 main里面的
extern map<string, node> globalTable;
//函数定义里面的map，用完之后就clear
extern map<string, node> localTable;
// 表示现在是在 函数定义 还是 全局变量(包括main)
extern bool isInFuncDef;
void Save2GlobalTable();
// 将其保存到localtable
void Save2LocalTable();

void Save2Table();
extern string typeOfExpr;
extern string typeOfConstant;
extern string varNameInitialized;
// 全局
extern string name;
extern string type; // var or const 
extern string kind;
//下面的不用管，只是为了凑够参数
extern vector<string> parakind;
extern vector<string> paraname;
//extern vector<string> paratype;
//
extern vector<string> valueParameters;
void NTKclear();
//
extern bool hasRtnStatement;
//
extern int demension1;
extern int demension2;
//
extern string switchType;
//
extern vector<int> errorA;
#endif
