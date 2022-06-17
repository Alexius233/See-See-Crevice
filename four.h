#pragma once
#pragma once


#include<string>
#include<vector>
#include<iostream>
#include<sstream>
using namespace std;


extern int rr;//临时变量生成
extern int sem;

extern string type_1;//操作数类型
extern string type_2;

extern string value_1; //操作数值
extern string value_2;

extern int func_num;
struct token
{
	string name;
	int type;// 1常量，2非临时变量，3是临时变量
	bool active;//活跃信息
	token() { name = ""; type = 0; active = false; }
	token(string namet, int typet, bool activet) { name = namet; type = typet; active = activet; }
	void clear() { name = ""; type = 0; active = false; }
};

extern token left_n;
extern token right_n;
extern token res_n;

struct QT
{
	string Operat;//操作符
	token opA;
	token opB;
	token res;
	int block;//基本块
	QT() { Operat = ""; opA.clear(); opB.clear(); res.clear(); }
	QT(string Operatt, token opAt, token opBt, token rest) { Operat = Operatt; opA = opAt; opB = opBt; res = rest; }
	void clear() { Operat = ""; opA.clear(); opB.clear(); res.clear(); }

};

extern vector<QT> qt;

template<class T>
class mystack
{
	int top;
	T* mys;
	int maxsize;
public:
	mystack() :top(-1), maxsize(111)
	{
		mys = new T[maxsize];
		if (mys == NULL)
		{
			cerr << "动态存储分配失败！" << endl;
			exit(1);
		}
	}
	mystack(int size) :top(-1), maxsize(size)
	{
		mys = new T[maxsize];
		if (mys == NULL)
		{
			cerr << "动态存储分配失败！" << endl;
			exit(1);
		}
	}
	//~mystack()
	//{
	//	delete[] mys;
	//}
	bool Empty();//是否为空
	void PUSH(T tp); //压栈
	T TOP();//返回栈顶元素
	T SEC();//返回次栈顶元素
	void POP();//出栈
	int SIZE();//栈大小
};

//模板函数：将string类型变量转换为常用的数值类型（此方法具有普遍适用性）
template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

extern mystack<token> SEM;
bool CompareType();
string resvalue(string opeart);
void ASSI();
void WriteinFileL(string name, string content);
void PUSHSEM(token operand);//写入文件
void GEQ(string operat);
void ASSI();
void IF();
void EL();
void IE();
void WH();
void DO();
void WE();
void RET();//RETURN语句：第一操作数
void WriteQt(string name, string content);//四元式写入文件

void FUNC();//定义函数开始语句，第一操作数为函数名
void MAIN();//主函数开始语句
void PARA();//函数参数
void CALL();//过程调用：第一操作数调用函数名
void RET();//RETURN语句：第一操作数
void divide();//划分基本块
void PARA_n();//形参
