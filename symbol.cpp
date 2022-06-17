#include "symbol.h"
#include "grammar.h"
#include<fstream>

void checkSynbollist_main(string str) {		//查重符号总表（即查是否函数重定义）
	for (unsigned i = 0; i < synbollist_main_g.size(); i++) {
		if (str == synbollist_main_g.at(i)->func) {
			string s = str + "重定义";
			clearAll_g();
			clearLists();
			clearGlobalVariable();
			Error(token_g.row, token_g.content, s);
		}
	}
}
void checkSynbollistToFunc(string funcId, string str) {	//查重函数符号表（查局部变量是否重定义）
	for (unsigned i = 0; i < synbollistToFunc_g->synbollist.size(); i++) {
		if (str == synbollistToFunc_g->synbollist.at(i)->content) {
			string s = str + "重定义";
			clearAll_g();
			clearLists();
			clearGlobalVariable();
			Error(token_g.row, token_g.content, s);
		}
	}
}

void checkParas(string str) {				//查重形参表(查形参是否重定义)
	for (unsigned i = 0; i < paras_g->size(); i++) {
		if (str == paras_g->at(i)->content) {
			string s = str + "重定义";
			clearAll_g();
			clearLists();
			clearGlobalVariable();
			Error(token_g.row, token_g.content, s);
		}
	}
}

void checkGlobal(string str) {				//查重全局变量
	for (unsigned i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		if (str == globalsynbollistToFunc_g->synbollist.at(i)->content) {
			string s = "全局变量" + str + "重定义";
			clearAll_g();
			clearLists();
			clearGlobalVariable();
			Error(token_g.row, token_g.content, s);
		}
	}
}


bool checkGlobalNum(string str) {
	for (unsigned i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		if (str == globalsynbollistToFunc_g->synbollist.at(i)->content) {
			return true;
		}
	}
	return false;
}

bool checkSynbollistToFuncNum(string str) {
	for (unsigned i = 0; i < synbollistToFunc_g->synbollist.size(); i++) {
		if (str == synbollistToFunc_g->synbollist.at(i)->content) {
			return true;
		}
	}
	return false;
}




//为了实现这6个函数，定义一些模块化函数......

void writeTypeSheet(string type) {		//写一张类型表
	if (typeSheet_g == NULL) {
		typeSheet_g = new TypeSheet;
		typeSheet_g->typevalue = type;
		typeSheet_g->tpoint = NULL;
	}
	else {
		showErrowWhenCreateASheet("TypeSheet");
	}
}

void writeLevelAndOffsetAndValue(int level, int offset) {	//写一张表，这张表存的是 level offset vlaue
	if (levelAndOffsetAndValue_g == NULL) {
		levelAndOffsetAndValue_g = new LevelAndOffsetAndValue;
		levelAndOffsetAndValue_g->level = level;
		levelAndOffsetAndValue_g->offset = offset;
		levelAndOffsetAndValue_g->value = "I don't know till now";
	}
	else {
		showErrowWhenCreateASheet("LevelAndOffsetAndValue");
	}
}


void showErrowWhenCreateASheet(string str) {
	string s = "在创建一个" + str + "对象时出现冲突";
	clearAll_g();
	clearLists();
	clearGlobalVariable();
	Error(token_g.row, token_g.content, s);
}

/*高级输出函数*/
void printGlobal() {											//输出全局变量
	for (unsigned i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		printMainSheet(globalsynbollistToFunc_g->synbollist.at(i));
	}
}
void printSynbollistMain_g() {								//输出符号总表的所有内容
	for (unsigned int i = 0; i < synbollist_main_g.size(); i++) {
		for (unsigned int j = 0; j < synbollist_main_g.at(i)->synbollist.size(); j++) {
			printMainSheet(synbollist_main_g.at(i)->synbollist.at(j));
		}
	}
}
/*底层输出函数*/



/*高级清理函数*/
void clearLists() {			//释放所用存入表中的指针
	//synbollist_main_g
	for (unsigned int i = 0; i < synbollist_main_g.size(); i++) {
		for (unsigned int j = 0; j < synbollist_main_g.at(i)->synbollist.size(); j++) {
			deleteMainSheet(synbollist_main_g.at(i)->synbollist.at(j));
		}
		delete synbollist_main_g.at(i);
	}
}

