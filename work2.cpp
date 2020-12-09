#include "public.h"
#include "work2.h"
#include "ErrorHandling.h"
using namespace std;

void clearToken()
{
	memset(token, 0, sizeof(token));
}
bool isSpace()
{
	//return (tmpChar == ' ' || tmpChar == '\t' || tmpChar == '\n'); 大BUG！！这里放到Linux环境不行！！！要用库函数
	return isspace(tmpChar);
}
bool isDigit()
{
	return (tmpChar >= '0' && tmpChar <= '9');
}

bool isPlus()
{
	return (tmpChar == '+');
}

bool isDiv()
{
	return (tmpChar == '/');
}

bool isMult()
{
	return (tmpChar == '*');
}

bool isLess()
{
	return (tmpChar == '<');
}

bool isGreater()
{
	return (tmpChar == '>');
}

bool isEqu()
{
	return (tmpChar == '=');
}
bool isMinu()
{
	return (tmpChar == '-');
}

bool isExcla()
{
	return (tmpChar == '!');
}

bool isComma()
{
	return (tmpChar == ',');
}

bool isSemicn()
{
	return (tmpChar == ';');
}

bool isLparent()
{
	return (tmpChar == '(');
}

bool isRparent()
{
	return (tmpChar == ')');
}

bool isLbrack()
{
	return (tmpChar == '[');
}

bool isRbrack()
{
	return (tmpChar == ']');
}

bool isLbrace()
{
	return (tmpChar == '{');
}

bool isRbrace()
{
	return (tmpChar == '}');
}


bool isLetter()
{
	return ((tmpChar >= 'A' && tmpChar <= 'Z') || 
			(tmpChar >= 'a' && tmpChar <= 'z'));
}

bool isColon()
{
	return (tmpChar == ':');
}

bool isSinQuo()
{
	return (tmpChar == '\'');
}
bool isDouQuo()
{
	return (tmpChar == '\"');
}
bool isUndLine()
{
	return (tmpChar == '_');
}
//

void getChar()
{
	tmpChar = text[cur++];
}
void retract() // mainly for token word;
{
	if (cur != 0)
		tmpChar = text[--cur];
}
void catToken()
{
	token[strlen(token)] = tmpChar;
}


void reserver()
{
	char t[1000]; ///////////////////////////////////////这里数组一开始开小了，导致栈溢出，才出现了runtime  error
	memset(t, 0, sizeof(t));
	for (int i = 0; i < strlen(token); i++) {
		if (token[i] <= 'Z' && token[i] >= 'A')
			t[i] = token[i] + 32;
		else 
			t[i] = token[i]; 
	}

	if (strcmp(t, "const") == 0) {
		strcpy(symbol, "CONSTTK");
	}
	else if (strcmp(t, "int") == 0) {
		strcpy(symbol, "INTTK");
	}
	else if (strcmp(t, "char") == 0) {
		strcpy(symbol, "CHARTK");
	}
	else if (strcmp(t, "void") == 0) {
		strcpy(symbol, "VOIDTK");
	}
	else if (strcmp(t, "main") == 0) {
		strcpy(symbol, "MAINTK");
	}
	else if (strcmp(t, "if") == 0) {
		strcpy(symbol, "IFTK");
	}
	else if (strcmp(t, "else") == 0) {
		strcpy(symbol, "ELSETK");
	}
	else if (strcmp(t, "switch") == 0) {
		strcpy(symbol, "SWITCHTK");
	}
	else if (strcmp(t, "case") == 0) {
		strcpy(symbol, "CASETK");
	}
	else if (strcmp(t, "default") == 0) {
		strcpy(symbol, "DEFAULTTK");
	}
	else if (strcmp(t, "while") == 0) {
		strcpy(symbol, "WHILETK");
	}
	else if (strcmp(t, "for") == 0) {
		strcpy(symbol, "FORTK");
	}
	else if (strcmp(t, "scanf") == 0) {
		strcpy(symbol, "SCANFTK");
	}
	else if (strcmp(t, "printf") == 0) {
		strcpy(symbol, "PRINTFTK");
	}
	else if (strcmp(t, "return") == 0) {
		strcpy(symbol, "RETURNTK");
	}
	else {
		strcpy(symbol, "IDENFR");
	}
}

