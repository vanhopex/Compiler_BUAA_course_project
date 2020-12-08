#include "public.h"
#include "work2.h"
#include "work3.h"
#include "ErrorHandling.h"
#include "middle_code_generate.h"
#include "check_while_coding.h"
#include "query_from_symtable.h"
#include "mips_code_generate.h"
using namespace std;

// mips_data    全局变量/常量
// mips_assicz  字符串
// mips_text    代码段 
vector<TextElement> mips_text;
vector<DataElement> mips_data;
vector<AsciiElement> mips_ascii;

int str_number = 0;
string GenerateStrName()
{
	return "zwh_str" + to_string(str_number++);
}

void Save2Text(TextElement s)
{
	mips_text.push_back(s );
}

void Save2Data(DataElement s)
{
	mips_data.push_back(s);
}

void Save2Assicz(AsciiElement s)
{
	mips_ascii.push_back(s);
}

void SaveValue2Reg(string reg_name, string value_name, string scope)
{
	/* 把r1放到reg_name寄存器中 */

	// 字符
	if (value_name.at(0) == '\'') {
		Save2Text(TextElement(M_ADDI, reg_name, "$0", value_name, 0));
	}
	// 数字
	else if ((value_name.at(0) <= '9' && value_name.at(0) >= '0') || value_name.at(0) == '+' || value_name.at(0) == '-') {
		Save2Text(TextElement(M_LI, reg_name, value_name, "", 0));
	}
	// 变量
	else {
		// 如果是局部变量
		if (IsInLocalTable(value_name, scope)) {
			int base_offset = GetBaseOffsetInSP(value_name, scope);
			//Save2Text(TextElement(M_LI, "$t8", "", "", base_offset));
			Save2Text(TextElement(M_LW, reg_name, "$fp", "", base_offset));
		}
		// 全局变量
		else {
			Save2Text(TextElement(M_LW, reg_name, value_name, "$0", 0));
		}
	}
}

string SaveR1_Reg1(string r1, FourElements element)
{
	/* 把r1放到$t1寄存器中 */

	// 字符
	if (r1.at(0) == '\'') {
		Save2Text(TextElement(M_ADDI, "$t1", "$0", r1, 0));
	}
	// 数字
	else if ((r1.at(0) <= '9' && r1.at(0) >= '0') || r1.at(0) == '+' || r1.at(0) == '-') {
		Save2Text(TextElement(M_LI, "$t1", r1, "", 0));
	}
	// 变量
	else {
		// 如果是局部变量
		if (IsInLocalTable(r1, element.scope)) {
			int base_offset = GetBaseOffsetInSP(r1, element.scope);
			//Save2Text(TextElement(M_LI, "$t4", "", "", base_offset));
			Save2Text(TextElement(M_LW, "$t1", "$fp", "", base_offset));
		}
		// 全局变量
		else {
			Save2Text(TextElement(M_LW, "$t1", r1, "$0", 0));
		}
	}

	return "$t1";
}

string SaveR2_Reg2(string r2, FourElements element)
{
	/* 把r2放到$t2寄存器中 */

	// 字符
	if (r2.at(0) == '\'') {
		Save2Text(TextElement(M_ADDI, "$t2", "$0", r2, 0));
	}
	// 数字
	else if ((r2.at(0) <= '9' && r2.at(0) >= '0') || r2.at(0) == '+' || r2.at(0) == '-') {
		Save2Text(TextElement(M_LI, "$t2", r2, "", 0));
	}
	// 变量
	else {
		// 如果是局部变量
		if (IsInLocalTable(r2, element.scope)) {
			int base_offset = GetBaseOffsetInSP(r2, element.scope);
			//Save2Text(TextElement(M_LI, "$t4", "", "", base_offset));
			Save2Text(TextElement(M_LW, "$t2", "$fp", "", base_offset));
		}
		// 全局变量
		else {
			Save2Text(TextElement(M_LW, "$t2", r2, "$0", 0));
		}
	}
	return "$t2";

}

