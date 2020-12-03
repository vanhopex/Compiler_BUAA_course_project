#include "public.h"
#include "work2.h"
#include "work3.h"
#include "ErrorHandling.h"
#include "middle_code_generate.h"
using namespace std;

// 目标代码分为三个部分，数据段、字符串(放在最后)、代码段

// mips_data    全局变量/常量
// mips_assicz  字符串
// mips_text    代码段 
vector<string> mips_text;
vector<string> mips_data;
vector<string> mips_assicz;

void Save2Text(string s)
{
	mips_text.push_back(s + "\n");
}

void Save2Data(string s)
{
	mips_data.push_back(s + "\n");
}

void Save2Assicz(string s)
{
	mips_assicz.push_back(s + "\n");
}


void GenerateData()
{

}
void GenerateText()
{
	// 使用switch选择op 调用对应的函数
	IR_OPS op = IR_NULL;

	switch (op) {
	case IR_ADD:
		//AddIns();
		break;
	case IR_SUB:
		break;

	// ... 



	default:
		break;
	}
}

void GenerateAssicz()
{

}

void GenerateMipsCode()
{
	// 
}