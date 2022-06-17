#include"grammar.h"
#include"block.h"

#include<vector>
#include "symbol.h"
using namespace std;

int main() {
	Recursive();	//语法分析开始

	cout << "符号总表数量: " << synbollist_main_g.size() << endl;
	int test_1 = synbollist_main_g.size();
	for (int i = 0; i < test_1; i++) {
		cout << "第" << i + 1 << "张函数符号表的名字: " << synbollist_main_g.at(i)->func << '\t' << "长度: " << synbollist_main_g.at(i)->synbollist.size() << endl;
	}
	//输出
	//printGlobal();
	//printSynbollistMain_g();
	writeGlobalToFile("C:\\byresult\\Synbol.txt");
	writeSynbollistMain_gToFile("C:\\byresult\\Synbol.txt");

	//delete工作  
	clearLists();	//释放所有已经填到表中的指针		
	clearGlobalVariable();//释放全局变量

	token null_point;
	null_point.name = "";
	null_point.type = 0;

	qt.push_back({ "" , null_point, null_point,null_point });//结束标记
	divide();			//基本块划分
	WriteQt("C:\\byresult\\Qt.txt", "四元式：");

	firstview();
	secondview();
	reviewresQT();
	thirdview();

	system("pause");
	return 0;
}