void AddIns(FourElements element)
{
	string r1 = element.r1;
	string r2 = element.r2;
	string res = element.res;
	string reg1, reg2;
	/* 把r1放到$t1寄存器中 */
	reg1 = SaveR1_Reg1(r1, element);
	/* 把r2放到$t2寄存器中 */
	reg2 = SaveR2_Reg2(r2, element);

	Save2Text(TextElement(M_ADD, "$t3", reg1, reg2, 0));

	// 看要赋值的变量是全局还是局部

// 局部变量
	if (IsInLocalTable(res, element.scope)) {
		int base_offset = GetBaseOffsetInSP(res, element.scope);
		//Save2Text(TextElement(M_LI, "$t4", "","", base_offset));
		Save2Text(TextElement(M_SW, "$t3", "$fp", "", base_offset));
	}
	// 全局变量
	else {
		Save2Text(TextElement(M_SW, "$t3", res, "$0", 0));
	}
}

void SubIns(FourElements element)
{
	string r1 = element.r1;
	string r2 = element.r2;
	string res = element.res;
	string reg1, reg2;
	/* 把r1放到$t1寄存器中 */
	reg1 = SaveR1_Reg1(r1, element);
	/* 把r2放到$t2寄存器中 */
	reg2 = SaveR2_Reg2(r2, element);

	Save2Text(TextElement(M_SUB, "$t3", reg1, reg2, 0));

	// 看要赋值的变量是全局还是局部

// 局部变量
	if (IsInLocalTable(res, element.scope)) {
		int base_offset = GetBaseOffsetInSP(res, element.scope);
		//Save2Text(TextElement(M_LI, "$t4", "", "", base_offset));
		Save2Text(TextElement(M_SW, "$t3", "$fp", "", base_offset));
	}
	// 全局变量
	else {
		Save2Text(TextElement(M_SW, "$t3", res, "$0", 0));
	}

}

void DivIns(FourElements element)
{
	string r1 = element.r1;
	string r2 = element.r2;
	string res = element.res;
	string reg1, reg2;
	/* 把r1放到$t1寄存器中 */
	reg1 = SaveR1_Reg1(r1, element);
	/* 把r2放到$t2寄存器中 */
	reg2 = SaveR2_Reg2(r2, element);

	Save2Text(TextElement(M_DIV, reg1, reg2, "", 0));
	Save2Text(TextElement(M_MFLO, "$t3", "", "", 0));
	/*把$t1存进res*/

	// 看要赋值的变量是全局还是局部

	// 局部变量
	if (IsInLocalTable(res, element.scope)) {
		int base_offset = GetBaseOffsetInSP(res, element.scope);
		//Save2Text(TextElement(M_LI, "$t4", "", "", base_offset));
		Save2Text(TextElement(M_SW, "$t3", "$fp", "", base_offset));
	}
	// 全局变量
	else {
		Save2Text(TextElement(M_SW, "$t3", res, "$0", 0));
	}

}

void MulIns(FourElements element)
{
	string r1 = element.r1;
	string r2 = element.r2;
	string res = element.res;
	string reg1, reg2;
	/* 把r1放到$t1寄存器中 */
	reg1 = SaveR1_Reg1(r1, element);
	/* 把r2放到$t2寄存器中 */
	reg2 = SaveR2_Reg2(r2, element);

	Save2Text(TextElement(M_MUL, reg1, reg2, "", 0));
	Save2Text(TextElement(M_MFLO, "$t3", "", "", 0));
	/*把$t1存进res*/

	// 看要赋值的变量是全局还是局部

	// 局部变量
	if (IsInLocalTable(res, element.scope)) {
		int base_offset = GetBaseOffsetInSP(res, element.scope);
		//Save2Text(TextElement(M_LI, "$t4", "", "", base_offset));
		Save2Text(TextElement(M_SW, "$t3", "$fp", "", base_offset));
	}
	// 全局变量
	else {
		Save2Text(TextElement(M_SW, "$t3", res, "$0", 0));
	}


}

void LabelIns(FourElements element)
{
	Save2Text(TextElement(M_LABEL, element.res,"","",0));
	
}

void GotoIns(FourElements element)
{
	Save2Text(TextElement(M_J, element.res, "", "", 0));
	
}


void CaseIns(FourElements element)
{
	// case var1 var2 label
	// if var1 == var2 goto label
	string r1 = element.r1;
	string r2 = element.r2;
	string label = element.res;
	string scope = element.scope;

	string reg1 = "$t1";
	string reg2 = "$t2";
	SaveValue2Reg(reg1, r1, scope);
	SaveValue2Reg(reg2, r2, scope);

	Save2Text(TextElement(M_BEQ, reg1, reg2, label, 0));

}

