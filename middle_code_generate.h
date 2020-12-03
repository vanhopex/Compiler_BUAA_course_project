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
	IR_FDEF,
	IR_RTNV,
	IR_ASS, // = 

};

// 中间代码是四元式
struct ForElements {
	/*string op = "";*/
	IR_OPS op = IR_NULL;
	string r1 = "";
	string r2 = "";
	string res = "";
	ForElements() :op(), r1(), r2(), res() {}
	/*ForElements(string _op, string _res) : op(_op), r1(), r2(), res(_res) {}
	ForElements(string _op, string _r1, string _res) : op(_op), r1(_r1), r2(), res(_res) {}*/
	ForElements(IR_OPS _op, string _r1, string _r2, string _res) : op(_op), r1(_r1), r2(_r2), res(_res) {}
};
string GenerateLabel();
string  GenerateMidVar();
void Save2IR(ForElements s);

extern vector<ForElements> middle_code;
#endif

