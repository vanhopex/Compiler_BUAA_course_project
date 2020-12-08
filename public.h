#ifndef __PUBLIC_H
#define __PUBLIC_H
#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <string>
#include <queue>
#include <map>
#include <stack>
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
	string name; // 变量名/参数名/函数名

	string type; // const or var or func or middle
	string kind; // int / char / 
				//  如果是func , with or without
	// 如果 type是 func
	// 记录其返回值类型，每个参数及类型
	//vector<string> paratype;
	vector<string> parakind;
	vector<string> paraname;
	
	/*记录变量、常量、参数在栈中的偏移*/
	/*func的参数同变量一起记录，只不过其type改成para*/
	// 偏移表示的是，这个变量名在栈中起始位置
	int offset = 0;  // 就是起始偏移，比如数组a[],的首地址
	int space = 0;  //  全局变量在内存中分配空间的大小，
	string scope;    // 标志这个变量在哪个函数里面或者全局变量/  函数名 或者 global

	// 对于数组变量，标记其是数组记录其两个维度的信息
	bool is_arr = false;
	int d1 = 0; // 维度1
	int d2 = 0; // 维度2

	// 数组的初值放到下面的int数组里面
	vector<int> initial_value_list;

};

//主map: 全局变量/常量 
extern map<string, node> globalTable;
//函数定义里面的map，用完之后就clear
extern map<string, node> localTable;
// 表示现在是在 函数定义 还是 全局变量(包括main)
extern bool isInFuncDef;
// 所有局部符号表
extern map<string, map<string, node>> all_local_tables;


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
extern int g_offset;
extern string g_scope; // 
extern int g_space;
extern vector<int> g_initial_value_list; // 存放数组的初值
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