void clearAll_g() {			//释放所有全局指针和队列中的指针,只有在出错时会调用
	delete synbollistToFunc_g;			   //函数所对应的符号表的指针
	delete mainSheet_g;					   //符号表项指针
	delete typeSheet_g;					   //类型表指针
	delete funcSheet_g;					   //函数表指针
	delete paras_g;						   //参数表指针
	delete para_g;						   //参数表项指针
	delete levelAndOffsetAndValue_g;	   //LevelAndOffset表指针
	//释放队列指针
	while (!tempMainSheetpointers.empty()) {
		delete tempMainSheetpointers.front();
		tempMainSheetpointers.pop();
	}
}

void clearGlobalVariable() {	//释放全局变量（由globalsynbollistToFunc_g指向）
	for (unsigned i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		deleteMainSheet(globalsynbollistToFunc_g->synbollist.at(i));
	}
}
/*高级清理函数*/



/*底层输出函数*/
void printMainSheet(MainSheet* mainSheet) {				//输出MainSheet表	
	cout << "MainSheet表内容: " << endl;
	cout << "标识符名字: " << mainSheet->content << endl;
	cout << "标识符类型: " << endl; printTypeSheet(mainSheet->type);
	cout << "标识符种类: " << mainSheet->category << endl;
	if (mainSheet->flag == 0) {
		printfFuncSheet((FuncSheet*)mainSheet->addr);
	}
	else if (mainSheet->flag == 1) {
		printLevelAndOffsetAndValue((LevelAndOffsetAndValue*)mainSheet->addr);
	}
}
void printTypeSheet(TypeSheet* typeSheet) {					//输出TypeSheet表
	cout << "----" << "TypeSheet表内容: " << endl;
	cout << "    " << "typeValue: " << typeSheet->typevalue << endl;
	//因为目前没有数组和结构体所以typeSheet的指针项为空
	cout << endl;
}
void printLevelAndOffsetAndValue(LevelAndOffsetAndValue* levelAndOffsetAndValue) {	//输出LevelAndOffset表
	cout << "----" << "LevelAndOffset表内容: " << endl;
	cout << "    " << "level:  " << levelAndOffsetAndValue->level << endl;
	cout << "    " << "offset: " << levelAndOffsetAndValue->offset << endl;
	cout << "    " << "vlaue:  " << levelAndOffsetAndValue->value << endl;
}
void printfFuncSheet(FuncSheet* funcsheet) {				//函数表输出
	cout << "----" << "函数表内容: " << endl;
	cout << "    " << "函数层次: " << funcsheet->level << endl;
	cout << "    " << "形参个数: " << funcsheet->fnum << endl;
	cout << "    " << "入口地址: " << funcsheet->entry << endl;
	cout << "    " << "参数表: " << endl;
	for (unsigned i = 0; i < funcsheet->para->size(); i++) {
		printParaSheet(funcsheet->para->at(i));
	}
	cout << endl;
}
void printParaSheet(ParaSheet* paraSheet) {					//输出形参表
	cout << "----" << "Para表内容: " << endl;
	cout << "    " << "形参名: " << paraSheet->content << endl;
	cout << "    " << "形参类型: "; printTypeSheet(paraSheet->type);
	cout << "    " << "形参表的LevelAndOffsetAndValue表: "; printLevelAndOffsetAndValue(paraSheet->addr);
	cout << endl;
}
/*底层输出函数*/



/*底层清理函数*/
void deleteMainSheet(MainSheet* mainSheet_p) {	//完全销毁一个MainSheet对象
	if (mainSheet_p->flag == 0) {
		deleteFuncSheet((FuncSheet*)mainSheet_p->addr);
	}
	else if (mainSheet_p->flag == 1) {
		deleteLevelAndOffsetAndValue((LevelAndOffsetAndValue*)mainSheet_p->addr);
	}
	deleteTypeSheet(mainSheet_p->type);
	delete mainSheet_p;
	//cout << "成功释放了一个MainSheet对象" << endl;
}
void deleteTypeSheet(TypeSheet* typeSheet_p) {	//完全销毁一个TypeSheet对象
	if (typeSheet_p == NULL)return;
	delete typeSheet_p;
	//cout << "成功释放了一个TypeSheet对象" << endl;
}
void deleteLevelAndOffsetAndValue(LevelAndOffsetAndValue* levelAndOffsetAndValue_p) {	//完全销毁一个levelAndOffset对象
	if (levelAndOffsetAndValue_p == NULL)return;
	delete levelAndOffsetAndValue_p;
	//cout << "成功释放了一个LevelAndOffsetAndValue对象" << endl;
}
void deleteFuncSheet(FuncSheet* funcSheet_p) {   //完全销毁一个FuncSheet对象
	if (funcSheet_p == NULL) {
		return;
	}
	for (unsigned i = 0; i < funcSheet_p->para->size(); i++) {
		deletePara(funcSheet_p->para->at(i));
	}
	delete funcSheet_p;
	//cout << "成功释放了一个FuncSheet对象" << endl;
}
void deletePara(ParaSheet* para_p) {				//完全销毁一个ParaSheet对象
	deleteLevelAndOffsetAndValue(para_p->addr);
	deleteTypeSheet(para_p->type);
	delete para_p;
	//cout << "成功释放了一个Para对象" << endl;
}
/*底层清理函数*/


