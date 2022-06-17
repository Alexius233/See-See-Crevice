#include "DAG.h"

using namespace std;

//vector<QT>qt; //原本的一个基本块内的四元式组
vector<QT>groupQT; //存放优化后的一个基本块内的四元式
vector<QT>resQT; //存放最终优化后的所有四元式
vector<node>DAG;
vector<token>SYMBL;
vector<token>SYMBL_ALL;
QT specialQT;

int searchNode(string idtemp) //根据标识符寻找到结点标号
{
	vector<node>::iterator it;
	for (it = DAG.begin(); it != DAG.end(); it++)
	{
		if (searchMark(idtemp, it->num) != -1) //找到了
		{
			return it->num;
		}
	}
	return -1;//没找到
}

int searchNode(string tempOper, string tempB, string tempC) //根据操作符和操作数B，C查找结点标号（双目运算符）
{
	vector<node>::iterator it;
	for (it = DAG.begin(); it != DAG.end(); it++)
	{
		if (it->Operat == tempOper)
		{
			int i = it->left;
			int j = it->right;
			if (searchMark(tempB, i) != -1 && searchMark(tempC, j) != -1) //操作符和操作数都对应正确
			{
				return it->num;
			}
		}
	}
	return -1; //没找到
}

int searchNode(string tempOper, string tempB) //根据操作符和操作数B查找结点标号（单目运算符）
{
	vector<node>::iterator it;
	for (it = DAG.begin(); it != DAG.end(); it++)
	{
		if (it->Operat == tempOper && it->right == -1) //操作符相同且为单目操作符
		{
			int i = it->left;
			if (searchMark(tempB, i) != -1) //操作符和操作数都对应正确
			{
				return it->num;
			}
		}
	}
	return -1;
}

int searchMark(string marktemp, int nodeNum) //在确定NODE内遍历标记找出是否存在此标记
{
	if (marktemp != "")
	{
		for (int i = 0; i < 20; i++)
		{
			if (DAG[nodeNum].Mark[i].name == marktemp)
				return i;
		}
	}
	return -1;//没找到
}

bool searchActive(string temp) //在SYMBL表中根据标识符返回活跃信息
{
	vector<token>::iterator tokent;
	if (temp == "")
		return false;
	for (tokent = SYMBL.begin(); tokent != SYMBL.end(); tokent++)
	{
		if (tokent->name == temp)
		{
			return tokent->active;
		}
	}
	return false;
}

void deleteMark(string nameM, int n) //删除除了第N号结点以外的附标记
{
	int j;
	vector<node>::iterator delMark;
	for (delMark = DAG.begin(); delMark != DAG.end(); delMark++)
	{
		if (delMark->num != n) //不是N号结点
		{
			j = searchMark(nameM, delMark->num);
			if (j != -1 && j != 0) //主标记免删
			{
				DAG[delMark->num].Mark[j].name = "";
				DAG[delMark->num].Mark[j].type = 0;
			}
		}
	}
}

int judgeOperate(string t) //返回操作符的类型
{
	if (t[0] == '=')
	{
		return 0;
	}
	else if (t[0] == '+' || t[0] == '-' || t[0] == '*' || t[0] == '/')
	{
		return 1;
	}
	else if (t[0] == '!' || t[0] == '<' || t[0] == '>' || t == "!=" || t == "==" || t == ">=" || t == "<=")
	{
		return 2;
	}
	else return 3; //特殊四元式
}

string calculate(string tempOper, string tempB, string tempC)   //计算两个数运算的值
{
	char a;
	char s[10];
	float v = 0;
	a = tempOper[0];
	switch (a)
	{
	case '+':
		v = (float)atof(tempB.c_str()) + (float)atof(tempC.c_str());
		break;
	case '-':
		v = (float)atof(tempB.c_str()) - (float)atof(tempC.c_str());
		break;
	case '*':
		v = (float)atof(tempB.c_str()) * (float)atof(tempC.c_str());
		break;
	case '/':
		v = (float)atof(tempB.c_str()) / (float)atof(tempC.c_str());
		break;
	}
	sprintf_s(s, "%f", v);
	string p(s);
	return p;
}

