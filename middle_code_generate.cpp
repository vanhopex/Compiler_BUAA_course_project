#include "public.h"
#include "work2.h"
#include "work3.h"
#include "ErrorHandling.h"
using namespace std;

// 中间代码是四元式
struct Quaternary {
	string op;
	string r1;
	string r2;
	string res;
};
// 所有中间代码存在这里
vector<Quaternary> middle_code; 


// 目标代码分为三个部分，数据段、字符串(放在最后)、代码段

// mips_data    全局变量/常量
// mips_assicz  字符串
// mips_text    代码段 
vector<string> mips_text;

// 全局标签数
int label_number = 1;
//全局产生新的标签
string GenerateLabel()
{
	return "zwh_label_" + to_string(label_number);
}
// 将语法分析的重载都写到这里来，结构清晰一点

// 函数内/ 变量常量参数偏移设计

