#ifndef __QUERY_FROM_SYMTABLE_
#define __QUERY_FROM_SYMTABLE_
#include"public.h"
using namespace std;
//这些是语法分析时候用的
int GetBaseOffset_GA(string var_name);
pair<int, int> GetDemensions_GA(string arr_name);
bool IsInLocalTable_GA(string var_name);
//下面这些要等语法分析完了之后才能用 
bool IsInLocalTable(string var_name, string func_name);
int GetBaseOffset(string var_name, string func_name);
pair<int, int> GetDemensions(string arr_name, string func_name);
#endif