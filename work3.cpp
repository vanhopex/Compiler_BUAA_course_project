#include "public.h"
#include "work2.h"
#include "work3.h"
using namespace std;

// 查看下一个单词和类别码
void NextSym()
{
	sym = s[symcur].symbolx;
	word = s[symcur].wordx;
	if (symcur < slength) {
		symcur++;
	}
}
// 保存词法分析
void SaveLex()
{
	grammer[grammerl].symbolx = sym;
	grammer[grammerl].wordx = word;
	grammer[grammerl].grammerx = "";
	grammerl++;
}

// 保存语法分析结果
void SaveGrammer(string s)
{
	grammer[grammerl].grammerx = s;
	grammer[grammerl].symbolx = "";
	grammer[grammerl].wordx = "";
	grammerl++;
}


void Integer()
{
	if (sym == "PLUS" || sym == "MINU") { 
		// 保存PLUS or MINU
		SaveLex(); 
		// 无符号整数
		NextSym(); 
	}
	SaveLex(); // 保存无符号整数

	SaveGrammer("<无符号整数>");
	SaveGrammer("<整数>");

	NextSym();
}

// 常量定义
void ConstantDefinition()
{
	// int
	if (sym == "INTTK") {
		SaveLex();

		NextSym(); // 标识符,
		SaveLex();
				   //这里都是默认正确的
		// 如果留一个判断错误的接口的话，应该判断每一个字符是不是应该是它

		
		NextSym(); // =
		SaveLex(); // 保存 =

		//整数
		NextSym();  // 只是为了查看下一个sym，不用保存
		Integer(); // 整数调用

		// 如果是整数调用完下一个是一个逗号
		while (sym == "COMMA") {
			SaveLex(); // 先保存这个COMMA

			//下面是新一轮的上面循环//
			// 标识符,
			NextSym(); 
			SaveLex();
			// =
			NextSym(); 
			SaveLex(); 
			//整数
			NextSym(); 
			Integer(); 
		}
	}
	// char
	else if (sym == "CHARTK") {
		SaveLex();

		NextSym(); // 标识符
		SaveLex();

		NextSym(); // =
		SaveLex();

		NextSym(); // 字符
		SaveLex();

		NextSym(); // 下一个

		while (sym == "COMMA") {
			SaveLex();

			NextSym(); // 标识符
			SaveLex();

			NextSym(); // =
			SaveLex();

			NextSym(); // 字符
			SaveLex();

			NextSym(); // 下一个
		}
	}
	else {
		error();
	}

	SaveGrammer("<常量定义>"); 
	// 上面两个while已经调用了一次 NextSym()
}


// 常量说明
void ConstantExplanation()
{
	// const
	if (sym != "CONSTTK") error();
	//SaveLex();

	// ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
	while (sym == "CONSTTK") {
		SaveLex();
		// 常量定义
		NextSym();
		ConstantDefinition();
		// ;
		if (sym != "SEMICN") error();
		SaveLex();

		NextSym();
		//else {
		//	while (sym == "SEMICN") {
		//		SaveLex();
		//		// 常量定义
		//		NextSym();
		//		//ConstantDefinition();
		//	}
		//}
	}
	SaveGrammer("<常量说明>");
}

void Output2File()
{
	for (int i = 0; i < grammerl; i++) {
		if (grammer[i].grammerx == "") {
			fprintf(out, "%s %s\n", grammer[i].symbolx.c_str(), grammer[i].wordx.c_str());
		}
		else {
			fprintf(out, "%s\n", grammer[i].grammerx.c_str());
		}
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
	ConstantExplanation();

	// 输出到文件
	Output2File();
	// 关闭文件
	CloseFiles();
	return 0;
}

//void out2file() // 将当前的
//{
//	fprintf(out, "%s %s\n", sym.c_str(), word.c_str());
//}
//
//void nextsymNoOut()
//{
//	sym = s[symcur].symbolx;
//	word = s[symcur].wordx;
//	//out2file();
//	//fprintf(out, "%s %s\n", sym.c_str(), word.c_str());
//	if (symcur < slength) {
//		symcur++;
//	}
//}
//void nextsym()
//{
//	sym = s[symcur].symbolx;
//	word = s[symcur].wordx;
//	//out2file();
//	////fprintf(out, "%s %s\n", sym.c_str(), word.c_str());
//	if (symcur < slength) {
//		symcur++;
//	}
//
//}
//
//// 将当前的 类别码和单词名称都输出到文件中。
//
//
//void Integer()
//{
//	if (sym == "PLUS" || sym == "MINU") { // tmpchar..
//		// 上一个是符号
//		nextsym(); // 无符号整数
//	}
//	fprintf(out, "%s\n", "无符号整数");// 无符号整数
//	fprintf(out, "%s\n", "整数");
//	nextsym();
//}
////
//void Character()
//{
//
//}
//
//// 常量定义
//void ConstantDefinition()
//{
//	// int
//	if (sym == "INTTK") {
//		nextsym(); // 标识符
//		nextsym(); // '='
//		nextsym(); // 整数 或者 +-
//		Integer(); // 整数调用
//
//		while (sym == "COMMA") {
//			nextsym(); // 标识符
//			nextsym(); // '='
//			nextsym(); // 整数 或者 +-
//			Integer(); // 整数
//		}
//	}
//	// char
//	else if (sym == "CHARTK") {
//		nextsym(); // 标识符
//		nextsym(); // '='
//		nextsym(); // 字符
//		while (sym == "COMMA") {
//			nextsym(); // 标识符
//			nextsym(); // '='
//			// 字符不用输出文法
//			nextsym(); // 字符
//		}
//	}
//	else {
//		error();
//	}
//	fprintf(out, "%s\n","常量定义");
//	nextsym();
//}
//
//// 常量说明
//void ConstantExplanation()
//{
//	// const
//	if (sym != "CONSTTK") error();
//	//
//	nextsym();
//	// 常量定义
//	ConstantDefinition();
//	// ;
//	if (sym != "SEMICN") error();
//
//	nextsymNoOut();
//
//	if (sym == "CONSTTK") {
//		out2file();
//		ConstantExplanation();
//	}
//	else {
//		out2file();
//		fprintf(out, "%s\n", "常量说明");
//		nextsym();
//	}
//}
//
//void Program()
//{
//	
//}
//