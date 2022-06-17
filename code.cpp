//取、存		:MOV Ri,Rk/M		Ri=(Rk/M)
//转向		:
//算术运算	:ADD Ri,Rk/M		Ri=(Ri)+(Rk/M)
//			:SUB Ri,Rk/M		Ri=(Ri)-(Rk/M)
//逻辑运算	:
#include "DAG.h"
#include "code.h"
#include <fstream>
#include <queue>

token AX; //单寄存器
token BX;
token CX;
token DX;

queue<token>param;
vector<token>RETm;
string namem;//存储函数名字
int ppp;//函数参数个数

void createDSEG() //建立DSEG段
{
	//查符号表，给变量生成空间
	//查变量时由于汇编不区分大小写，所以应该在这里
	ofstream file;
	file.open("C:\\byresult\\code.txt");
	file << "SSEG	SEGMENT STACK" << endl;
	file << "SKT	DB	200 DUP(0)" << endl;
	file << "SSEG	ENDS" << endl;
	file << "DSEG	SEGMENT " << endl;
	vector<token>::iterator tokent; //原本是遍历符号表，这里先遍历我本身的SYMBL
	for (tokent = SYMBL_ALL.begin(); tokent != SYMBL_ALL.end(); tokent++)
	{
		file << "	" << tokent->name << "	DW	2 DUP(0)" << endl;
	}
	file << "DSEG	ENDS " << endl;
}

void createCSEGBEGIN() //建立CSEG段
{
	ofstream file;
	file.open("C:\\byresult\\code.txt", ios::app);
	file << "CSEG	SEGMENT " << "\n"
		<< "	ASSUME	CS:CSEG,DS:DSEG,SS:SSEG" << endl;
	file << "START:	MOV	AX,DSEG" << "\n"
		<< "	MOV	DS,AX" << endl;
	file << "	MOV	AX,SSEG" << "\n"
		<< "	MOV	SS,AX" << "\n"
		<< "	MOV	SP,SIZE SKT" << endl;
	file.close();
}

void createCSEGEND() //建立CSEG段
{
	ofstream file;
	file.open("C:\\byresult\\code.txt", ios::app);
	file << "	MOV	AX,4C00H" << "\n"
		<< "	INT	21H" << "\n"
		<< "CSEG	ENDS " << "\n"
		<< "	END	START" << endl;
	file.close();
}

void createCode(vector<QT>resQT) //根据一个四元式组生成一段代码
{
	ofstream file;
	file.open("C:\\byresult\\code.txt", ios::app);
	vector<QT>::iterator it;
	string lastOper = "";
	int i = 1; //四元式序号
	int j = 1;
	for (it = resQT.begin(); it != resQT.end(); it++, i++)
	{
		code(*it, lastOper, i);
		lastOper = it->Operat;
	}
	if (AX.active)
	{
		file << "	MOV	" << AX.name << ",AX" << endl;
	}
}

