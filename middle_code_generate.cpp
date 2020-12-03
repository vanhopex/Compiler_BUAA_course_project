#include "public.h"
#include "work2.h"
#include "work3.h"
#include "ErrorHandling.h"
#include "middle_code_generate.h"
using namespace std;


// 所有中间代码存在这里
vector<ForElements> middle_code; 



void Save2IR(ForElements s)
{
	middle_code.push_back(s);
}
// 全局标签数
int label_number = 1;
//全局产生新的标签
string GenerateLabel()
{
	return "label_" + to_string(label_number++);
}
// 产生局部中间变量，在退出表达式的时候将mid_var_num置0
int mid_var_num = 0;
string  GenerateMidVar()
{
	return "t_" + to_string(mid_var_num++);
}

// 将语法分析的重载都写到这里来，结构清晰一点

// 函数内/ 变量常量参数偏移设计

