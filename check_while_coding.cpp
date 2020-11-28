#include "public.h"
#include "work2.h"
#include "work3.h"
#include "ErrorHandling.h"
using namespace std;

void PrintTable()
{
	// 全局符号表的所有信息
	cout << "In Global Table" << endl;
	map<string, node>::iterator g_iter = globalTable.begin();
	while (g_iter != globalTable.end()) {
		printf("%s, %s, %s, %d\n", g_iter->second.name.c_str(), g_iter->second.type.c_str(), g_iter->second.kind.c_str(), g_iter->second.offset);
		g_iter++;
	}

	// 所有局部符号表， 和其内部的符号信息
	cout << "\nLocal Table \n" << endl;
	map<string, map<string, node>>::iterator l_iter = all_local_tables.begin();
	while (l_iter != all_local_tables.end()) {
		cout << "Now In function: " << l_iter->first << endl;
		map<string, node>::iterator ll_iter = l_iter->second.begin();
		while (ll_iter != l_iter->second.end()) {
			printf("%s, %s, %s, %d\n", ll_iter->second.name.c_str(), ll_iter->second.type.c_str(), ll_iter->second.kind.c_str(), ll_iter->second.offset);
			ll_iter++;
			
		}
		cout << endl;
		l_iter++;
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
	PrintTable();

	//FuncDefWithReturn();
	//MainFunction();
	//Statement();
	// 输出到文件
	Output2File();
	// 关闭文件
	CloseFiles();
	return 0;
}