void MinusIns(FourElements element)
{
	string r1 = element.r1;
	string res = element.res;
	string reg1;
	/* 把r1放到$t1寄存器中 */
	reg1 = SaveR1_Reg1(r1, element);

	Save2Text(TextElement(M_SUB, "$t3", "$0", reg1, 0));

	// 看要赋值的变量是全局还是局部

// 局部变量
	if (IsInLocalTable(res, element.scope)) {
		int base_offset = GetBaseOffsetInSP(res, element.scope);
		//Save2Text(TextElement(M_LI, "$t4", "", "", base_offset));
		Save2Text(TextElement(M_SW, "$t3", "$fp", "", base_offset));
	}
	// 全局变量
	else {
		Save2Text(TextElement(M_SW, "$t3", res, "$0", 0));
	}

	//Save2Text(TextElement(M_ENTER, "", "", "", 0));
}

void EqIns(FourElements element) 
{
	string r1 = element.r1;
	string r2 = element.r2;
	string label = element.res;
	string scope = element.scope;

	string reg1 = "$t1";
	string reg2 = "$t2";
	SaveValue2Reg(reg1, r1, scope);
	SaveValue2Reg(reg2, r2, scope);
	Save2Text(TextElement(M_BEQ, reg1, reg2, label, 0));
}

void NeqIns(FourElements element)
{
	string r1 = element.r1;
	string r2 = element.r2;
	string label = element.res;
	string scope = element.scope;

	string reg1 = "$t1";
	string reg2 = "$t2";
	SaveValue2Reg(reg1, r1, scope);
	SaveValue2Reg(reg2, r2, scope);
	Save2Text(TextElement(M_NEQ, reg1, reg2, label, 0));
}

void GtIns(FourElements element)
{
	string r1 = element.r1;
	string r2 = element.r2;
	string label = element.res;
	string scope = element.scope;

	string reg1 = "$t1";
	string reg2 = "$t2";
	string reg3 = "$t3";
	SaveValue2Reg(reg1, r1, scope);
	SaveValue2Reg(reg2, r2, scope);
	Save2Text(TextElement(M_SUB, reg3, reg1, reg2, 0));
	Save2Text(TextElement(M_BGTZ,label, "", "", 0));
}

void LtIns(FourElements element)
{
	string r1 = element.r1;
	string r2 = element.r2;
	string label = element.res;
	string scope = element.scope;

	string reg1 = "$t1";
	string reg2 = "$t2";
	string reg3 = "$t3";
	SaveValue2Reg(reg1, r1, scope);
	SaveValue2Reg(reg2, r2, scope);
	Save2Text(TextElement(M_SUB, reg3, reg1, reg2, 0));
	Save2Text(TextElement(M_BLTZ, label, "", "", 0));
}

void GetIns(FourElements element)
{
	string r1 = element.r1;
	string r2 = element.r2;
	string label = element.res;
	string scope = element.scope;

	string reg1 = "$t1";
	string reg2 = "$t2";
	string reg3 = "$t3";
	SaveValue2Reg(reg1, r1, scope);
	SaveValue2Reg(reg2, r2, scope);
	Save2Text(TextElement(M_SUB, reg3, reg1, reg2, 0));
	Save2Text(TextElement(M_BGEZ, label, "", "", 0));
}

void LetIns(FourElements element)
{
	string r1 = element.r1;
	string r2 = element.r2;
	string label = element.res;
	string scope = element.scope;

	string reg1 = "$t1";
	string reg2 = "$t2";
	string reg3 = "$t3";
	SaveValue2Reg(reg1, r1, scope);
	SaveValue2Reg(reg2, r2, scope);
	Save2Text(TextElement(M_SUB, reg3, reg1, reg2, 0));

	Save2Text(TextElement(M_BLEZ, label, "", "", 0));
}