/*最终要实现的7个函数*/
//没查到返回no
string searchType(string funcname, string name) {//查非全局用户定义和临时变量类型
	for (unsigned i = 0; i < synbollist_main_g.size(); i++) {
		if (synbollist_main_g.at(i)->func == funcname) {
			for (unsigned j = 0; j < synbollist_main_g.at(i)->synbollist.size(); j++) {
				if (synbollist_main_g.at(i)->synbollist.at(j)->content == name) {
					return synbollist_main_g.at(i)->synbollist.at(j)->type->typevalue;
				}
			}
		}
	}
	if (synbollistToFunc_g != NULL)
	{
		for (unsigned i = 0; i < synbollistToFunc_g->synbollist.size(); i++) {
			if (synbollistToFunc_g->synbollist.at(i)->content == name) {
				return synbollistToFunc_g->synbollist.at(i)->type->typevalue;
			}
		}
	}
	return "no";
}

string searchType(string name) {//查全局用户定义变量类型
	for (unsigned i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		if (globalsynbollistToFunc_g->synbollist.at(i)->content == name) {
			return globalsynbollistToFunc_g->synbollist.at(i)->type->typevalue;
		}
	}
	return "no";
}
string searchValue(string funcname, string name) {//查非全局变量和临时变量值
	for (unsigned i = 0; i < synbollist_main_g.size(); i++) {
		if (synbollist_main_g.at(i)->func == funcname) {
			for (unsigned j = 0; j < synbollist_main_g.at(i)->synbollist.size(); j++) {
				if (synbollist_main_g.at(i)->synbollist.at(j)->content == name) {
					if (synbollist_main_g.at(i)->synbollist.at(j)->category == "f") {
						return ((FuncSheet*)synbollist_main_g.at(i)->synbollist.at(j)->addr)->value;
					}
					else {
						if (((LevelAndOffsetAndValue*)(synbollist_main_g.at(i)->synbollist.at(j)->addr))->value != "I don't know till now") {
							return ((LevelAndOffsetAndValue*)(synbollist_main_g.at(i)->synbollist.at(j)->addr))->value;
						}
						else {
							return "no";
						}
					}
				}
			}
		}
	}
	if (synbollistToFunc_g != NULL) {
		for (unsigned i = 0; i < synbollistToFunc_g->synbollist.size(); i++) {
			if (name == synbollistToFunc_g->synbollist.at(i)->content) {
				if (((LevelAndOffsetAndValue*)(synbollistToFunc_g->synbollist.at(i))->addr)->value != "I don't know till now") {
					return ((LevelAndOffsetAndValue*)(synbollistToFunc_g->synbollist.at(i))->addr)->value;
				}
				else {
					return "no";
				}
			}
		}
	}

	return "no";
}
string searchValue(string name) {//查全局用户定义变量值
	for (unsigned i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		if (globalsynbollistToFunc_g->synbollist.at(i)->content == name) {
			if (((LevelAndOffsetAndValue*)(globalsynbollistToFunc_g->synbollist.at(i)->addr))->value != "I don't know till now") {
				return ((LevelAndOffsetAndValue*)(globalsynbollistToFunc_g->synbollist.at(i)->addr))->value;
			}
			else {
				return "no";
			}
		}
	}
	return "no";
}
void saveTemp(string funcname, string name, string type, string value) {//保存临时变量类型和值
	for (unsigned i = 0; i < synbollist_main_g.size(); i++) {
		if (funcname == synbollist_main_g.at(i)->func) {
			MainSheet* tempMainSheet;

			tempMainSheet = new MainSheet;
			tempMainSheet->content = name;
			tempMainSheet->category = "vt";
			tempMainSheet->flag = 1;
			writeTypeSheet(type);
			tempMainSheet->type = typeSheet_g;
			typeSheet_g = NULL;
			writeLevelAndOffsetAndValue(((LevelAndOffsetAndValue*)(synbollist_main_g.at(i)->synbollist.at(0)->addr))->level, synbollist_main_g.at(i)->offset_max);
			if (type == "char") {
				synbollist_main_g.at(i)->offset_max += 1;
			}
			else if (type == "int") {
				synbollist_main_g.at(i)->offset_max += 4;
			}
			else if (type == "double") {
				synbollist_main_g.at(i)->offset_max += 8;
			}
			else if (type == "float") {
				synbollist_main_g.at(i)->offset_max += 4;
			}
			levelAndOffsetAndValue_g->value = value;
			tempMainSheet->addr = levelAndOffsetAndValue_g;
			levelAndOffsetAndValue_g = NULL;
			synbollist_main_g.at(i)->synbollist.push_back(tempMainSheet);
			tempMainSheet = NULL;
			return;

		}
	}

	if (synbollistToFunc_g != NULL && synbollistToFunc_g->func == funcname) {
		MainSheet* tempMainSheet;

		tempMainSheet = new MainSheet;
		tempMainSheet->content = name;
		tempMainSheet->category = "vt";
		tempMainSheet->flag = 1;
		writeTypeSheet(type);
		tempMainSheet->type = typeSheet_g;
		typeSheet_g = NULL;
		writeLevelAndOffsetAndValue(((LevelAndOffsetAndValue*)(synbollistToFunc_g->synbollist.at(0)->addr))->level, synbollistToFunc_g->offset_max);
		if (type == "char") {
			synbollistToFunc_g->offset_max += 1;
		}
		else if (type == "int") {
			synbollistToFunc_g->offset_max += 4;
		}
		else if (type == "double") {
			synbollistToFunc_g->offset_max += 8;
		}
		else if (type == "float") {
			synbollistToFunc_g->offset_max += 4;
		}
		levelAndOffsetAndValue_g->value = value;
		tempMainSheet->addr = levelAndOffsetAndValue_g;
		levelAndOffsetAndValue_g = NULL;
		synbollistToFunc_g->synbollist.push_back(tempMainSheet);
		tempMainSheet = NULL;

	}

}
void saveAdmin(string funcname, string name, string value) {//保存用户的非全局变量的类型和值
	for (unsigned i = 0; i < synbollist_main_g.size(); i++) {
		if (funcname == synbollist_main_g.at(i)->func) {
			for (unsigned j = 0; j < synbollist_main_g.at(i)->synbollist.size(); j++) {
				if (name == synbollist_main_g.at(i)->synbollist.at(j)->content) {
					((LevelAndOffsetAndValue*)synbollist_main_g.at(i)->synbollist.at(j)->addr)->value = value;
					return;
				}
			}
		}
	}
	for (unsigned i = 0; i < synbollistToFunc_g->synbollist.size(); i++) {
		if (name == synbollistToFunc_g->synbollist.at(i)->content) {
			((LevelAndOffsetAndValue*)(synbollistToFunc_g->synbollist.at(i))->addr)->value = value;
		}
	}
}
void saveGlobal(string name, string value) {			//保存用户定义的全局变量
	unsigned i = 0;
	for (i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		if (name == globalsynbollistToFunc_g->synbollist.at(i)->content) {
			((LevelAndOffsetAndValue*)(globalsynbollistToFunc_g->synbollist.at(i)->addr))->value = value;
			break;
		}
	}
	if (i == globalsynbollistToFunc_g->synbollist.size()) {
		cout << "没查到要存的变量" << endl;
	}
}

