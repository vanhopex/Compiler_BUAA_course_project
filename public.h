#ifndef __PUBLIC_H
#define __PUBLIC_H
#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <string>
#include <map>
#define MAXLE 10000
extern char text[10000]; // save the source code read from the testfile.txt
extern char token[1000]; // global, save the current string
extern char symbol[10];
extern char tmpChar;
extern int totalLen; // the length of the text
extern int cur; // global pointer
extern int slength;
//
extern FILE* fp, * out;
extern int MAX_LINE;
extern char buffer[100];
extern bool isFirstInLine;
// work3
struct Item {
	std::string symbolx;
	std::string wordx; // 词法分析的结果

	std::string grammerx; // 语法分析结果
	int line;
	bool isfirst = false;
};
extern struct Item s[1000];
extern std::string sym;
extern std::string word;
extern int symcur;
extern struct Item grammer[10000];
extern int grammerl;
extern std::map<std::string, bool> defType;
// 错误处理
struct SymbolTable {
	std::string name;
	std::string type;
	std::string level;
};
extern struct SymbolTable symtable[10000];
// 记录行数
extern int gline;
#endif
