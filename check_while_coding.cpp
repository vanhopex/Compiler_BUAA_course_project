#include "public.h"
#include "work2.h"
#include "work3.h"
#include "ErrorHandling.h"
#include "middle_code_generate.h"
using namespace std;

void PrintTable()
{
	// 全局符号表的所有信息
	cout << "In Global Table" << endl;
	map<string, node>::iterator g_iter = globalTable.begin();
	while (g_iter != globalTable.end()) {
		printf("%s, %s, %s, %d; d1: %d, d2: %d\n", g_iter->second.name.c_str(), g_iter->second.type.c_str(), g_iter->second.kind.c_str(), g_iter->second.space, g_iter->second.d1, g_iter->second.d2);
		g_iter++;
	}

	// 所有局部符号表， 和其内部的符号信息
	cout << "\nLocal Table \n" << endl;
	map<string, map<string, node>>::iterator l_iter = all_local_tables.begin();
	while (l_iter != all_local_tables.end()) {
		cout << "Now In function: " << l_iter->first << endl;
		map<string, node>::iterator ll_iter = l_iter->second.begin();
		while (ll_iter != l_iter->second.end()) {
			printf("%s, %s, %s, %d, %s\n", ll_iter->second.name.c_str(), ll_iter->second.type.c_str(), ll_iter->second.kind.c_str(), ll_iter->second.offset, ll_iter->second.scope.c_str());
			ll_iter++;
		}
		cout << endl;
		l_iter++;
	}

}

string GetIrOp(IR_OPS op)
{
	switch (op) {
		case IR_ADD:   return "ADD";
		case IR_SUB:   return "SUB";
		case IR_DIV:   return "DIV";
		case IR_MUL:   return "MUL";
		case IR_MINUS: return "MINUS";
		case IR_GOTO:  return "GOTO";
		case IR_CASE:  return "CASE";
		case IR_LABEL: return "LABEL";
		case IR_EQ:	   return "EQ";
		case IR_NEQ:   return "NEQ";
		case IR_GT:    return "GT";
		case IR_LT:    return "LT";
		case IR_GET:   return "GET";
		case IR_LET:   return "LET";
		case IR_IN:	   return "INPUT";
		case IR_OUT:   return "OUTPUT";
		case IR_CALL:  return "CALL";
		case IR_PARA:  return "PARA";
		case IR_PUSH:  return "PUSH";
		case IR_FDEF:  return "FDEF"; // 这个应该可以用label代替
		case IR_FEND:  return "FEND";
		case IR_RTNV:  return "RTNV";
		case IR_ASS:   return "ASS";
		case IR_LW:    return "LW";
		case IR_LV0:   return "LV0";
		case IR_SV0:   return "SV0";
		case IR_VAR:   return "VAR";
		case IR_CONST : return "COSNT";
		default: return "ERROR";
	}
}

void PrintMiddleCode()
{
	vector<FourElements>::iterator iter = middle_code.begin();
	while (iter != middle_code.end()) {
		cout << GetIrOp(iter->op)  << " " << iter->r1 << " " << iter->r2 << " " << iter->res  << endl;

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

	PrintTable();
	PrintMiddleCode();

	//FuncDefWithReturn();
	//MainFunction();
	//Statement();
	// 输出到文件
	Output2File();
	// 关闭文件
	CloseFiles();
	return 0;
}