void saveGlobal(string name, string type, string value) {			//保存全局临时变量

	if (mainSheet_g == NULL) {
		mainSheet_g = new MainSheet;
		mainSheet_g->content = name;
		mainSheet_g->category = "vt";
		mainSheet_g->flag = 1;
		writeTypeSheet(type);
		mainSheet_g->type = typeSheet_g;
		typeSheet_g = NULL;
		writeLevelAndOffsetAndValue(0, global_offset_g);
		if (type == "char") {
			global_offset_g += 1;
		}
		else if (type == "int") {
			global_offset_g += 4;
		}
		else if (type == "double") {
			global_offset_g += 8;
		}
		else if (type == "float") {
			global_offset_g += 4;
		}
		levelAndOffsetAndValue_g->value = value;
		mainSheet_g->addr = levelAndOffsetAndValue_g;
		levelAndOffsetAndValue_g = NULL;
		globalsynbollistToFunc_g->synbollist.push_back(mainSheet_g);
		mainSheet_g = NULL;
	}
	else {
		showErrowWhenCreateASheet("MainSheet");
	}

}

void writeGlobalToFile(string fileName) {
	ofstream ofs(fileName, ios::app);
	for (unsigned i = 0; i < globalsynbollistToFunc_g->synbollist.size(); i++) {
		writeMainSheetToFile(fileName, globalsynbollistToFunc_g->synbollist.at(i));
	}
	ofs.close();
}
void writeSynbollistMain_gToFile(string fileName) {
	ofstream ofs(fileName, ios::app);
	for (unsigned int i = 0; i < synbollist_main_g.size(); i++) {
		for (unsigned int j = 0; j < synbollist_main_g.at(i)->synbollist.size(); j++) {
			writeMainSheetToFile(fileName, synbollist_main_g.at(i)->synbollist.at(j));
		}
	}
	ofs.close();
}


