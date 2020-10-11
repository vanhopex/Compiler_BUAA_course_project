#include "public.h"
#include "work2.h"
using namespace std;

void clearToken()
{
	memset(token, 0, sizeof(token));
}
bool isSpace()
{
	return (tmpChar == ' ' || tmpChar == '\t' || tmpChar == '\n');
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
		strcpy_s(symbol, "CONSTTK");
	}
	else if (strcmp(t, "int") == 0) {
		strcpy_s(symbol, "INTTK");
	}
	else if (strcmp(t, "char") == 0) {
		strcpy_s(symbol, "CHARTK");
	}
	else if (strcmp(t, "void") == 0) {
		strcpy_s(symbol, "VOIDTK");
	}
	else if (strcmp(t, "main") == 0) {
		strcpy_s(symbol, "MAINTK");
	}
	else if (strcmp(t, "if") == 0) {
		strcpy_s(symbol, "IFTK");
	}
	else if (strcmp(t, "else") == 0) {
		strcpy_s(symbol, "ELSETK");
	}
	else if (strcmp(t, "switch") == 0) {
		strcpy_s(symbol, "SWITCHTK");
	}
	else if (strcmp(t, "case") == 0) {
		strcpy_s(symbol, "CASETK");
	}
	else if (strcmp(t, "default") == 0) {
		strcpy_s(symbol, "DEFAULTTK");
	}
	else if (strcmp(t, "while") == 0) {
		strcpy_s(symbol, "WHILETK");
	}
	else if (strcmp(t, "for") == 0) {
		strcpy_s(symbol, "FORTK");
	}
	else if (strcmp(t, "scanf") == 0) {
		strcpy_s(symbol, "SCANFTK");
	}
	else if (strcmp(t, "printf") == 0) {
		strcpy_s(symbol, "PRINTFTK");
	}
	else if (strcmp(t, "return") == 0) {
		strcpy_s(symbol, "RETURNTK");
	}
	else {
		strcpy_s(symbol, "IDENFR");
	}
}

void getsym()
{
	clearToken();
	memset(symbol, 0, sizeof(symbol));
	
	getChar();
	
	while (isSpace()) { // if the tmpchar is " ", "\t", "\n" skip it;
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

		strcpy_s(symbol, "INTCON");
	}
	
	else if (isColon()) {
		catToken();
		strcpy_s(symbol, "COLON");
	}

	else if (isEqu()) {

		catToken();
		getChar();
		
		if (isEqu()) {
			catToken();
			strcpy_s(symbol, "EQL");
		}
		else {
			strcpy_s(symbol, "ASSIGN");
			retract(); // error 2: 这里要回退一个字符 
		}	
	}
	
	else if (isLess()) {
		catToken(); 
		 
		getChar(); // 再读入一个字符 
		if (isEqu()) { // <=
			catToken();
			strcpy_s(symbol, "LEQ"); 
		}
		else {
			retract(); // 回退一个字符，这个字符现在没有catToken
			strcpy_s(symbol, "LSS");
		}
	}
	
	else if (isGreater()) {
		catToken(); 
		 
		getChar(); // 再读入一个字符 
		if (isEqu()) { // >=
			catToken();
			strcpy_s(symbol, "GEQ"); 
		}
		else {
			retract(); // 回退一个字符，这个字符现在没有catToken
			strcpy_s(symbol, "GRE");
		}
	}
	
	else if (isExcla()){
		catToken();
		getChar(); // 肯定是=， 不然报错
		catToken();
		strcpy_s(symbol, "NEQ"); 
	}
	else if (isPlus()) {
		catToken();
		strcpy_s(symbol, "PLUS");
	}
	else if (isMinu()) {
		catToken();
		strcpy_s(symbol, "MINU");
	}
	else if (isDiv()) {
		catToken();
		strcpy_s(symbol, "DIV");
	}
	else if (isMult()) {
		catToken();
		strcpy_s(symbol, "MULT");
	}
	else if (isLparent()) {
		catToken();
		strcpy_s(symbol, "LPARENT");
	}
	else if (isRparent()) {
		catToken();
		strcpy_s(symbol, "RPARENT");
	}
	else if (isLbrace()) {
		catToken();
		strcpy_s(symbol, "LBRACE");
	}
	else if (isRbrace()) {
		catToken();
		strcpy_s(symbol, "RBRACE");
	}
	else if (isLbrack()) {
		catToken();
		strcpy_s(symbol, "LBRACK");
	}
	else if (isRbrack()) {
		catToken();
		strcpy_s(symbol, "RBRACK");
	}
	else if (isSemicn()) {
		catToken();
		strcpy_s(symbol, "SEMICN");
	}
	else if (isComma()) {
		catToken();
		strcpy_s(symbol, "COMMA");
	}
	else if (isSinQuo()){
//		catToken();
		getChar();
		catToken();
		strcpy_s(symbol, "CHARCON");
		getChar();
	}
	else if (isDouQuo()) {
		getChar();
		while(!isDouQuo()) {
			catToken();
			getChar();
		}
		strcpy_s(symbol, "STRCON");
	}
}

void ReadFiles()
{
	if ((fp = fopen("testfile.txt", "r")) == NULL) {
		printf("Can not open testfile!");
		exit(1);
	}
	if ((out = fopen("output.txt", "w+")) == NULL) {
		printf("Can not open out file!");
		exit(1);
	}

	while (fgets(buffer, MAX_LINE, fp) != NULL) {
		strcat_s(text, buffer);
	}
}

void CloseFiles()
{
	fclose(fp);
	fclose(out);
}


void error()
{
	cout << "there is an error in line：" << endl;
}

void work2()
{
	int i = 0;
	while (cur <= strlen(text) - 1) {
		getsym();;
		if (strcmp(symbol, "") != 0) {
			s[i].symbolx = symbol;
			s[i].wordx = token;
			printf("%s %s\n", s[i].symbolx.c_str(), s[i].wordx.c_str());
		}
		i++;
	}
	slength = i;
}