void attachNode(int number, token tempA) //把标记A附着到结点上
{
	int i; //存储附着到的MARK的序号
	for (i = 0; i < 20; i++) //把A附加到B上
	{
		if (DAG[number].Mark[i].name == "")
		{
			DAG[number].Mark[i].name = tempA.name;
			DAG[number].Mark[i].type = tempA.type;
			break;
		}
	}
}

void swapMark(node &temp) //标记互换
{
	token swap;
	for (int i = 1; i < 20; i++)
	{
		if (temp.Mark[i].type != 0 && temp.Mark[0].type > temp.Mark[i].type) 	//互换
		{
			swap = temp.Mark[0];
			temp.Mark[0] = temp.Mark[i];
			temp.Mark[i] = swap;
		}
	}
}

QT createQT(string tempOper, token opA, token opB, token res) //创建双目运算符四元式
{
	QT temp;
	temp.Operat = tempOper;
	temp.opA.active = opA.active; temp.opA.name = opA.name; temp.opA.type = opA.type;
	temp.opB.active = opB.active; temp.opB.name = opB.name; temp.opB.type = opB.type;
	temp.res.active = res.active; temp.res.name = res.name; temp.res.type = res.type;
	return temp;
}

QT createQT(string tempOper, token opA, token res) //创建单目运算符四元式
{
	QT temp;
	temp.Operat = tempOper;
	temp.opB.clear();
	temp.opA.active = opA.active; temp.opA.name = opA.name; temp.opA.type = opA.type;
	temp.res.active = res.active; temp.res.name = res.name; temp.res.type = res.type;
	return temp;
}

int createNode()//建立新结点，返回最后一个结点的标号
{
	node temp;
	temp.left = -1;
	temp.right = -1;
	temp.Operat = "";
	DAG.push_back(temp);
	int number = (int)DAG.size() - 1;
	DAG[number].num = number;
	return number;
}

