#pragma once
#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
using namespace std;
extern int row;
class Token {		//Token
public:
	Token(char t, string c, int r);
	Token();
	void set(char t, string c, int r);

	char type;		//单词的类型，如关键字，标识符，界符等
	string content;			//这个token代表的单词
	int row;
};

class Recognizer {		//扫描器
public:
	string  fname;				//扫描的文件名
	ifstream ifs;				//输入流
	char ch;					//当前读出的字符
	int state;					//自动机的状态
	int type;					//扫描出的单词的类型
	string keyOrIdentifier;
	string number;
	string String;
	string c;

	int numState;
	double n, num;
	int p, m, t, e;

	void init(string f);		//确定扫描器要扫描的文件
	string getAWord();			//从源文件读取一个单词，返回给词法分析器

private:
	void getAChar();			//从源文件中读出一个字符
	void pushbackAChar();		//回退一个字符到流中
	bool isALetter();			//判断一个字符是否是字符
	bool isANumber();			//判断一个字符是否是数字

};

class Lexer {		//词法分析器，O(∩_∩)O
public:
	void init(string name);		//初始化扫描器
	Token next_Token();		//产生一个Token,如果是标识符指向符号表，关键字指向关键字表，界符指向界符表，常量指向常量表
private:
	Recognizer rec;				//扫描器
	string currentWord;			//当前读到的单词
};
