#include "public.h"
#include "work2.h"
#include "work3.h"
#include "ErrorHandling.h"
#include "middle_code_generate.h"
#include "query_from_symtable.h"
using namespace std;

/*
* 中间变量的生成是伴随着语法分析进行的
* 并不是等语法分析之后
* 设计中间代码生成的时候，要时刻记住这个时间关系
*/

// 所有中间代码存在这里
vector<FourElements> middle_code; 


void Save2IR(FourElements s)
{
	middle_code.push_back(s);
}

// 全局标签数
int label_number = 1;
//全局产生新的标签
string GenerateLabel()
{
	
	return  "label_" + to_string(label_number++);
	// 加入局部符号表

}


// 产生局部中间变量，在退出表达式的时候将mid_var_num置0
int mid_var_num = 0;
string  GenerateMidVar()
{
	string middle_var = "t_" + to_string(mid_var_num++);
	// 先看是否在符号表中，在就直接返回

	// 若这个中间变量不在符号表中，那就新建一个放进去。
	if (!IsInLocalTable_GA(middle_var)) {
		node tmp;
		tmp.name = middle_var;
		tmp.type = "middle";
		tmp.offset = g_offset;
		tmp.scope = g_scope;
		g_offset += 4;
		localTable[tmp.name] = tmp;
	}
	return middle_var;
}
string  GenerateMidVar(string kind)
{
	string middle_var = "t_" + to_string(mid_var_num++);
	// 先看是否在符号表中，在就直接返回

	// 若这个中间变量不在符号表中，那就新建一个放进去。
	if (!IsInLocalTable_GA(middle_var)) {
		node tmp;
		tmp.name = middle_var;
		tmp.type = "middle";
		tmp.kind = kind;
		tmp.offset = g_offset;
		tmp.scope = g_scope;
		g_offset += 4;
		localTable[tmp.name] = tmp;
	}
	return middle_var;
}
// 退出表达式的时候将mid_var_num置0
void SetVarNum0()
{
	//mid_var_num = 0;
}
// 将语法分析的重载都写到这里来，结构清晰一点

// 函数内/ 变量常量参数偏移设计