void InIns(FourElements element)
{
	string var_name = element.res;
	string type = GetKindOfVar(var_name, element.scope);

	// 全局变量读入放到内存中
	if (element.scope == "global") {
		// char
		if (type == "char") {
			Save2Text(TextElement(M_LI, "$v0", "", "", 12));
		}
		// int 
		else {
			Save2Text(TextElement(M_LI, "$v0", "", "", 5));
		}
		Save2Text(TextElement(M_SYSCALL, "", "", "", 0));
		Save2Text(TextElement(M_SW, "$v0", var_name, "$0", 0));
		//Save2Text("sw $v0 0(" + var_name + ")");

	}
	// 局部变量读入放入到栈对应的偏移中
	else {
		// char
		if (type == "char") {
			Save2Text(TextElement(M_LI, "$v0", "", "", 12));
		}
		// int 
		else {
			Save2Text(TextElement(M_LI, "$v0", "", "", 5));
		}
		Save2Text(TextElement(M_SYSCALL, "", "", "", 0));

		// $v0存到栈空间里面

		//Save2Text(TextElement(M_LI, "$t4", "", "", GetBaseOffsetInSP(var_name, element.scope)));
		Save2Text(TextElement(M_SW, "$V0", "$fp", "", GetBaseOffsetInSP(var_name, element.scope)));
	}
	//Save2Text(TextElement(M_ENTER, "", "", "", 0));
}
void OutIns(FourElements element)
{
	string type = element.r1;
	string content = element.res;
	int sys_num = 0;
	string scope = element.scope;
	// 判断输出的类型
	if (type == "string") {
		// 分别生成 text ， ascii 两个指令
		string name = GenerateStrName();

		Save2Assicz(AsciiElement(name, element.res));

		Save2Text(TextElement(M_LA, "$a0", name, "", 0));
		
		sys_num = 4;
	}
	else if (type == "int") {
		string reg = "$a0";

		SaveValue2Reg(reg, content, scope);

		//string reg1 = SaveR1_Reg1(content, element);

		//Save2Text(TextElement(M_ADDI, "$a0", "$t1", "0", 0)); // 或者这里可以直接把寄存器的名字$a0传进去


		sys_num = 1;
	}
	else if (type == "char") {

		string reg = "$a0";

		SaveValue2Reg(reg, content, scope);

//		string reg1 = SaveR1_Reg1(content, element);

//		Save2Text(TextElement(M_ADDI, "$a0", "$t1", "0", 0)); // 或者这里可以直接把寄存器的名字$a0传进去



		sys_num = 11;
	}
	// 输出换行符
	else if (type == "enter") {

		Save2Text(TextElement(M_ADDI, "$a0", "$0", "\'\\n\'", 0));
		sys_num = 11;
	}

	Save2Text(TextElement(M_LI, "$v0", "", "", sys_num));
	Save2Text(TextElement(M_SYSCALL, "", "", "", 0));
	//Save2Text(TextElement(M_ENTER, "", "", "", 0));
}

void PushIns(FourElements element)
{

}

void CallIns(FourElements element)
{
	Save2Text(TextElement(M_JAL,element.res, "", "", 0));
	
}


void ParaIns(FourElements element)
{

}


void FdefIns(FourElements element)
{
	Save2Text(TextElement(M_LABEL, element.res, "", "", 0));
	// 下面是栈空间的分配，符号表中应该记录了整个栈空间的大小啊！！！
}

void FendIns(FourElements element)
{

	// 把栈空间填回去
}

// 无用
void RtnvIns(FourElements element)
{

}

void AssIns(FourElements element)
{

	// Ass value offset var_name

	// value - > var_name + offset;
	string value = element.r1;
	string offset = element.r2;
	string res = element.res;

	string reg1, reg2;

	/* 把value放到$t1寄存器中 */
	reg1 = SaveR1_Reg1(value, element);

	/* 把offset放到$t2寄存器中 */
	reg2 = SaveR2_Reg2(offset, element);

	/* 把value 写入 var_name + offset*/
		// res是局部变量
	if (IsInLocalTable(res, element.scope)) {
		Save2Text(TextElement(M_ADD, "$t2", "$fp", "$t2", 0));
		Save2Text(TextElement(M_SW, "$t1", "$t2", "", 0));
	}
	// res是全局变量
	else {
		Save2Text(TextElement(M_SW, "$t1", res, "$t2", 0));
	}

	//Save2Text(TextElement(M_ENTER, "", "", "", 0));
}