void code(QT temp, string lastOper, int i) //根据单个四元式生成CODE
{
	ofstream addfile;
	addfile.open("C:\\byresult\\code.txt", ios::app);
	if (temp.Operat == "=")	//赋值语句
	{
		if (lastOper == "para_n")
		{
			addfile << "	ASSUME	CS:" << namem << ",DS:" << namem << endl;
			addfile << "    PUSH  " << "AX" << endl;
			addfile << "    PUSH  " << "BX" << endl;
			addfile << "    PUSH  " << "CX" << endl;
			addfile << "    PUSH  " << "DX" << endl;
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",AX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",BX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",CX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",DX" << endl;
				param.pop();
			}
		}
		if (AX.active && AX.name != "")
			addfile << "	MOV	" << AX.name << ",AX" << endl;
		addfile << "	MOV	" << "AX," << temp.opA.name << endl;
		AX = temp.res;
	}
	else if (temp.Operat == "+") //加法
	{
		if (lastOper == "para_n")
		{
			addfile << "	ASSUME	CS:" << namem << ",DS:" << namem << endl;
			addfile << "                PUSH  " << "AX" << endl;
			addfile << "                PUSH  " << "BX" << endl;
			addfile << "                PUSH  " << "CX" << endl;
			addfile << "                PUSH  " << "DX" << endl;
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",AX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",BX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",CX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",DX" << endl;
				param.pop();
			}
		}
		if (AX.name == temp.opA.name || AX.name == temp.opB.name || BX.name == temp.opA.name || BX.name == temp.opB.name || CX.name == temp.opA.name || CX.name == temp.opB.name || DX.name == temp.opA.name || DX.name == temp.opB.name)
		{
			if (AX.name == temp.opA.name && !(AX.active))
			{
				addfile << "	ADD	" << "AX," << temp.opB.name << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opA.name && AX.active)
			{
				addfile << "	MOV	" << AX.name << ",AX" << endl;
				addfile << "	ADD	" << "AX," << temp.opB.name << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opB.name && !(AX.active))
			{
				addfile << "	ADD	" << "AX," << temp.opA.name << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opB.name && AX.active)
			{
				addfile << "	MOV	" << AX.name << ",AX" << endl;
				addfile << "	ADD	" << "AX," << temp.opA.name << endl;
				AX = temp.res;
			}
			else if (BX.name == temp.opA.name && !(BX.active))
			{
				addfile << "	ADD	" << "BX," << temp.opB.name << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opA.name && BX.active)
			{
				addfile << "	MOV	" << BX.name << ",BX" << endl;
				addfile << "	ADD	" << "BX," << temp.opB.name << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opB.name && BX.active)
			{
				addfile << "	MOV	" << BX.name << ",BX" << endl;
				addfile << "	ADD	" << "BX," << temp.opA.name << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opB.name && !(BX.active))
			{
				addfile << "	ADD	" << "BX," << temp.opA.name << endl;
				BX = temp.res;
			}
			else if (CX.name == temp.opA.name && !(CX.active))
			{
				addfile << "	ADD	" << "CX," << temp.opB.name << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opA.name && CX.active)
			{
				addfile << "	MOV	" << CX.name << ",CX" << endl;
				addfile << "	ADD	" << "CX," << temp.opB.name << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opB.name && CX.active)
			{
				addfile << "	MOV	" << CX.name << ",CX" << endl;
				addfile << "	ADD	" << "CX," << temp.opA.name << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opB.name && !(CX.active))
			{
				addfile << "	ADD	" << "CX," << temp.opA.name << endl;
				CX = temp.res;
			}
			else if (DX.name == temp.opA.name && !(DX.active))
			{
				addfile << "	ADD	" << "DX," << temp.opB.name << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opA.name && DX.active)
			{
				addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	ADD	" << "DX," << temp.opB.name << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opB.name && DX.active)
			{
				addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	ADD	" << "DX," << temp.opA.name << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opB.name && !(DX.active))
			{
				addfile << "	ADD	" << "DX," << temp.opA.name << endl;
				DX = temp.res;
			}
		}
		else if (!(AX.active))
		{
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	ADD	" << "AX," << temp.opB.name << endl;
			AX = temp.res;
		}
		else if (!(BX.active))
		{
			addfile << "	MOV	" << "BX," << temp.opA.name << endl;
			addfile << "	ADD	" << "BX," << temp.opB.name << endl;
			BX = temp.res;
		}
		else if (!(CX.active))
		{
			addfile << "	MOV	" << "CX," << temp.opA.name << endl;
			addfile << "	ADD	" << "CX," << temp.opB.name << endl;
			CX = temp.res;
		}
		else if (!(DX.active))
		{
			addfile << "	MOV	" << "DX," << temp.opA.name << endl;
			addfile << "	ADD	" << "DX," << temp.opB.name << endl;
			DX = temp.res;
		}
		else
		{
			addfile << "	MOV	" << AX.name << ",AX" << endl;
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	ADD	" << "AX," << temp.opB.name << endl;
			AX = temp.res;
		}
		/*if (AX.active && AX.name != "")
			addfile << "	MOV	" << AX.name << ",AX" << endl;
		addfile << "	MOV	" << "AX," << temp.opA.name << endl;
		addfile << "	ADD	" << "AX," << temp.opB.name << endl;
		AX = temp.res;*/
	}
	else if (temp.Operat == "-" && (temp.opB.name != "")) //减法，这里要加一个符号，还没写
	{
		if (lastOper == "para_n")
		{
			addfile << "	ASSUME	CS:" << namem << ",DS:" << namem << endl;
			addfile << "                PUSH  " << "AX" << endl;
			addfile << "                PUSH  " << "BX" << endl;
			addfile << "                PUSH  " << "CX" << endl;
			addfile << "                PUSH  " << "DX" << endl;
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",AX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",BX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",CX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",DX" << endl;
				param.pop();
			}
		}
		if (AX.name == temp.opA.name || BX.name == temp.opA.name || CX.name == temp.opA.name || DX.name == temp.opA.name)
		{
			if (AX.name == temp.opA.name && AX.active)
			{
				addfile << "	MOV	" << AX.name << ",AX" << endl;
				addfile << "	SUB	" << "AX," << temp.opB.name << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opA.name && !(AX.active))
			{
				addfile << "	SUB	" << "AX," << temp.opB.name << endl;
				AX = temp.res;
			}
			else if (BX.name == temp.opA.name && !(BX.active))
			{
				addfile << "	SUB	" << "BX," << temp.opB.name << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opA.name && BX.active)
			{
				addfile << "	MOV	" << BX.name << ",BX" << endl;
				addfile << "	SUB	" << "BX," << temp.opB.name << endl;
				BX = temp.res;
			}
			else if (CX.name == temp.opA.name && !(CX.active))
			{
				addfile << "	SUB	" << "CX," << temp.opB.name << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opA.name && CX.active)
			{
				addfile << "	MOV	" << CX.name << ",CX" << endl;
				addfile << "	SUB	" << "CX," << temp.opB.name << endl;
				CX = temp.res;
			}
			else if (DX.name == temp.opA.name && !(DX.active))
			{
				addfile << "	SUB	" << "DX," << temp.opB.name << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opA.name && DX.active)
			{
				addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	SUB	" << "DX," << temp.opB.name << endl;
				DX = temp.res;
			}

		}
		else if (!(AX.active))
		{
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	SUB	" << "AX," << temp.opB.name << endl;
			AX = temp.res;
		}
		else if (!(BX.active))
		{
			addfile << "	MOV	" << "BX," << temp.opA.name << endl;
			addfile << "	SUB	" << "BX," << temp.opB.name << endl;
			BX = temp.res;
		}
		else if (!(CX.active))
		{
			addfile << "	MOV	" << "CX," << temp.opA.name << endl;
			addfile << "	SUB	" << "CX," << temp.opB.name << endl;
			CX = temp.res;
		}
		else if (!(DX.active))
		{
			addfile << "	MOV	" << "DX," << temp.opA.name << endl;
			addfile << "	SUB	" << "DX," << temp.opB.name << endl;
			DX = temp.res;
		}
		else
		{
			addfile << "	MOV	" << AX.name << ",AX" << endl;
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	SUB	" << "AX," << temp.opB.name << endl;
			AX = temp.res;
		}
		/*if (AX.active && AX.name != "")
			addfile << "	MOV	" << AX.name << ",AX" << endl;
		addfile << "	SUB	" << "AX," << temp.opB.name << endl;
		AX = temp.res;*/
	}
	else if (temp.Operat == "*") //乘法
	{
		if (lastOper == "para_n")
		{
			addfile << "	ASSUME	CS:" << namem << ",DS:" << namem << endl;
			addfile << "                PUSH  " << "AX" << endl;
			addfile << "                PUSH  " << "BX" << endl;
			addfile << "                PUSH  " << "CX" << endl;
			addfile << "                PUSH  " << "DX" << endl;
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",AX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",BX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",CX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",DX" << endl;
				param.pop();
			}
		}
		if (AX.name == temp.opA.name || AX.name == temp.opB.name)
		{
			if (AX.name == temp.opA.name && !(AX.active))
			{
				if (DX.active)
					addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	IMUL	" << temp.opB.name << endl;
				addfile << "	MOV	" << temp.res.name << ",AX" << endl;
				addfile << "	MOV	" << temp.res.name << "+2,DX" << endl;
				AX.clear(); DX.clear();
			}
			else if (AX.name == temp.opA.name && AX.active)
			{
				if (DX.active)
					addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	MOV	" << AX.name << ",AX" << endl;
				addfile << "	IMUL	" << temp.opB.name << endl;
				addfile << "	MOV	" << temp.res.name << ",AX" << endl;
				addfile << "	MOV	" << temp.res.name << "+2,DX" << endl;
				AX.clear(); DX.clear();
			}
			else if (AX.name == temp.opB.name && AX.active)
			{
				if (DX.active)
					addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	MOV	" << AX.name << ",AX" << endl;
				addfile << "	IMUL	" << temp.opA.name << endl;
				addfile << "	MOV	" << temp.res.name << ",AX" << endl;
				addfile << "	MOV	" << temp.res.name << "+2,DX" << endl;
				AX.clear(); DX.clear();
			}
			else if (AX.name == temp.opB.name && !(AX.active))
			{
				if (DX.active)
					addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	IMUL	" << temp.opA.name << endl;
				addfile << "	MOV	" << temp.res.name << ",AX" << endl;
				addfile << "	MOV	" << temp.res.name << "+2,DX" << endl;
				AX.clear(); DX.clear();
			}
		}
		else
		{
			if (DX.active)
				addfile << "	MOV	" << DX.name << ",DX" << endl;
			if (AX.active)
				addfile << "	MOV	" << AX.name << ",AX" << endl;
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	IMUL	" << temp.opB.name << endl;
			addfile << "	MOV	" << temp.res.name << ",AX" << endl;
			addfile << "	MOV	" << temp.res.name << "+2,DX" << endl;
			AX.clear(); DX.clear();
		}
		/*if (AX.active && AX.name != "")
			addfile << "	MOV	" << AX.name << ",AX" << endl;
		addfile << "	MOV	" << "AX," << temp.opB.name << endl;
		addfile << "	MUL	" << temp.opB.name << endl;
		AX = temp.res;*/
	}
	else if (temp.Operat == "/") //除法
	{
		if (lastOper == "para_n")
		{
			addfile << "	ASSUME	CS:" << namem << ",DS:" << namem << endl;
			addfile << "                PUSH  " << "AX" << endl;
			addfile << "                PUSH  " << "BX" << endl;
			addfile << "                PUSH  " << "CX" << endl;
			addfile << "                PUSH  " << "DX" << endl;
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",AX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",BX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",CX" << endl;
				param.pop();
			}
			if (!(param.empty()))
			{
				addfile << "	MOV	" << param.front().name << ",DX" << endl;
				param.pop();
			}
		}
		if (AX.name == temp.opA.name)
		{
			if (AX.active)
				addfile << "	MOV	" << AX.name << ",AX" << endl;
			if (DX.active)
				addfile << "	MOV	" << DX.name << ",DX" << endl;
			addfile << "	CWD	" << endl;
			addfile << "	IDIV	" << temp.opB.name << endl;
			AX = temp.res;
		}
		/*if (AX.active && AX.name != "")
			addfile << "	MOV	" << AX.name << ",AX" << endl;
		addfile << "	CWD	"  << endl;
		addfile << "	DIV	" << temp.opB.name << endl;
		AX = temp.res;*/
	}
	else if (temp.Operat == "&&") //逻辑与
	{
		if (AX.name == temp.opA.name || AX.name == temp.opB.name || BX.name == temp.opA.name || BX.name == temp.opB.name || CX.name == temp.opA.name || CX.name == temp.opB.name || DX.name == temp.opA.name || DX.name == temp.opB.name)
		{
			if (AX.name == temp.opA.name && !(AX.active))
			{
				addfile << "	AND	" << "AX," << temp.opB.name << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opA.name && AX.active)
			{
				addfile << "	MOV	" << AX.name << ",AX" << endl;
				addfile << "	AND	" << "AX," << temp.opB.name << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opB.name && !(AX.active))
			{
				addfile << "	AND	" << "AX," << temp.opA.name << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opB.name && AX.active)
			{
				addfile << "	MOV	" << AX.name << ",AX" << endl;
				addfile << "	AND	" << "AX," << temp.opA.name << endl;
				AX = temp.res;
			}
			else if (BX.name == temp.opA.name && !(BX.active))
			{
				addfile << "	AND	" << "BX," << temp.opB.name << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opA.name && BX.active)
			{
				addfile << "	MOV	" << BX.name << ",BX" << endl;
				addfile << "	AND	" << "BX," << temp.opB.name << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opB.name && BX.active)
			{
				addfile << "	MOV	" << BX.name << ",BX" << endl;
				addfile << "	AND	" << "BX," << temp.opA.name << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opB.name && !(BX.active))
			{
				addfile << "	AND	" << "BX," << temp.opA.name << endl;
				BX = temp.res;
			}
			else if (CX.name == temp.opA.name && !(CX.active))
			{
				addfile << "	AND	" << "CX," << temp.opB.name << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opA.name && CX.active)
			{
				addfile << "	MOV	" << CX.name << ",CX" << endl;
				addfile << "	AND	" << "CX," << temp.opB.name << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opB.name && CX.active)
			{
				addfile << "	MOV	" << CX.name << ",CX" << endl;
				addfile << "	AND	" << "CX," << temp.opA.name << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opB.name && !(CX.active))
			{
				addfile << "	AND	" << "CX," << temp.opA.name << endl;
				CX = temp.res;
			}
			else if (DX.name == temp.opA.name && !(DX.active))
			{
				addfile << "	AND	" << "DX," << temp.opB.name << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opA.name && DX.active)
			{
				addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	AND	" << "DX," << temp.opB.name << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opB.name && DX.active)
			{
				addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	AND	" << "DX," << temp.opA.name << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opB.name && !(DX.active))
			{
				addfile << "	AND	" << "DX," << temp.opA.name << endl;
				DX = temp.res;
			}
		}
		else if (!(AX.active))
		{
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	AND	" << "AX," << temp.opB.name << endl;
			AX = temp.res;
		}
		else if (!(BX.active))
		{
			addfile << "	MOV	" << "BX," << temp.opA.name << endl;
			addfile << "	AND	" << "BX," << temp.opB.name << endl;
			BX = temp.res;
		}
		else if (!(CX.active))
		{
			addfile << "	MOV	" << "CX," << temp.opA.name << endl;
			addfile << "	AND	" << "CX," << temp.opB.name << endl;
			CX = temp.res;
		}
		else if (!(DX.active))
		{
			addfile << "	MOV	" << "DX," << temp.opA.name << endl;
			addfile << "	AND	" << "DX," << temp.opB.name << endl;
			DX = temp.res;
		}
		else
		{
			addfile << "	MOV	" << AX.name << ",AX" << endl;
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	AND	" << "AX," << temp.opB.name << endl;
			AX = temp.res;
		}
		/*addfile << "	MOV	" << "AX," << temp.opA.name << endl;
		addfile << "	AND	" << "AX," << temp.opB.name << endl;*/

	}
	else if (temp.Operat == "||") //逻辑或
	{
		if (AX.name == temp.opA.name || AX.name == temp.opB.name || BX.name == temp.opA.name || BX.name == temp.opB.name || CX.name == temp.opA.name || CX.name == temp.opB.name || DX.name == temp.opA.name || DX.name == temp.opB.name)
		{
			if (AX.name == temp.opA.name && !(AX.active))
			{
				addfile << "	OR	" << "AX," << temp.opB.name << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opA.name && AX.active)
			{
				addfile << "	MOV	" << AX.name << ",AX" << endl;
				addfile << "	OR	" << "AX," << temp.opB.name << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opB.name && !(AX.active))
			{
				addfile << "	OR	" << "AX," << temp.opA.name << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opB.name && AX.active)
			{
				addfile << "	MOV	" << AX.name << ",AX" << endl;
				addfile << "	OR	" << "AX," << temp.opA.name << endl;
				AX = temp.res;
			}
			else if (BX.name == temp.opA.name && !(BX.active))
			{
				addfile << "	OR	" << "BX," << temp.opB.name << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opA.name && BX.active)
			{
				addfile << "	MOV	" << BX.name << ",BX" << endl;
				addfile << "	OR	" << "BX," << temp.opB.name << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opB.name && BX.active)
			{
				addfile << "	MOV	" << BX.name << ",BX" << endl;
				addfile << "	OR	" << "BX," << temp.opA.name << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opB.name && !(BX.active))
			{
				addfile << "	OR	" << "BX," << temp.opA.name << endl;
				BX = temp.res;
			}
			else if (CX.name == temp.opA.name && !(CX.active))
			{
				addfile << "	OR	" << "CX," << temp.opB.name << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opA.name && CX.active)
			{
				addfile << "	MOV	" << CX.name << ",CX" << endl;
				addfile << "	OR	" << "CX," << temp.opB.name << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opB.name && CX.active)
			{
				addfile << "	MOV	" << CX.name << ",CX" << endl;
				addfile << "	OR	" << "CX," << temp.opA.name << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opB.name && !(CX.active))
			{
				addfile << "	OR	" << "CX," << temp.opA.name << endl;
				CX = temp.res;
			}
			else if (DX.name == temp.opA.name && !(DX.active))
			{
				addfile << "	OR	" << "DX," << temp.opB.name << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opA.name && DX.active)
			{
				addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	OR	" << "DX," << temp.opB.name << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opB.name && DX.active)
			{
				addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	OR	" << "DX," << temp.opA.name << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opB.name && !(DX.active))
			{
				addfile << "	OR	" << "DX," << temp.opA.name << endl;
				DX = temp.res;
			}
		}
		else if (!(AX.active))
		{
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	OR	" << "AX," << temp.opB.name << endl;
			AX = temp.res;
		}
		else if (!(BX.active))
		{
			addfile << "	MOV	" << "BX," << temp.opA.name << endl;
			addfile << "	OR	" << "BX," << temp.opB.name << endl;
			BX = temp.res;
		}
		else if (!(CX.active))
		{
			addfile << "	MOV	" << "CX," << temp.opA.name << endl;
			addfile << "	OR	" << "CX," << temp.opB.name << endl;
			CX = temp.res;
		}
		else if (!(DX.active))
		{
			addfile << "	MOV	" << "DX," << temp.opA.name << endl;
			addfile << "	OR	" << "DX," << temp.opB.name << endl;
			DX = temp.res;
		}
		else
		{
			addfile << "	MOV	" << AX.name << ",AX" << endl;
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	OR	" << "AX," << temp.opB.name << endl;
			AX = temp.res;
		}
	}
	else if (temp.Operat == "!=" || temp.Operat == "==" || temp.Operat == "<" || temp.Operat == "<=" || temp.Operat == ">" || temp.Operat == ">=") //不等于，判断相等，大于等于，大于，小于等于，小于
	{
		if (AX.name == temp.opA.name)
		{
			addfile << "	CMP	" << "AX," << temp.opB.name << endl;
		}
		else if (BX.name == temp.opA.name)
		{
			addfile << "	CMP	" << "BX," << temp.opB.name << endl;
		}
		else if (CX.name == temp.opA.name)
		{
			addfile << "	CMP	" << "CX," << temp.opB.name << endl;
		}
		else if (DX.name == temp.opA.name)
		{
			addfile << "	CMP	" << "DX," << temp.opB.name << endl;
		}
		else if (!(AX.active))
		{
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	CMP	" << "AX," << temp.opB.name << endl;
			AX = temp.opA;
		}
		else if (!(BX.active))
		{
			addfile << "	MOV	" << "BX," << temp.opA.name << endl;
			addfile << "	CMP	" << "BX," << temp.opB.name << endl;
			BX = temp.opA;
		}
		else if (!(CX.active))
		{
			addfile << "	MOV	" << "CX," << temp.opA.name << endl;
			addfile << "	CMP	" << "CX," << temp.opB.name << endl;
			CX = temp.opA;
		}
		else if (!(DX.active))
		{
			addfile << "	MOV	" << "DX," << temp.opA.name << endl;
			addfile << "	CMP	" << "DX," << temp.opB.name << endl;
			DX = temp.opA;
		}
		else
		{
			addfile << "	MOV	" << AX.name << ",AX" << endl;
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	CMP	" << "AX," << temp.opB.name << endl;
			AX = temp.opA;
		}
		/*if (AX.active && AX.name != "")
			addfile << "	MOV	" << AX.name << ",AX" << endl;
		addfile << "	CMP	" << "AX," << temp.opB.name << endl;*/
	}
	else if (temp.Operat == "!") //非
	{
		if (AX.name == temp.opA.name || BX.name == temp.opA.name || CX.name == temp.opA.name || DX.name == temp.opA.name)
		{
			if (AX.name == temp.opA.name && !(AX.active))
			{
				addfile << "	NOT	" << "AX" << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opA.name && AX.active)
			{
				addfile << "	MOV	" << AX.name << ",AX" << endl;
				addfile << "	NOT	" << "AX" << endl;
				AX = temp.res;
			}
			else if (BX.name == temp.opA.name && !(BX.active))
			{
				addfile << "	NOT	" << "BX" << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opA.name && BX.active)
			{
				addfile << "	MOV	" << BX.name << ",BX" << endl;
				addfile << "	NOT	" << "BX" << endl;
				BX = temp.res;
			}
			else if (CX.name == temp.opA.name && !(CX.active))
			{
				addfile << "	NOT	" << "CX" << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opA.name && CX.active)
			{
				addfile << "	MOV	" << CX.name << ",CX" << endl;
				addfile << "	NOT	" << "CX" << endl;
				CX = temp.res;
			}
			else if (DX.name == temp.opA.name && !(DX.active))
			{
				addfile << "	NOT	" << "DX" << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opA.name && DX.active)
			{
				addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	NOT	" << "DX" << endl;
				DX = temp.res;
			}
		}
		else if (!(AX.active))
		{
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	NOT	" << "AX" << endl;
			AX = temp.res;
		}
		else if (!(BX.active))
		{
			addfile << "	MOV	" << "BX," << temp.opA.name << endl;
			addfile << "	NOT	" << "BX" << endl;
			BX = temp.res;
		}
		else if (!(CX.active))
		{
			addfile << "	MOV	" << "CX," << temp.opA.name << endl;
			addfile << "	NOT	" << "CX" << endl;
			CX = temp.res;
		}
		else if (!(DX.active))
		{
			addfile << "	MOV	" << "DX," << temp.opA.name << endl;
			addfile << "	NOT	" << "DX" << endl;
			DX = temp.res;
		}
		else
		{
			addfile << "	MOV	" << AX.name << ",AX" << endl;
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	NOT	" << "AX" << endl;
			AX = temp.res;
		}
	}
	else if (temp.Operat == "-" && (temp.opB.name == ""))
	{
		if (AX.name == temp.opA.name || BX.name == temp.opA.name || CX.name == temp.opA.name || DX.name == temp.opA.name)
		{
			if (AX.name == temp.opA.name && !(AX.active))
			{
				addfile << "	NEG	" << "AX" << endl;
				AX = temp.res;
			}
			else if (AX.name == temp.opA.name && AX.active)
			{
				addfile << "	MOV	" << AX.name << ",AX" << endl;
				addfile << "	NEG	" << "AX" << endl;
				AX = temp.res;
			}
			else if (BX.name == temp.opA.name && !(BX.active))
			{
				addfile << "	NEG	" << "BX" << endl;
				BX = temp.res;
			}
			else if (BX.name == temp.opA.name && BX.active)
			{
				addfile << "	MOV	" << BX.name << ",BX" << endl;
				addfile << "	NEG	" << "BX" << endl;
				BX = temp.res;
			}
			else if (CX.name == temp.opA.name && !(CX.active))
			{
				addfile << "	NEG	" << "CX" << endl;
				CX = temp.res;
			}
			else if (CX.name == temp.opA.name && CX.active)
			{
				addfile << "	MOV	" << CX.name << ",CX" << endl;
				addfile << "	NEG	" << "CX" << endl;
				CX = temp.res;
			}
			else if (DX.name == temp.opA.name && !(DX.active))
			{
				addfile << "	NEG	" << "DX" << endl;
				DX = temp.res;
			}
			else if (DX.name == temp.opA.name && DX.active)
			{
				addfile << "	MOV	" << DX.name << ",DX" << endl;
				addfile << "	NEG	" << "DX" << endl;
				DX = temp.res;
			}
		}
		else if (!(AX.active))
		{
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	NEG	" << "AX" << endl;
			AX = temp.res;
		}
		else if (!(BX.active))
		{
			addfile << "	MOV	" << "BX," << temp.opA.name << endl;
			addfile << "	NEG	" << "BX" << endl;
			BX = temp.res;
		}
		else if (!(CX.active))
		{
			addfile << "	MOV	" << "CX," << temp.opA.name << endl;
			addfile << "	NEG	" << "CX" << endl;
			CX = temp.res;
		}
		else if (!(DX.active))
		{
			addfile << "	MOV	" << "DX," << temp.opA.name << endl;
			addfile << "	NEG	" << "DX" << endl;
			DX = temp.res;
		}
		else
		{
			addfile << "	MOV	" << AX.name << ",AX" << endl;
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			addfile << "	NEG	" << "AX" << endl;
			AX = temp.res;
		}
	}
	else if (temp.Operat == "if") //if语句
	{
		if (lastOper == "<")
			addfile << "	JGE	" << "QT" << temp.res.name << endl;
		else if (lastOper == "<=")
			addfile << "	JG	" << "QT" << temp.res.name << endl;
		else if (lastOper == ">")
			addfile << "	JLE	" << "QT" << temp.res.name << endl;
		else if (lastOper == ">=")
			addfile << "	JL	" << "QT" << temp.res.name << endl;
	}
	else if (temp.Operat == "el") //else语句
	{
		addfile << "	JMP	" << "QT" << temp.res.name << endl;
		addfile << "QT" << i + 1 << ":";
	}
	else if (temp.Operat == "ie") //ie结构尾部
	{
		addfile << "QT" << i << ":";
	}
	else if (temp.Operat == "wh") //while语句
	{
		addfile << "QT" << i << ":";
	}
	else if (temp.Operat == "do") //do语句，循环部分
	{
		if (lastOper == "<")
			addfile << "	JGE	" << "QT" << temp.res.name << endl;
		else if (lastOper == "<=")
			addfile << "	JG	" << "QT" << temp.res.name << endl;
		else if (lastOper == ">")
			addfile << "	JLE	" << "QT" << temp.res.name << endl;
		else if (lastOper == ">=")
			addfile << "	JL	" << "QT" << temp.res.name << endl;
	}
	else if (temp.Operat == "we") //while结构尾部
	{
		addfile << "	JMP	" << "QT" << temp.res.name << endl;
		addfile << "QT" << i << ":";
	}
	else if (temp.Operat == "FUNC")//函数，最后一个操作数为函数名
	{
	addfile << temp.res.name << "      PROC" << endl;
	namem = temp.res.name;
	}
	else if (temp.Operat == "para_n")//参数
	{
		param.push(temp.opA);
		addfile << "	" << temp.opA.name << "	DB	4 DUP(0)" << endl;
	}
	else if (temp.Operat == "RET")//参数
	{
		addfile << "	    MOV	" << "AX," << temp.opA.name << endl;
		addfile << "            POP  " << "DX" << endl;
		addfile << "            POP  " << "CX" << endl;
		addfile << "            POP  " << "BX" << endl;
		addfile << "    RET  " << endl;
		addfile << namem << "    ENDP  " << endl;
		RETm.push_back(temp.opA);
	}

	else if (temp.Operat == "para")//参数
	{
		/*if (!(param.empty()))
		{
			 if(AX.active)
				 addfile << "	MOV	" << AX.name << ",AX" << endl;
			 addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			 addfile << "	MOV	" << param.front().name <<  ",AX" << endl;
			 param.pop();
		}*/
		if (ppp == 1)
		{
			if (AX.active)
				addfile << "	MOV	" << AX.name << ",AX" << endl;
			addfile << "	MOV	" << "AX," << temp.opA.name << endl;
			ppp++;
		}
		else if (ppp == 2)
		{
			if (BX.active)
				addfile << "	MOV	" << BX.name << ",BX" << endl;
			addfile << "	MOV	" << "BX," << temp.opA.name << endl;
			ppp++;
		}
		else if (ppp == 3)
		{
			if (BX.active)
				addfile << "	MOV	" << CX.name << ",CX" << endl;
			addfile << "	MOV	" << "CX," << temp.opA.name << endl;
			ppp++;
		}
		else if (ppp == 4)
		{
			if (BX.active)
				addfile << "	MOV	" << DX.name << ",DX" << endl;
			addfile << "	MOV	" << "DX," << temp.opA.name << endl;
			ppp++;
		}
	}
	else if (temp.Operat == "call")//函数调用
	{
		ppp = 0;
		addfile << "    CALL  " << temp.opA.name << endl;
		/*int bbb = 1;
		vector<token> ::iterator aaa;
		for (aaa = param.begin(); aaa != param.end(); aaa++,bbb++)
		{
			if (temp.opA.name != ""&&bbb==1)
			{
				addfile << "    MOV  " <<  "AX," << temp.opA.name << endl;
				addfile << "    MOV  " << aaa->name << ",AX"<< endl;
			}
			if (temp.opB.name != "" && bbb == 2)
			{
				addfile << "    MOV  " << "AX," << temp.opB.name << endl;
				addfile << "    MOV  " << aaa->name << ",AX" << endl;
			}
			if (temp.res.name != "" && bbb == 3)
			{
				addfile << "    MOV  " << "AX," << temp.res.name << endl;
				addfile << "    MOV  " << aaa->name << ",AX" << endl;
			}
		}*/
	}
	/*else if (temp.Operat == "RET")//函数返回
	{
		if(AX.active)
			addfile << "	MOV	" << AX.name << ",AX" << endl;
		if (BX.active)
			addfile << "	MOV	" << BX.name << ",BX" << endl;
		if (CX.active)
			addfile << "	MOV	" << CX.name << ",CX" << endl;
		if (DX.active)
			addfile << "	MOV	" << DX.name << ",DX" << endl;
		addfile << "    POP  " << "DX" << endl;
		addfile << "    POP  " << "CX" << endl;
		addfile << "    POP  " << "BX" << endl;
		addfile << "    RET  "<< endl;
	}*/
	/*else if (temp.Operat == "END")//函数结束
	{
		addfile << temp.opA.name <<"    ENDP  " << endl;
	}*/
}