void createDAG(vector<QT>block_qt) //根据给定的四元式组block_qt构造优化的DAG，原本的block_qt不改变，而生成一个DAG
{
	DAG.clear();
	vector<QT>::iterator QTit;
	QT QTtemp;
	specialQT.clear();
	for (QTit = block_qt.begin(); QTit != block_qt.end(); QTit++)//遍历一整个基本块
	{
		QTtemp = *QTit;
		int numB, numC;
		numB = searchNode(QTtemp.opA.name);
		numC = searchNode(QTtemp.opB.name);
		if (numB == -1) //没有B这个结点则创建
		{
			numB = createNode();
			DAG[numB].Mark[0].name = QTtemp.opA.name;
			DAG[numB].Mark[0].type = QTtemp.opA.type;
		}
		if (numC == -1) //没有C这个结点则创建
		{
			numC = createNode();
			DAG[numC].Mark[0].name = QTtemp.opB.name;
			DAG[numC].Mark[0].type = QTtemp.opB.type;
		}
		int judgetemp = judgeOperate(QTtemp.Operat);
		if (judgetemp == 0 && QTtemp.opB.name == "") //如果是赋值四元式A=B，B可能是常值
		{
			if (QTtemp.opA.type == 1) //如果是常值表达式A=C
			{
				int i;
				i = stringToNum<int>(QTtemp.opA.name);
				QTtemp.opA.name = to_string(i);
				string tempC = QTtemp.opA.name;
				int k = searchNode(tempC); //寻找C是否已经定义过,用于存放C所在的结点标号
				int l = searchNode(QTtemp.res.name); //寻找A是否已经定义过
				if (k != -1) //C已经定义过
				{
					attachNode(k, QTtemp.res); //把A附着于C上
				}
				else //C没定义过
				{
					int i = createNode();
					DAG[i].Mark[0].name = tempC;
					DAG[i].Mark[0].type = 1;
					DAG[i].Mark[1].name = QTtemp.res.name;
					DAG[i].Mark[1].type = QTtemp.res.type;
					DAG[i].num = i; //创建新结点并初始化
					k = i;
				}
				if (l != -1)//A在N2已经定义过
				{
					deleteMark(QTtemp.res.name, k);
				}
			}
			else //是赋值四元式A=B
			{
				int l = searchNode(QTtemp.res.name); //寻找A是否已经定义过
				attachNode(numB, QTtemp.res); //把A附加在B上
				if (l != -1)//A在N2已经定义过
				{
					deleteMark(QTtemp.res.name, numB);
				}
			}
		}
		else if (judgetemp != 3 && judgetemp != 0) //是1，2类型的表达式
		{
			if (QTtemp.opA.type == 1 && QTtemp.opB.type == 1)//是常值表达式A=C1ωC2
			{
				int i;
				i = stringToNum<int>(QTtemp.opA.name);
				QTtemp.opA.name = to_string(i);
				i = stringToNum<int>(QTtemp.opB.name);
				QTtemp.opB.name = to_string(i);
				string tempC = calculate(QTtemp.Operat, QTtemp.opA.name, QTtemp.opB.name); //计算常值C=C1ωC2
				int k = searchNode(tempC); //寻找C是否已经定义过
				int l = searchNode(QTtemp.res.name); //寻找A是否已经定义过
				if (k != -1) //C定义过
				{
					attachNode(k, QTtemp.res);
				}
				else //C未定义过，申请新结点,并把A附着在C上
				{
					int i = createNode();
					DAG[i].Mark[0].name = tempC;
					DAG[i].Mark[0].type = 1;
					DAG[i].Operat = "";
					DAG[i].Mark[1].name = QTtemp.res.name;
					DAG[i].Mark[1].type = QTtemp.res.type;
					DAG[i].num = i; //创建新结点并初始化
					k = i;
				}
				if (l != -1)
				{
					deleteMark(QTtemp.res.name, k);
				}
			}
			else if (QTtemp.opB.name == "") //A=ωB
			{
				int i = searchNode(QTtemp.Operat, QTtemp.opA.name);
				if (i != -1) //已经存在公共表达式ωB
				{
					attachNode(i, QTtemp.res);
				}
				else //不存在公共表达式
				{
					int j = createNode();
					DAG[j].left = numB;
					DAG[j].Mark[0].name = QTtemp.res.name;
					DAG[j].Mark[0].type = QTtemp.res.type;
					DAG[j].Operat = QTtemp.Operat;
					DAG[j].num = j; //创建新结点并初始化
					i = j;
				}
				deleteMark(QTtemp.res.name, i);
			}
			else //A=BωC
			{
				int i = searchNode(QTtemp.Operat, QTtemp.opA.name, QTtemp.opB.name);
				if (i != -1) //已经存在公共表达式BωC
				{
					attachNode(i, QTtemp.res);
				}
				else //不存在公共表达式
				{
					int j = createNode();
					DAG[j].left = numB;
					DAG[j].right = numC;
					DAG[j].Mark[0].name = QTtemp.res.name;
					DAG[j].Mark[0].type = QTtemp.res.type;
					DAG[j].Operat = QTtemp.Operat;
					DAG[j].num = j; //创建新结点并初始化
					i = j;
				}
				deleteMark(QTtemp.res.name, i);
			}
		}
		else if (judgetemp == 3) //特殊四元式
		{
			specialQT = QTtemp;
		}
	}
	vector<node>::iterator it;
	for (it = DAG.begin(); it != DAG.end(); it++) //遍历DAG结点交换标记
	{
		swapMark(*it);
	}
}

