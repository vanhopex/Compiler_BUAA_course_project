#ifndef __MIPS_CODE_GENERATE_
#define __MIPS_CODE_GENERATE_
#include "public.h"

using namespace std;
enum MIPS_OP {
	M_NULL,
	M_ENTER, // 每次执行完一条中间代码输出一个换行符
	M_ADD,
	M_SUB,
	M_DIV,
	M_MUL,
	M_LI,
	M_LA,
	M_SW,
	M_LW,
	M_SYSCALL,
	M_ADDI,
	M_SUBI,
	M_LABEL,
	M_J,
	M_JAR,
	M_JR,
	M_MFLO,
	M_MOVE,
	M_JAL,
	M_BEQ,
	M_NEQ,
    M_BGTZ,
	M_BGEZ,
	M_BLTZ,
	M_BLEZ,
	M_SLL,
	M_SRL,
};

// 目标代码分为三个部分，数据段、字符串(放在最后)、代码段
struct TextElement {
	MIPS_OP op = M_NULL;
	string r1 = "";
	string r2 = "";
	string r3 = "";
	int num = 0;
	TextElement() : op(), r1(), r2(), r3(), num() {}
	TextElement(MIPS_OP _op, string _rs, string _rt, string _rd, int _num) : op(_op), r1(_rs), r2(_rt), r3(_rd), num(_num) {}

};

struct DataElement {
	string name = "";
	string type = ""; // .word .space
	int space = 0;
	DataElement() : name(), type(), space() {}
	DataElement(string _name, string _type, int _space) : name(_name), type(_type), space(_space) {}
};


struct AsciiElement {
	string name = "";
	string type = "";
	string content = "";
	AsciiElement() : name(), type(".ascii"), content() {}
	AsciiElement(string _name, string _content) : name(_name), type(".ascii"), content(_content.append("\\0")) {}
};


#endif
