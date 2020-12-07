// 查符号表，主要是用来查询函数内变量偏移
#include "public.h"
#include "work2.h"
#include "work3.h"
#include "ErrorHandling.h"
#include "query_from_symtable.h"
using namespace std;


// 
/*
* 我用GA表示GrammerAnalysis
* 下面这个后面带GA的就是在语法分析的时候查阅符号表所用的函数
* 
* 然后根据全局变量isInFuncDef来判断是查 globalTable 还是 localTable
* 
* ///////////////////////注意变量名不区分大小写， 调用 Error Handling 里面的toLower
* 
* 

*/

// 根据函数名和变量名判断变量是不是在局部符号表（主要是用来判断中间变量的）
bool IsInLocalTable_GA(string var_name) {
	string tmpname = toLower(var_name);

	map<string, node>::iterator iter;

	iter = localTable.begin();
	while (iter != localTable.end()) {

		if (toLower(iter->first) == tmpname) return true;

		iter++;
	}

	return false;
}

// 得到数组变量的两个维度信息 / 局部函数会引用全局变量的数组，所以局部符号表查不到，就要查全局符号表
pair<int, int> GetDemensions_GA(string arr_name) {
	
	string tmpname = toLower(arr_name);
	map<string, node>::iterator iter;

	if (isInFuncDef) {
		// 查局部符号表
		iter = localTable.begin();
		while (iter != localTable.end()) {
			if (toLower(iter->first) == tmpname) return make_pair(iter->second.d1, iter->second.d2);
			iter++;
		}
		// 查全局符号表
		iter = globalTable.begin();
		while (iter != globalTable.end()) {
			if (toLower(iter->first) == tmpname) return make_pair(iter->second.d1, iter->second.d2);
			iter++;
		}
	}
	else {
		cout << "GetDemensions_GA Error!!!" << endl << endl;
	}

	return make_pair(-1,-1);
}


// 给出常量/变量/数组/参数的名字，在符号表中找它的初始偏移
int GetBaseOffset_GA(string var_name)
{
	// 全局变量需要定义偏移吗？不需要，全局变量只需要定义所需的空间


	// 下面直接在局部符号表中查找

	if (isInFuncDef) {
		string tmpname = toLower(var_name);
		map<string, node>::iterator iter;

		iter = localTable.begin();
		while (iter != localTable.end()) {

			if (toLower(iter->first) == tmpname) return iter->second.offset;

			iter++;
		}
		cout << "GetBaseOffset_GA error" << endl;
	}
	else {
		cout << "GetBaseOffset_GA error" << endl;
	}

	return 0;
}


/*
* 下面这些要在语法分析完了之后才能用!!!
*  
*/

// 根据函数名和变量名判断变量是不是在局部符号表（主要是用来判断中间变量的）
bool IsInLocalTable(string var_name, string func_name) {

	map<string, map<string, node>>::iterator iter = all_local_tables.find(func_name);
	if (iter == all_local_tables.end()) {
		//cout << "can not find function name" << endl;
		return false;
	}
	map<string, node>::iterator liter = iter->second.find(var_name);

	return (liter != iter->second.end());
}

// 得到数组变量的两个维度信息
pair<int, int> GetDemensions(string arr_name, string func_name) {
	// 先在局部符号表找，没有再从全局符号表找，找到先看看是不是数组变量

	// 查找局部符号表


	// 查找全局符号表
	return make_pair(1, 1);
}


// 给出常量/变量/数组/参数的名字，在符号表中找它的初始偏移
int GetBaseOffset(string var_name, string func_name)
{
	// 全局符号表
	if (func_name == "global") {
		map<string, node>::iterator iter = globalTable.find(var_name);
		if (iter == globalTable.end()) {
			cout << "can not find the var_name in global_table check your program!!!" << endl;
		}
		else {
			return iter->second.offset;
		}
	}
	// 局部符号表
	else {
		map<string, map<string, node>>::iterator all_iter = all_local_tables.find(func_name);
		if (all_iter == all_local_tables.end()) {
			cout << "can not find the function name " << func_name <<  " in all_local_tables !!! check you program!" << endl;
		}
		map<string, node>::iterator local_iter = all_iter->second.find(var_name);
		if (local_iter == all_iter->second.end()) {
			cout << "can not find the var_name name " << var_name << "in local_tables !!! check you program!" << endl;
		}
		else {
			return local_iter->second.offset;
		}

	}


	return 0;
}