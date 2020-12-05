#ifndef __ERRORHANDLING_H_
#define __ERRORHANDLING_H_
#include "public.h"
using namespace std;
void Error(char c);
string GetType(string tmpname);
bool isRedefined(string tmpname);
bool IsDefined(string tmpname);
// 这是先判断完这个标识符存在,然后再找的它的kind
string GetIdentifierKind(string tmpname);
string GetFuncKind(string tmpname);
string GetFuncKindInLocal();
vector<string> GetParKinds(string tmpname);
int Str2Int(string tmpstr);
string toLower(string tmpstring);
#endif