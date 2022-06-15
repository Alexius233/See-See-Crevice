#include <iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cstdlib>
#include<math.h>
#define itp 0
#define rtp 1
#define ctp 2
#include"wordanaly.h"

class Grammar{
public:
	void init(string& sin) { input = sin;  point = 0; AM.createMap(); sym.creat_list(); }
	void mainprogramme();
	void YUJU_list();
	void YUJU();
//变量声明语句
void var_define();
void var_type();
void namelist();
void var_init();
//算术表达式语句
void E();
void T();
void F();
void G();
void Q();
void OR();
void AND();
void M();
void L();
void CMP();
void B();
void C();
void EQU();
//赋值语句
void FUZHI();
void Elist();
void nextw();
//if语句
void if_YUJU();
void else_YUJU();

//复合语句
void com_YUJU();

//逻辑语句
void logic();


int point;
Machine AM;//扫描器
symbollist sym;//符号表
string input;
token t;//当前token
string type;
};
//---------------------------------------
int typepoint(string type) {
	if (type == "int") {
		return itp;
	}
	else if (type == "float") {
		return rtp;
	}
	else {
		return ctp;
	}


}


//--------------------------------------编译器类
class complier{
public:
	string input;
	int point;
	symbollist sym;
	Machine AM;
	Grammar GR;
	void begincomplie(string sin);
};
//----------------------------------------------
void complier::begincomplie(string sin){
	input=sin;
    GR.init(input);
    GR.mainprogramme();
	GR.sym.showlist();
	//point = 0;
	//sym.creat_list();
	//AM.createMap();
	//while (point < sin.length() - 1)
	//{
	//	AM.getString(point, input,sym);
	//	point--;
	//}
	//cout << endl<<"编译成功！";
}
void Grammar::nextw(){//获取一个token
	//cout << point;
	if(point<input.length()-1)
	{	
		AM.getString(point,input,sym);
		point--;
		t = sym.t;

		cout << "("<<t.type << "," << t.point<<")";
	}
	else {

		t.type = "END";
		t.point = 0;
		cout<<endl << "词法分析成功！"<< endl;
	}
}
void Grammar::mainprogramme() {
	point = 0;
	nextw();
	YUJU_list();//语句表
	if (t.type == "END") {//结束
		cout << "语法分析通过！";
		return;
	}
	else {

		cout << endl<<"未知ERROR!";
		exit(0);
	}



}

//语句生成文法
void Grammar::YUJU_list() {//<语句表>-><语句><语句表> |$
	//cout << t.type;
	if (t.type != "END") {//不是结尾
		cout << "语句表";
		YUJU();//一条语句
		YUJU_list();
	}
	else {//是结尾
		return;
	}
	
}
void Grammar::YUJU() {//<语句> -><赋值语句>|<变量说明>|<if语句>|<while语句>|$

	if (t.type == "K" && (sym.K_list[t.point] == "int" || sym.K_list[t.point] == "float" || sym.K_list[t.point] == "char")) {//类型声明在前，为变量声明语句
		var_define();
	}
	else if (t.type == "K" && sym.K_list[t.point] == "if") {//条件语句
		//cout << "条件语句";
		if_YUJU();
		return;
	}
	else if (t.type == "K" && sym.K_list[t.point] == "while") {//while语句

	}
	else if (t.type == "I" ) {//赋值语句

		FUZHI();

	}
	else {
		return;
	}
}

//变量声明-----------------------------------------------------------------------------------------------------------------------
void Grammar::var_define() {
	if (t.type=="K"&&(sym.K_list[t.point] == "int" || sym.K_list[t.point] == "float" || sym.K_list[t.point] == "char")) {//左产 
		
		var_type();
		namelist();
		if (sym.P_list[t.point] == ";") {
			nextw();
			return;

		}
		else {
			cout << endl<<"ERROR： 缺少分号！";
			exit(0);
		}


	}
	
	else {
		cout << endl<<"ERROR: 未定义的变量类型！--"<< sym.I_list[t.point];
		exit(0);
	}

}

void Grammar::var_type(){
	if (sym.K_list[t.point] == "int" || sym.K_list[t.point] == "float" || sym.K_list[t.point] == "char") {
		nextw();
		//--------------------------------------------语义动作，获取当前变量类型
		type = sym.K_list[t.point];
		//---------------------------------------------
		return;
	}
	else{
		cout<<"非法的类型！";
		exit(0);
	}
	
	
	
}
void Grammar:: var_init() {
	if (t.type == "I") {

		//---------------------------------------------语义动作，获取当前变量名称填表
		if (t.point < sym.SYMBOL.size() - 1) {//之前出现过，变量重复定义！
			cout << endl<<"ERROR 变量"<<sym.SYMBOL[t.point].name<<"重复定义！";
			exit(0);
		}
		else {
			sym.SYMBOL[t.point].type = typepoint(type);
			sym.SYMBOL[t.point].cat = "v";
		}
		nextw();


		//---------------------------------------------




		if (t.type == "P" && sym.P_list[t.point] == "=") {

			nextw();
			E();
			return;
		}
		else {
			return;
		}
	}
	else {


		cout << "ERROR！ 无法定义该类型";
		exit(0);
	}




}
void Grammar::namelist(){
	var_init();
	if (t.type == "P" && sym.P_list[t.point] == ",") {
		nextw();
		namelist();
		return;
	}
	else {
		return;
	}

	
}
//算术表达式文法------------------------------------------------------------------------------------------------------------------------------------------
void Grammar:: E() {//E->ORL删除左递归
	//cout << "算术表达式";
	OR();
	L();
	return;

}
void Grammar::L() {//L->||ORL
	if (t.type == "P" &&sym.P_list[t.point] == "||" ) {
		nextw();
		OR();
		L();
		return;
	}
	else {

		return;
	}


}
void Grammar::OR() {

	AND();
	M();
	return;
}
void Grammar::M() {//M->&&ANDM
	if (t.type == "P" && sym.P_list[t.point] == "&&") {
		nextw();
		AND();
		M();
		return;
	}
	else {

		return;
	}

}
void Grammar::AND() {//AND->CMPB
	CMP();
	B();
	return;


}