void LwIns(FourElements element)
{
	string var_name = element.r1;
	string offset = element.r2;
	string res = element.res;
	string scope = element.scope;
	//var_name + offset  -->  res

	string reg1 = "$t1";
	string reg2 = "$t2";

// offset 存到reg2
	SaveValue2Reg(reg2, offset, scope);
// (var_name + offset).value 存到reg1
	if (IsInLocalTable(var_name, element.scope)) {
		// 基础偏移
		int base_offset = GetBaseOffsetInSP(var_name, element.scope);
		// 总的偏移存到reg2
		Save2Text(TextElement(M_ADDI,reg2, reg2,to_string(base_offset),0));
		//  把$fp + reg2 的值存到 reg1
		Save2Text(TextElement(M_ADD, reg2, "$fp", reg2, 0));
		Save2Text(TextElement(M_LW, reg1, reg2, "", 0));
	}
	// 全局变量
	else {
		Save2Text(TextElement(M_LW, reg1, var_name, reg2, 0));
	}

/*把reg1值存入到res*/
	if (IsInLocalTable(res, element.scope)) {
		int base_offset = GetBaseOffsetInSP(res, element.scope);
		Save2Text(TextElement(M_SW, "$t1", "$fp", "", base_offset));
	}
	// 全局变量
	else {
		Save2Text(TextElement(M_SW, "$t1", res, "$0", 0));
	}

	Save2Text(TextElement(M_ENTER, "", "", "", 0));
}

void Lv0Ins(FourElements element)
{
	string res = element.res;
	// 局部变量
	if (IsInLocalTable(res, element.scope)) {
		int base_offset = GetBaseOffsetInSP(res, element.scope);
		Save2Text(TextElement(M_LW, "$v0", "$fp", "", base_offset));
	}
	// 全局变量
	else {
		Save2Text(TextElement(M_LW, "$v0", res, "$0", 0));
	}
}

void Sv0Ins(FourElements element)
{
	string reg = "$t1";
	SaveValue2Reg(reg, element.r1, element.scope);
	Save2Text(TextElement(M_MOVE, "$v0", reg, "", 0));  // v0 = reg
}