void createQT() //根据优化的DAG，重组四元式C
{
	//需要增加功能，将类似于A与a这样的变量要改变其标识符用于在生成目标代码时区分变量
	groupQT.clear();
	vector<node>::iterator itdag;
	QT temp;
	for (itdag = DAG.begin(); itdag != DAG.end(); itdag++)
	{
		if (itdag->Operat == "") //是叶结点
		{
			for (int i = 1; i < 20; i++) //Ai为非临时变量则生成Ai=B
			{
				if (itdag->Mark[i].type == 2) //为非临时变量
				{
					temp.Operat = "=";
					temp.opA = itdag->Mark[0];
					temp.res = itdag->Mark[i];
					groupQT.push_back(temp);
					temp.clear();
				}
			}
		}
		else //非叶结点
		{
			if (DAG[itdag->right].Mark[0].name == "")//单目运算符
			{
				temp.Operat = itdag->Operat;
				temp.opA = DAG[itdag->left].Mark[0]; //以主标记参与运算
				temp.res = itdag->Mark[0];
				groupQT.push_back(temp);
				temp.clear();
				for (int i = 1; i < 20; i++) //Ai为非临时变量则生成Ai=A
				{
					if (itdag->Mark[i].type == 2) //为非临时变量
					{
						temp.Operat = "=";
						temp.opA = itdag->Mark[0];
						temp.res = itdag->Mark[i];
						groupQT.push_back(temp);
						temp.clear();
					}
				}
			}
			else //双目运算符
			{
				temp.Operat = itdag->Operat;
				temp.opA = DAG[itdag->left].Mark[0]; //以主标记参与运算
				temp.opB = DAG[itdag->right].Mark[0];
				temp.res = itdag->Mark[0];
				groupQT.push_back(temp);
				temp.clear();
				for (int i = 1; i < 20; i++) //Ai为非临时变量则生成Ai=A
				{
					if (itdag->Mark[i].type == 2) //为非临时变量
					{
						temp.Operat = "=";
						temp.opA = itdag->Mark[0];
						temp.res = itdag->Mark[i];
						groupQT.push_back(temp);
						temp.clear();
					}
				}
			}
		}
	}
	if (specialQT.Operat != "")
	{
		groupQT.push_back(specialQT);
		int i = specialQT.block;
		vector<QT>::iterator it;
		for (it = groupQT.begin(); it != groupQT.end(); it++)
		{
			it->block = i;
		}
	}
}

//void cleanDAG() //害clean啥啊，老出问题不clean了
//{
//	vector<node>::iterator DAGit;
//	for (DAGit = DAG.begin(); DAGit != DAG.end(); )
//	{
//		if (DAGit->Mark[0].name == "" && DAGit->Operat == "" && DAGit->left == -1 && DAGit->right == -1)
//		{
//			DAGit = DAG.erase(DAGit);
//			if (DAGit == DAG.end()) break;
//			DAGit->num--;
//		}
//		else
//		{
//			DAGit++;
//		}
//	}
//}

void printQT(vector<QT>tempQT) //打印四元式组
{
	vector<QT>::iterator it;
	for (it = tempQT.begin(); it != tempQT.end(); it++)
	{
		if (it->opA.name == "")
		{
			cout << "( " << it->Operat << " , " << " _ " << " , " << " _ " << " , " << " _ " << " )" << endl;
		}
		else if (it->opB.name == "")
		{
			if (it->Operat == "")
			{
				cout << "( " << " = " << " , " << it->opA.name << " , " << " _ " << " , " << it->res.name << " )" << endl;

			}
			else
			{
				cout << "( " << it->Operat << " , " << it->opA.name << " , " << " _ " << " , " << it->res.name << " )" << endl;
			}
		}
		else
		{
			cout << "( " << it->Operat << " , " << it->opA.name << " , " << it->opB.name << " , " << it->res.name << " )" << endl;
		}
	}
}

void initQT(vector<QT> & tempQT) //测试用构造初始四元式组
{
	tempQT.clear();
	tempQT.push_back({ "=",{"1",1,false},{"",0,false},{"A",2,false} }); //A=1
	tempQT.push_back({ "=",{"2",1,false},{"",0,false},{"B",2,false} }); //B=2

	//tempQT.push_back({ "=",{"5",1,false},{"",0,false},{"B",2,false} }); //B=5
	//tempQT.push_back({ "*",{"2",1,false},{"3.14",1,false},{"t1",3,false} }); //t1=2*3.14
	//tempQT.push_back({ "+",{"R",2,false},{"r",2,false},{"t2",3,false} }); //t2=R+r
	//tempQT.push_back({ "*",{"t1",3,false},{"t2",3,false},{"t3",3,false} }); //t3=t1*t2
	//tempQT.push_back({ "=",{"t3",3,false},{"",0,false},{"A",2,false} }); //A=t3
	//tempQT.push_back({ "*",{"2",1,false},{"3.14",1,false},{"t4",3,false} }); //t4=2*3.14
	//tempQT.push_back({ "+",{"R",2,false},{"r",2,false},{"t5",3,false} }); //t5=R+r
	//tempQT.push_back({ "*",{"t4",3,false},{"t5",3,false},{"t6",3,false} }); //t6=t4*t5
	//tempQT.push_back({ "-",{"R",2,false},{"r",2,false},{"t7",3,false} }); //t7=R-r
	//tempQT.push_back({ "/",{"t6",3,false},{"t7",3,false},{"t8",3,false} }); //t8=t6/t7
	//tempQT.push_back({ "=",{"t8",3,false},{"",0,false},{"B",2,false} }); //B=t8

}