void Grammar::B() {//B->wCMPB
	if (t.type == "P" && (sym.P_list[t.point] == "<"|| sym.P_list[t.point] == ">"|| sym.P_list[t.point] == ">="|| sym.P_list[t.point] == "<=")) {
		nextw();
		CMP();
		B();
		return;
	}
	else {

		return;
	}




}
void Grammar:: CMP() {//CMP->EQUC
	EQU();
	C();

}
void Grammar::C() {//C->wEQUC
	if (t.type == "P" && (sym.P_list[t.point] == "==" || sym.P_list[t.point] == "!=" )) {
		   nextw();
			EQU();
			C();
			return;
	}
	else {

		return;
	}


}
void Grammar::EQU() {//EQU->TG
	T();
	G();


}
void Grammar::G() {//G->wTG|$
	if (t.type=="P"&&(sym.P_list[t.point]=="+"|| sym.P_list[t.point] == "-")) {
		nextw();
		T();
		G();
		return;
	}
	else {

		return;
	}

}
void Grammar::F() {
	if (t.type == "I"||t.type=="C1"||t.type=="C2"||t.type=="CT") {
		nextw();
		return;
	}
	else if (t.type=="P"&&sym.P_list[t.point] =="(") {
		nextw();
		E();

		if (t.type == "P" && sym.P_list[t.point] == ")") {
			nextw();
			return;
		}
		else {
			cout << "ERROR: 缺少)";
			exit(0);
		}


	}
	else {
		cout << "ERROR: 此类型无法进行运算！";
		exit(0);
	}

}
void Grammar:: T() {//T->FQ
	F();
	Q();
	return;
}
void Grammar::Q() {//Q->wFQ|$

	if (t.type == "P" && (sym.P_list[t.point] == "*" || sym.P_list[t.point] == "/")) {
		nextw();
		F();
		Q();
		return;
	}
	else {
		return;

	}

}

//赋值语句文法----------------------------------------------------------------------------------------------------------
void Grammar::FUZHI() {

	Elist();
	if (t.type == "P" && sym.P_list[t.point] == ";") {
		nextw();
		return;
	}
	else {
		cout << "ERROR!缺少分号！";
		exit(0);
	}



}
//<等式表>-> <标识符> = <算术表达式>，<等式表>| <标识符> = <算术表达式>

void Grammar::Elist() {
	if (t.type == "I") {
		nextw();

		if (t.type == "P" && sym.P_list[t.point] == "=") {
			nextw();
			E();

			if (t.type == "P" && sym.P_list[t.point] == ",") {
				nextw();
				Elist();
				return;

			}
			else {
				return;

			}
		}
		else {
			cout << "ERROR 缺少=";
			exit(0);
		}
		
	
	}
}


//复合语句------------------------------------------------------------------------------------------
void Grammar::com_YUJU() {
	if (t.type == "P" && sym.P_list[t.point] == "{") {
		cout << "{";
		nextw();
		YUJU_list();
		if (t.type == "P" && sym.P_list[t.point] == "}") {
			cout << "pppp";
			nextw();
			cout << t.type;
			return;
		}
		else {

			cout << "ERROR 缺少}!";
		}
		
	}
	else {//无花括号，只可跟一条语句
		YUJU();
		return;
	}
}
//if语句-----------------------------------------------------------------------------------------------
void Grammar::if_YUJU()/*<if语句>-><if关键字>(<算术表达式>)<复合语句><else语句>
                       <else语句>-><else关键字>(<算术表达式>)<复合语句>|$*/
{

	if (t.type == "K" && sym.K_list[t.point] == "if") {
		cout << "<if关键字>";
		nextw();
		if (t.type == "P" && sym.P_list[t.point] == "(") {
			//cout << "(";
			nextw();
			E();
			cout << "算术表达式结束";
			if (t.type == "P" && sym.P_list[t.point] == ")") {
				cout << ")";
				nextw();
				com_YUJU();
				else_YUJU();
				return;
			}
			else {
				cout << "ERROR！ 缺少）";
			}

			return;

		}
		else {

			cout << "ERROR！ 缺少（";
		}
	}
	
}
void Grammar::else_YUJU() {//<else语句>-><else关键字><复合语句>|$
	//cout << "else语句";
	if (t.type == "K" && sym.K_list[t.point] == "else") {
		nextw();
		com_YUJU();
		return;
	}
	else {
		return;
	}

}

int main(){
	fstream in;
	char ch;
	string sin;
	in.open("test.txt", ios::in);
	if (!in)cout << "文件打开失败";
	while (in >> noskipws >> ch) {
		sin += ch;
	}
	
	sin+="#";
	//cout << sin;
	complier cp;
	cp.begincomplie(sin);
	
	
	
}
