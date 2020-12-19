#include "public.h"
using namespace std;

char text[100000]; // save the source code read from the testfile.txt
char token[10000]; // global, save the current string
char symbol[1000];
char tmpChar;
int totalLen; // the length of the text
int cur = 0; // global pointer
int slength = 0;
//文件相关
FILE *fp, *out, *errorfile, *mips_file, *mid_file;
int MAX_LINE = 10000;
char buffer[10000];
bool isFirstInLine = false;
//work3
string sym;
string word;
struct Item s[100000]; //存词法分析的结果
int symcur = 0; //
struct Item grammer[100000];// 存语法分析的结果,包括汉字
int grammerl = 0; // grammer数组的长度
map<string, bool> defType;
// 错误处理
int gline = 1;
bool isInRequirement = false; // 是否在<条件>产生式里面
bool isInFuncDefWithReturn = false; // 是否在<有返回值函数定义>里面
bool isInFuncDefWithoutReturn = false; //是否在<无返回值函数定义>里面
//主map: 全局变量/常量 和 main里面的
map<string, node> globalTable;
//函数定义里面的map，用完之后就clear
map<string, node> localTable;
// 表示现在是在 函数定义 还是 全局变量(包括main)
bool isInFuncDef = false;
// 当前变量的
string name;
string type; // var or const 
string kind;
int g_offset;
string g_scope = "global";
int g_space = 4;
vector<int> g_initial_value_list;
//下面的不用管，只是为了凑够参数
vector<string> parakind;
vector<string> paraname;
//vector<string> paratype;
//
string typeOfExpr;
string typeOfConstant;
string varNameInitialized;
// 函数调用的值参数表
vector<string> valueParameters;
//
bool hasRtnStatement;

//数组的两个维度
int demension1 = 0;
int demension2 = 0;
//switch表达式的类型
string switchType;
// 存a类型error的行数，如果到了就在nextsym输出
vector<int> errorA;

//这是清空全局变量的。
void NTKclear()
{
	name = "";
	type = "";
	kind = "";
	//g_offset = 0;
	parakind.clear();
	paraname.clear();
}

// 将其保存到全局table
void Save2GlobalTable()
{
	node tmp;
	tmp.name = name;
	tmp.type = type;
	tmp.kind = kind;
	tmp.offset = g_offset;
	tmp.scope = "global";
	tmp.space = g_space;
	tmp.d1 = demension1;
	tmp.d2 = demension2;
	tmp.initial_value_list = g_initial_value_list;
	// 参数列表： 如果type是func的话，才会修改和查看
	tmp.parakind = parakind;
	tmp.paraname = paraname;
	//tmp.paratype = paratype;

	globalTable[name] = tmp;
}
// 将其保存到localtable
void Save2LocalTable()
{
	node tmp;
	tmp.name = name;
	tmp.type = type;
	tmp.kind = kind;
	tmp.offset = g_offset;
	tmp.scope = g_scope;
	tmp.d1 = demension1;
	tmp.d2 = demension2;
	tmp.initial_value_list = g_initial_value_list;
	// 参数列表： 如果type是func的话，才会修改和查看
	tmp.parakind = parakind;
	tmp.paraname = paraname;
	//tmp.paratype = paratype;

	localTable[name] = tmp;
}

void Save2Table()
{
	if (isInFuncDef) Save2LocalTable();
	else Save2GlobalTable();
}

// 保存所有local_table
map<string, map<string, node>> all_local_tables;

