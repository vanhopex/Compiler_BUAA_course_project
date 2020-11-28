// 查符号表，主要是用来查询函数内变量偏移
#include "public.h"
#include "work2.h"
#include "work3.h"
#include "ErrorHandling.h"
using namespace std;

int GetOffset(string func_name, string name)
{
	// func_name 是函数的变量名
	// name 是变量/常量/参数/数组的名字
	// 考虑数组是否需要单独查询

}