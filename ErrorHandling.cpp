#include "public.h"
#include "work2.h"
#include "work3.h"
#include "ErrorHandling.h"
void Error(char c)
{
	int ans = gline;
	//if (s[symcur].isfirst) ans 
	if (c == 'k' && s[symcur-1].isfirst) ans--;

	cout << ans << " " << c << endl;
	//fprintf(errorfile, "%d %c\n", ans, c);
}
string toLower(string tmpstring)
{
	string s = tmpstring;

	transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}
bool isRedefined(string tmpname)
{
	// 都得转化成小写比较
	tmpname = toLower(tmpname);

	map<string, node>::iterator iter;
	if (isInFuncDef) {
		/*iter = localTable.find(tmpname);
		if (iter != localTable.end()) return true;*/
		iter = localTable.begin();
		while (iter != localTable.end()) {

			if (toLower(iter->first) == tmpname) return true;

			iter++;
		}
	}
	else {

		iter = globalTable.begin();
		while (iter != globalTable.end()) {

			if (toLower(iter->first) == tmpname) return true;

			iter++;
		}

		/*iter = globalTable.find(tmpname);
		if (iter != globalTable.end())return true;*/
	}
	return false;
}
string GetType(string tmpname)
{
	map<string, node>::iterator iter; 

	if (isInFuncDef) {
		// 先找局部作用域
		iter = localTable.find(tmpname);
		if (iter != localTable.end()) {
			return iter->second.type; // 直接返回
		}
		// 名字有可能在参数列表
		iter = localTable.begin();
		while (iter != localTable.end()) {
			if (iter->second.type == "func") break;

			iter++;
		}
		if (iter != localTable.end()) {
			vector<string> paraNameList = iter->second.paraname;
			for (int i = 0; i < paraNameList.size(); ++i) {
				if (toLower(paraNameList[i]) == tmpname) return "var";// 参数列表里面的都是变量var不是const
			}
		}
		// 再找全局作用域
		iter = globalTable.find(tmpname); /////////////////// 可能都找不到！！！！
		if (iter != globalTable.end()) {
			return iter->second.type;
		}
	}
	else {
		// 直接找全局作用域
		iter = globalTable.find(tmpname);  /////////////////// 可能都找不到！！！！ 
		if (iter != globalTable.end()) {
			return iter->second.type;
		}

	}
	// /////////////////// 可能都找不到！！！！想想为什么？？？？？
	return "var";
}

bool IsDefined(string tmpname) 
{
	map<string, node>::iterator iter;
	// 如果现在在局部函数里面
	if (isInFuncDef) {
		// 先在局部函数找
		iter = localTable.find(tmpname);
		if (iter != localTable.end()) return true;

		// 名字有可能在参数列表
		iter = localTable.begin();
		while (iter != localTable.end()) {
			if (iter->second.type == "func") break;

			iter++;
		}
		if (iter != localTable.end()) {
			vector<string> paraNameList = iter->second.paraname;
			for (int i = 0; i < paraNameList.size(); ++i) {
				if (toLower(paraNameList[i]) == tmpname) return true;
			}
		}
		else error();
		//  再在全局变量里面找
		iter = globalTable.find(tmpname);
		if (iter != globalTable.end())return true;
	}
	// 在main里面
	else {
		iter = globalTable.find(tmpname);
		if (iter != globalTable.end())return true;
	}
	// 没找到
	return false;
}

// 这是先判断完这个标识符存在,然后再找的它的kind
string GetIdentifierKind(string tmpname)
{
	map<string, node>::iterator iter;
	
	if(isInFuncDef) {
		// 先找局部作用域
		iter = localTable.find(tmpname);
		if (iter != localTable.end()) {
			return iter->second.kind; // 直接返回
		}

		// 名字有可能在参数列表
		iter = localTable.begin();
		while (iter != localTable.end()) {
			if (iter->second.type == "func") break;

			iter++;
		}
		if (iter != localTable.end()) {
			vector<string> paraNameList = iter->second.paraname;
			for (int i = 0; i < paraNameList.size(); ++i) {
				if (toLower(paraNameList[i]) == tmpname) return paraNameList[i];
			}
		}
		else error();
		// 再找全局作用域
		iter = globalTable.find(tmpname);
		if (iter != globalTable.end()) {
			return iter->second.kind;
		}
	}
	else {
		// 直接找全局作用域
		iter = globalTable.find(tmpname);
		if (iter != globalTable.end()) {
			return iter->second.kind;
		}
	}
	return NULL;
}

string GetFuncKind(string tmpname)
{
	string ans;
	map<string, node>::iterator iter;
	iter = globalTable.find(tmpname);
	if (iter != globalTable.end()) {
		ans = iter->second.kind;
	}
	return ans;
}

string GetFuncKindInLocal()
{
	string ans;
	map<string, node>::iterator iter;
	iter = localTable.begin();
	while (iter != localTable.end()) {
		if (iter->second.type == "func") {
			ans =  iter->second.kind;
		}
		iter++;
	}
	return ans;
}

vector<string> GetParKinds(string tmpname)
{
	vector<string> ans;
	map<string, node>::iterator iter;
	iter = globalTable.find(tmpname);
	if (iter != globalTable.end()) {
		ans = iter->second.parakind;
	}
	return ans;
}

 // 字符串转数字
int Str2Int(string tmpstr)
{
	int i = 0;
	int sum = 0;
	int sign = 1;

	if (tmpstr.at(0) == '+') {
		sign = 1;
		i++;
	}
	else if (tmpstr.at(0) == '-') {
		sign = -1;
		i++;
	}

	
	for (i; i < tmpstr.size(); i++) {
		sum = sum * 10 + tmpstr[i] - '0';
	}
	return sum*sign;
}