void getsym()
{
	clearToken();
	memset(symbol, 0, sizeof(symbol));
	
	getChar();
	
	while (isSpace()) { // if the tmpchar is " ", "\t", "\n" skip it;
		if (tmpChar == '\n') {
			gline++;
			// 说明这个symbol是一行开始的symbol，如果缺失的话当前sym已经到下一行了，所以要特殊处理
			isFirstInLine = true;
		}
		getChar();
	}
	 
// 此处已经有一个非空的字符了
	if (isLetter() || isUndLine()) { // if the tmpChar is a letter
		
		do{
			catToken();
			getChar();
			
		} while(isLetter() || isDigit() || isUndLine());
		retract();
		reserver();
	}
	
	else if (isDigit()) {
		
		do {
			catToken();
			getChar();
		}while(isDigit());
		
		retract();

		strcpy(symbol, "INTCON");
	}
	
	else if (isColon()) {
		catToken();
		strcpy(symbol, "COLON");
	}

	else if (isEqu()) {

		catToken();
		getChar();
		
		if (isEqu()) {
			catToken();
			strcpy(symbol, "EQL");
		}
		else {
			strcpy(symbol, "ASSIGN");
			retract(); // error 2: 这里要回退一个字符 
		}	
	}
	
	else if (isLess()) {
		catToken(); 
		 
		getChar(); // 再读入一个字符 
		if (isEqu()) { // <=
			catToken();
			strcpy(symbol, "LEQ"); 
		}
		else {
			retract(); // 回退一个字符，这个字符现在没有catToken
			strcpy(symbol, "LSS");
		}
	}
	
	else if (isGreater()) {
		catToken(); 
		 
		getChar(); // 再读入一个字符 
		if (isEqu()) { // >=
			catToken();
			strcpy(symbol, "GEQ"); 
		}
		else {
			retract(); // 回退一个字符，这个字符现在没有catToken
			strcpy(symbol, "GRE");
		}
	}
	
	else if (isExcla()){
		catToken();
		getChar(); // 肯定是=， 不然报错
		catToken();
		strcpy(symbol, "NEQ"); 
	}
	else if (isPlus()) {
		catToken();
		strcpy(symbol, "PLUS");
	}
	else if (isMinu()) {
		catToken();
		strcpy(symbol, "MINU");
	}
	else if (isDiv()) {
		catToken();
		strcpy(symbol, "DIV");
	}
	else if (isMult()) {
		catToken();
		strcpy(symbol, "MULT");
	}
	else if (isLparent()) {
		catToken();
		strcpy(symbol, "LPARENT");
	}
	else if (isRparent()) {
		catToken();
		strcpy(symbol, "RPARENT");
	}
	else if (isLbrace()) {
		catToken();
		strcpy(symbol, "LBRACE");
	}
	else if (isRbrace()) {
		catToken();
		strcpy(symbol, "RBRACE");
	}
	else if (isLbrack()) {
		catToken();
		strcpy(symbol, "LBRACK");
	}
	else if (isRbrack()) {
		catToken();
		strcpy(symbol, "RBRACK");
	}
	else if (isSemicn()) {
		catToken();
		strcpy(symbol, "SEMICN");
	}
	else if (isComma()) {
		catToken();
		strcpy(symbol, "COMMA");
	}
	else if (isSinQuo()){
//		catToken();
		getChar();
		if (!(tmpChar == '+' || tmpChar == '-' || tmpChar == '*' || tmpChar == '/' || (tmpChar >= '0' && tmpChar <= '9')
			|| tmpChar == '_' ||(tmpChar >= 'a' && tmpChar <= 'z') || (tmpChar >= 'A' && tmpChar <= 'Z'))) errorA.push_back(gline);
		catToken();
		strcpy(symbol, "CHARCON");
		getChar();
	}
	else if (isDouQuo()) {
		getChar();
		if (isDouQuo()) errorA.push_back(gline); //空字符串

		while(!isDouQuo()) {
			if (!(tmpChar == 32 || tmpChar == 33 || (tmpChar >= 35 && tmpChar <= 126))) errorA.push_back(gline);
			catToken();
			getChar();
		}
		strcpy(symbol, "STRCON");
	}
	else {
		errorA.push_back(gline);
	}
}

void ReadFiles()
{
	if ((fp = fopen("testfile.txt", "r")) == NULL) {
		printf("Can not open testfile!");
		exit(1);
	}
	/*if ((out = fopen("output.txt", "w+")) == NULL) {
		printf("Can not open out file!");
		exit(1);
	}*/
	if ((mips_file = fopen("mips.txt", "w+")) == NULL) {
		printf("Can not open mips file!");
		exit(1);
	}

	while (fgets(buffer, MAX_LINE, fp) != NULL) {
		strcat(text, buffer);
	}
}

void CloseFiles()
{
	fclose(fp);
	//fclose(out);
	fclose(mips_file);
	//fclose(errorfile);
}


void error()
{
	//cout << "there is an error in line: " << gline  << sym << "zhang"<< word << endl;
}

void work2()
{
	int i = 0;
	while (cur <= strlen(text) - 1) {
		getsym();
		if (strcmp(symbol, "") != 0) {
			s[i].symbolx = symbol;
			s[i].wordx = token;
			s[i].line = gline;
			s[i].isfirst = isFirstInLine;
			if (isFirstInLine) isFirstInLine = false;
			//printf("%s %s %d\n", s[i].symbolx.c_str(), s[i].wordx.c_str(), s[i].line);
		}
		i++;
	}
	slength = i;
}