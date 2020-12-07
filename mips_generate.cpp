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
/*
* enum IR_OPS {
	IR_NULL,
	IR_ADD,
	IR_SUB,
	IR_DIV,
	IR_MUL,
	IR_LABEL,
	IR_GOTO,
	IR_CASE,
	IR_MINUS, // 符号 单目运算符
	IR_EQ, //  ==
	IR_NEQ, // !=
	IR_GT,  // >
	IR_LT,  // <
	IR_GET, // >=
	IR_LET,  // <=
	IR_IN,
	IR_OUT,
	IR_PUSH,
	IR_CALL,
	IR_PARA,
	IR_FDEF, // 函数定义开始
	IR_FEND, // 函数定义结束
	IR_RTNV,
	IR_ASS, // = 
	IR_LW,
	IR_SW,
	IR_LV0,  // 取寄存器v0 的值
	IR_SV0,  // 存值到V0
	IR_VAR,
	IR_CONST,
};
*/

void AddIns(FourElements element)
{

}

void SubIns(FourElements element)
{

}

void MulIns(FourElements element)
{

}

void DivIns(FourElements element)
{

}
void AssIns(FourElements element)
{

}

void LabelIns(FourElements element)
{

}

void GotoIns(FourElements element)
{

}
void InIns(FourElements element)
{

}

void CaseIns(FourElements element)
{

}

void MinusIns(FourElements element)
{

}

void EqIns(FourElements element) 
{

}

void NeqIns(FourElements element)
{

}

void GtIns(FourElements element)
{

}
void OutIns(FourElements element)
{

}



void GenerateText()
{
	// 使用switch选择op 调用对应的函数
	IR_OPS op = IR_NULL;
	//vector<FourElements>::iterator iter = middle_code.begin();
	for (int i = 0; i < middle_code.size(); i++) {
		FourElements element = middle_code.at(i);
		op = element.op;

		switch (op) {
			case IR_ADD:
				AddIns(element);
				break;
			case IR_SUB:
				SubIns(element);
				break;
			case IR_MUL:
				MulIns(element);
				break;
			case IR_DIV:
				DivIns(element);
				break;
			case IR_LABEL:
				LabelIns(element);
				break;
			case IR_GOTO:
				GotoIns(element);
				break;
			case IR_CASE:
				CaseIns(element);
				break;
			case IR_MINUS:
				MinusIns(element);
				break;
			case IR_EQ:
				EqIns(element);
				break;
			case IR_NEQ:
				NeqIns(element);
				break;
			case IR_GT:
				GtIns(element);
				break;
			case IR_ASS:
				AssIns(element);
				break;
				// ... 
			case IR_IN:
				InIns(element);
				break;
			case IR_OUT:
				OutIns(element);
				break;

			default: cout << "mips_generate error in op switch!" << endl;
				break;
		}


	}
}

void GenerateAssicz()
{

}

void GenerateMipsCode()
{
	// 
}