//int main()
//{
//	//initQT(qt);
//	//QTit = qt.begin();
//	//createDAG();
//	//createQT();
//	//printQT(groupQT);
//	return 0;
//}

void createSYMBL() //遍历优化后的QT组，生成活跃信息表
{
	vector<QT>::iterator it;
	SYMBL.clear();
	for (it = groupQT.begin(); it != groupQT.end(); it++)
	{
		int i = -1;
		if ((i = searchSYMBL(it->opA.name)) == -1) //未填入A操作符则填，已经填入则不改动
		{
			if (it->opA.type == 2) //为非临时变量
			{
				SYMBL.push_back({ it->opA.name,it->opA.type,true });
				it->opA.active = true;
			}
			else if (it->opA.type == 3) //为临时变量
				SYMBL.push_back(it->opA);
		}
		if ((i = searchSYMBL(it->opB.name)) == -1) //未填入B操作符则填，已经填入则不改动
		{
			if (it->opB.type == 2) //为非临时变量
			{
				SYMBL.push_back({ it->opB.name,it->opB.type,true });
				it->opB.active = true;
			}
			else if (it->opB.type == 3) //为临时变量
				SYMBL.push_back(it->opB);
		}
		if ((i = searchSYMBL(it->res.name)) == -1) //未填入B操作符则填，已经填入则不改动
		{
			if (it->res.type == 2) //为非临时变量
			{
				SYMBL.push_back({ it->res.name,it->res.type,true });
				it->res.active = true;
			}
			else if (it->res.type == 3) //为临时变量
				SYMBL.push_back(it->res);
		}
	}
}

void createRes() //遍历groupQT，把四元式加入进resQT
{
	vector<QT>::iterator it;
	for (it = groupQT.begin(); it != groupQT.end(); it++)
	{
		resQT.push_back(*it);
	}
}

int searchSYMBL(string temp) //在SYMBL表中搜索该标识符是否存在
{
	vector<token>::iterator tokent;
	int i = 0;
	if (temp == "")
	{
		return -1;
	}
	for (tokent = SYMBL.begin(); tokent != SYMBL.end(); tokent++, i++)
	{
		if (tokent->name == temp)
		{
			return i;
		}
	}
	return -1;
}

void changeActive() //逆序生成活跃信息
{
	vector<QT>::reverse_iterator it;
	int i = 0;
	for (it = groupQT.rbegin(); it != groupQT.rend(); it++)
	{
		if (it->res.name != "") //操作数res存在,当然也不会为常数
		{
			i = searchSYMBL(it->res.name); //操作数res存在，不可能不存在于SYMBL表中，故不加判断
			if (i != -1 && i < SYMBL.size()) {
				it->res.active = SYMBL[i].active;
				SYMBL[i].active = false;
			}
		}
		if (it->opB.type != 1 && it->opB.name != "") //操作数opB存在,且不为常数
		{
			i = searchSYMBL(it->opB.name);
			if (i != -1 && i < SYMBL.size()) {
				it->opB.active = SYMBL[i].active;
				SYMBL[i].active = true;
			}
		}
		if (it->opA.type != 1 && it->opA.name != "") //操作数opA存在,且不为常数
		{
			i = searchSYMBL(it->opA.name);
			if (i != -1 && i < SYMBL.size()) {
				it->opA.active = SYMBL[i].active;
				SYMBL[i].active = true;
			}
		}
	}

}

void addSYMBL() //填入一个全局的符号表为DSEG段做准备
{
	vector<token>::iterator it;
	vector<token>::iterator itS;
	bool exist = false;
	for (it = SYMBL.begin(); it != SYMBL.end(); it++)
	{
		for (itS = SYMBL_ALL.begin(); itS != SYMBL_ALL.end(); itS++)
		{
			if (it->name != ""&&it->name == itS->name)
			{
				exist = true;
			}
		}
		if (!exist)
			SYMBL_ALL.push_back(*it);
	}
}