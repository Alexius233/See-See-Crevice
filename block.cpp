#include "block.h"
#include <stack>
#include <queue>
#include <fstream>

extern vector<QT>resQT;

void firstview() //第一遍遍历,生成所有优化后的四元式于resQT
{
	vector<QT>::iterator it;//指向qt内容的迭代器
	vector<QT>block_qt;
	resQT.clear();
	int i = 1;
	for (it = qt.begin(); it != qt.end(); it++) //遍历qt
	{
		if (i != it->block) //it为下一个基本块内的四元式
		{
			createDAG(block_qt);
			createQT();
			createSYMBL();
			changeActive();
			addSYMBL(); //填入一个全局的符号表为DSEG段做准备
			createRes(); //生成优化后的四元式组resQT
			i++;
			block_qt.clear();//处理完一个基本块清空
		}
		block_qt.push_back(*it); //取四元式
	}
}

void secondview() //第二遍遍历，填跳转块号
{
	vector<QT>::iterator it; //遍历resQT的迭代器
	stack<token*>ifstack; //if语义栈
	stack<token*>whstack; //while语义栈
	stack<int>whstacknum; //while反填栈
	queue<int>forqueuenum; //for语句语义队列
	stack<int>fornum_level; //for嵌套语义栈
	queue<token*>forqueue; //for语句语义队列
	stack<token*>for_level; //for嵌套语义栈
	int i = 1; //四元式的序号
	for (it = resQT.begin(); it != resQT.end(); i++, it++)
	{
		if (it->Operat == "if")
		{
			ifstack.push(&it->res);
		}
		else if (it->Operat == "el")
		{
			ifstack.top()->name = to_string(i + 1);
			ifstack.pop();
			ifstack.push(&it->res);
		}
		else if (it->Operat == "ie")
		{
			ifstack.top()->name = to_string(i);
			ifstack.pop();
		}
		else if (it->Operat == "wh")
		{
			whstacknum.push(i);
		}
		else if (it->Operat == "do")
		{
			whstack.push(&it->res);
		}
		else if (it->Operat == "we")
		{
			it->res.name = to_string(whstacknum.top());
			whstacknum.pop();
			whstack.top()->name = to_string(i);
			whstack.pop();
		}
		else if (it->Operat == "for")
		{
			if (!forqueue.empty())
			{
				for_level.push(forqueue.front());
				forqueue.pop();
				fornum_level.push(forqueuenum.front());
				forqueuenum.pop();
			}
		}
		else if (it->Operat == "fordo")
		{
			forqueue.push(&it->opB);
			forqueue.push(&it->res);
			forqueuenum.push(i - 1);
			forqueuenum.push(i + 1);
		}
		else if (it->Operat == "jumpdo")
		{
			it->res.name = to_string(forqueuenum.front());
			forqueuenum.pop();
		}
		else if (it->Operat == "dos")
		{
			forqueue.front()->name = to_string(i + 1);
			forqueue.pop();
		}
		else if (it->Operat == "doe")
		{
			it->res.name = to_string(forqueuenum.front());
			forqueuenum.pop();
		}
		else if (it->Operat == "fore")
		{
			forqueue.front()->name = to_string(i);
			forqueue.pop();
			if (!for_level.empty())
			{
				forqueue.push(for_level.top());
				for_level.pop();
				forqueuenum.push(fornum_level.top());
				fornum_level.pop();
			}
		}
	}
}

void thirdview() //第三遍遍历，生成目标代码
{
	createDSEG(); //建立DSEG段
	createCSEGBEGIN(); //建立CSEG段
	createCode(resQT); //生成目标代码
	createCSEGEND(); //建立CSEG段
}

void reviewresQT() //遍历resQT并输出
{
	ofstream file;
	file.open("C:\\byresult\\resQT.txt");
	vector<QT>::iterator it;
	for (it = resQT.begin(); it != resQT.end(); it++)
	{
		file << it->Operat << "	" << it->opA.name <<
			"	" << it->opB.name << "	" <<
			it->res.name << endl;
	}
}