void writeMainSheetToFile(string funcName, MainSheet* mainSheet) {
	ofstream ofs(funcName, ios::app);
	ofs << "MainSheet表内容: " << endl;
	ofs << "标识符名字: " << mainSheet->content << endl;
	ofs << "标识符类型: " << endl; printTypeSheet(mainSheet->type);
	ofs << "标识符种类: " << mainSheet->category << endl;
	if (mainSheet->flag == 0) {
		writeFuncSheetToFile(funcName, (FuncSheet*)mainSheet->addr);
	}
	else if (mainSheet->flag == 1) {
		writeLevelAndOffsetAndValueToFile(funcName, (LevelAndOffsetAndValue*)mainSheet->addr);
	}
	ofs << endl;
	ofs.close();
}
void writeTypeSheetToFile(string funcName, TypeSheet* typeSheet) {
	ofstream ofs(funcName, ios::app);
	ofs <<  "TypeSheet表内容: " << endl;
	ofs << "typeValue: " << typeSheet->typevalue << endl;
	ofs << endl;
	ofs.close();
}
void writeLevelAndOffsetAndValueToFile(string funcName, LevelAndOffsetAndValue* levelAndOffsetAndValue) {
	ofstream ofs(funcName, ios::app);
	//ofs << "----" << "LevelAndOffsetAndValue表内容: " << endl;
	ofs << "level:  " << levelAndOffsetAndValue->level << endl;
	ofs <<"offset: " << levelAndOffsetAndValue->offset << endl;
	ofs <<"vlaue:  " << levelAndOffsetAndValue->value << endl;
	ofs << endl;
	ofs.close();
}
void writeFuncSheetToFile(string funcName, FuncSheet* funcsheet) {
	ofstream ofs(funcName, ios::app);
	ofs << "函数表内容: " << endl;
	ofs << "函数层次: " << funcsheet->level << endl;
	ofs <<  "形参个数: " << funcsheet->fnum << endl;
	//ofs << "    " << "入口地址: " << funcsheet->entry << endl;
	//ofs << "    " << "参数表: " << endl;
	for (unsigned i = 0; i < funcsheet->para->size(); i++) {
		writeParaSheetToFile(funcName, funcsheet->para->at(i));
	}
	ofs << endl;
	ofs.close();
}
void writeParaSheetToFile(string funcName, ParaSheet* paraSheet) {
	ofstream ofs(funcName, ios::app);
	ofs <<  "参数表: " << endl;
	ofs <<  "形参名: " << paraSheet->content << endl;
    writeTypeSheetToFile(funcName, paraSheet->type);
	//ofs << "    " << "形参表的LevelAndOffsetAndValue表: "; writeLevelAndOffsetAndValueToFile(funcName, paraSheet->addr);
	ofs << endl;
	ofs.close();
}

/*最重要实现的7个函数*/
string TypeOfPara(string funcName, int pos) {
	for (unsigned i = 0; i < synbollist_main_g.size(); i++) {
		if (synbollist_main_g.at(i)->func == funcName) {
			return synbollist_main_g.at(i)->synbollist.at(pos)->type->typevalue;
		}
	}
	return "no";
}

//通过函数名，返回形参个数
int NumOfPara(string funcName) {
	for (unsigned i = 0; i < synbollist_main_g.size(); i++) {
		if (synbollist_main_g.at(i)->func == funcName) {
			return ((FuncSheet*)(synbollist_main_g.at(i)->synbollist.at(0)->addr))->fnum;
		}
	}
	return -1;
}