void VarIns(FourElements element)
{
	// 先看这个scope，如果不是global就不用管
	// 是global则加入到data段
	if (element.scope == "global") {
		Save2Data(DataElement(element.res, ".space", GetGlobaleVarSpace(element.res)));
	}
	// 函数内参数/变量/数组，不用管，已经在符号表分配好了空间

}
void ConstIns(FourElements element)
{
	// 先看这个scope，如果不是global就不用管
	// 是global则加入到data段
	if (element.scope == "global") {
		Save2Data(DataElement(element.res, ".space", GetGlobaleVarSpace(element.res)));
	}
	// 函数内参数/变量/数组，不用管，已经在符号表分配好了空间

}
void GenerateMipsCode()
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
			case IR_LT:
				LtIns(element);
				break;
			case IR_GET:
				GetIns(element);
				break;
			case IR_LET:
				LetIns(element);
				break;
			case IR_IN:
				InIns(element);
				break;
			case IR_OUT:
				OutIns(element);
				break;
			case IR_PUSH:
				PushIns(element);
				break;
			case IR_CALL:
				CallIns(element);
				break;
			case IR_PARA:
				ParaIns(element);
				break;
			case IR_FDEF:
				FdefIns(element);
				break;
			case IR_FEND:
				FendIns(element);
				break;
			case IR_RTNV:
				RtnvIns(element);
				break;
			case IR_ASS:
				AssIns(element);
				break;
			case IR_LW:
				LwIns(element);
				break;
			case IR_LV0:
				Lv0Ins(element);
				break;
			case IR_SV0:
				Sv0Ins(element);
				break;
			case IR_VAR:
				VarIns(element);
				break;
			case IR_CONST:
				ConstIns(element);
				break;
			default: cout << "mips_generate error in op switch!" << endl;
				break;
		}
		Save2Text(TextElement(M_ENTER, "", "", "", 0));
	}
}
void OutputMipsCode()
{
	// .data
	printf("\n\n.data\n");
	for (int i = 0; i < mips_data.size(); i++) {
		printf("\t%s: %s %d\n", mips_data[i].name.c_str(), mips_data[i].type.c_str(), mips_data[i].space);
	}

	// .ascci
	for (int i = 0; i < mips_ascii.size(); i++) {
		printf("\t%s: %s \"%s\"\n", mips_ascii[i].name.c_str(), mips_ascii[i].type.c_str(), mips_ascii[i].content.c_str());
	}

	// .text
	printf("\n.text\n");
	vector<TextElement>::iterator iter = mips_text.begin();
	while (iter != mips_text.end()) {
		switch (iter->op) {
			case M_ENTER:
				printf("\n");
				break;
			// li $t1,		,	 ,	num
			// li $t1,	'a'	,	 , 
			case M_LI:
				if (iter->r2 == "") {
					printf("\tli %s %d\n", iter->r1.c_str(), iter->num);
				}
				else {
					printf("\tli %s %s\n", iter->r1.c_str(), iter->r2.c_str());
				}
				
				break;
			// sw $t1, $fp, , offset
			// sw $t1, $t2, , offset
			// sw $t1, var_name, $t2, 0 
			case M_SW:
				if (iter->r2.at(0) == '$') {
					printf("\tsw %s %d(%s)\n", iter->r1.c_str(),iter->num, iter->r2.c_str());
				}
				else {
					printf("\tsw %s %s(%s)\n", iter->r1.c_str(), iter->r2.c_str(), iter->r3.c_str());
				}
				break;
            // lw $t1, $fp, , offset.
			// lw $t1, $t2, , offset.
			// lw $t1, var_name, $t2, 0 .
			case M_LW:
				if (iter->r2.at(0) == '$') {
					printf("\tlw %s %d(%s)\n", iter->r1.c_str(), iter->num, iter->r2.c_str());
				}
				else {
					printf("\tlw %s %s(%s)\n", iter->r1.c_str(), iter->r2.c_str(), iter->r3.c_str());
				}
				break;
			// addi $t1, $t2,  $t3, num.
			case M_ADDI:
				printf("\taddi %s %s %s\n", iter->r1.c_str(), iter->r2.c_str(), iter->r3.c_str());
				break;
			// add $t3,  $t1,  $t2, .
			case M_ADD:
				printf("\tadd %s %s %s\n", iter->r1.c_str(), iter->r2.c_str(), iter->r3.c_str());
				break;
			case M_SUB:
				printf("\tsub %s %s %s\n", iter->r1.c_str(), iter->r2.c_str(), iter->r3.c_str());
				break;
			case M_MUL:
				printf("\tmult %s %s\n", iter->r1.c_str(), iter->r2.c_str());
				break;
			case M_DIV:
				printf("\tdiv %s %s\n", iter->r1.c_str(), iter->r2.c_str());
				break;
			// mflo $t1,   ,   ,   .
			case M_MFLO:
				printf("\tmflo %s\n", iter->r1.c_str());
				break;
			// M_SYCALL,   ,    ,  .
			case M_SYSCALL:
				printf("\tsyscall\n");
				break;
			// M_LABEL , label_x,   ,  , .
			case M_LABEL:
				printf("%s:\n",iter->r1.c_str());
				break;
			// M_LA, $a0,  str_name,  ,   .
			case M_LA:
				printf("\tla %s %s\n", iter->r1.c_str(), iter->r2.c_str());
				break;
			// M_j, label, , , .
			case M_J:
				printf("\tj %s\n", iter->r1.c_str());
				break;
			// M_MOVE, $t1, $t2, , . t1 = t2
			case M_MOVE:
				printf("\tmove %s %s\n", iter->r1.c_str(), iter->r2.c_str());
				break;
			case M_JAL:
				printf("\tjal %s\n", iter->r1.c_str());
				break;
			case M_BEQ:
				printf("\tbeq %s %s %s\n", iter->r1.c_str(), iter->r2.c_str(), iter->r3.c_str());
				break;
			case M_NEQ:
				printf("\tneq %s %s %s\n", iter->r1.c_str(), iter->r2.c_str(), iter->r3.c_str());
				break;
			case M_BGTZ:
				printf("\tbgtz %s %s \n", iter->r1.c_str(), iter->r2.c_str());
				break;
			case M_BGEZ:
				printf("\tbgez %s %s \n", iter->r1.c_str(), iter->r2.c_str());
				break;
			case M_BLTZ:
				printf("\tbltz %s %s \n", iter->r1.c_str(), iter->r2.c_str());
				break;
			case M_BLEZ:
				printf("\tblez %s %s \n", iter->r1.c_str(), iter->r2.c_str());
				break;

			default:
				printf("output_mips error! %d\n", iter->op);
		}
		//printf("\n");
		iter++;
	}

}

int main()
{
	// 读文件
	ReadFiles();
	// 执行词法分析
	work2();
	// 执行语法分析
	NextSym();
	Program();
	// 符号表
	PrintTable();

	//中间代码
	PrintMiddleCode();

	// 生成mips代码
	GenerateMipsCode();
	// 输出mips代码
	OutputMipsCode();
	// 输出到文件
	Output2File();
	// 关闭文件
	CloseFiles();
	return 0;
}