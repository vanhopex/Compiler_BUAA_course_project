#include "public.h"
using namespace std;
char text[10000]; // save the source code read from the testfile.txt
char token[1000]; // global, save the current string
char symbol[10];
char tmpChar;
int totalLen; // the length of the text
int cur = 0; // global pointer
int slength = 0;
//文件相关
FILE* fp, * out;
int MAX_LINE = 100;
char buffer[100];
//work3
string sym;
string word;
struct Item s[1000]; //存词法分析的结果
int symcur = 0; //
struct Item grammer[10000];// 存语法分析的结果,包括汉字
int grammerl = 0; // grammer数组的长度
