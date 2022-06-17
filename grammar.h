#pragma once
#include"word.h"
#include"symbol.h"
#include"four.h"
#include<string>
using namespace std;

extern int bool_jubu; //判断是不是在定义局部变量

extern string id_qt;//四元式保存标识符和数组位置
extern string num_qt;
extern int arr_num_qt;//数组元素

extern string func_name;//记录当前函数名称

extern Lexer scan;			//扫描器
extern Token token_g;			//读出来的token串

extern string changtype_1;//常量类型
extern string changtype_2;

extern vector<string> T_num;//保存被用户定义的临时变量tnum

extern string func_call;//调用函数函数名
extern int func_call_num;
extern int parapos;//函数参数位置

void Error(int col, string tmp, string tmp2);//错误分析
int Recursive();//子程序主函数
int Variable_Type();//变量类型
int ID();//标识符
int NumCharID();//初始化常量
int ZeroOrMoreDeclation();//声明列表
int Declaration();//声明当前字符的类型
int Const_Declaration();//常量定义const
int Variable_Declaration();//变量定义（数组、函数）
int Array_Declaration();//数组声明、变量初始化、函数定义、变量声明
int EqualStringOrNumOrNull();//数组初始化
int StringOrNum();
int Statement();		//语句的函数
int Block();			//函数内部内容
int ExpressionStatement();//带；的表达式语句
int Expression();//表达式
int Assignment();//赋值语句
int Assignment_1();
int Logical_Expression();//
int Other_Logical();//
int Equality();
int Equality_2();
int Arithmetic_Expression();
int Arithmetic_Expression_2();
int MD_Expression();
int Arithmetic_Expression_1();
int MD_Expression_1();
int Factor();
int Primary();//终结符

int Paraters();			//形参
int Realparaters();
int Realparaters_2();
int Realparaters_3();
int Other_Type_ID();
int ArrFunc();//访问数组







