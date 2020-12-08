#ifndef __MIDDLE_CODE_GENERATE_
#define __MIDDLE_CODE_GENERATE_
#include "public.h"
using namespace std;
enum IR_OPS {
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
	IR_ASS, //  ass value  offset res
	IR_LW,   // lw var_name offset res
	IR_SW,
	IR_LV0,  // 取寄存器v0 的值
	IR_SV0,  // 存值到V0
	IR_VAR,
	IR_CONST,
};
// 中间代码是四元式
struct FourElements {
	/*string op = "";*/
	IR_OPS op = IR_NULL;
	string r1 = "";
	string r2 = "";
	string res = "";
	string scope = ""; //
	FourElements() :op(), r1(), r2(), res() {}
	/*FourElements(string _op, string _res) : op(_op), r1(), r2(), res(_res) {}
	FourElements(string _op, string _r1, string _res) : op(_op), r1(_r1), r2(), res(_res) {}*/
	FourElements(IR_OPS _op, string _r1, string _r2, string _res) : op(_op), r1(_r1), r2(_r2), res(_res) {}
	// 五元式用来指示变量/常量/位于哪个函数内部/便于生成目标代码的时候差符号表
	FourElements(IR_OPS _op, string _r1, string _r2, string _res, string _scope) : op(_op), r1(_r1), r2(_r2), res(_res), scope(_scope) {}
};
string GenerateLabel();
string  GenerateMidVar();
void SetVarNum0();
void Save2IR(FourElements s);
extern vector<FourElements> middle_code;
#endif

