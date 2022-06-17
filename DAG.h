#pragma once
#include "four.h"

#include <iostream>
#include <string>
#include <vector>


struct node
{
	int num; //结点号
	int left; //左结点
	int right; //右结点
	string Operat; //操作符
	token Mark[20]; //标记，0号为主标记
	node() { num = -1; left = -1; right = -1; Operat = ""; }
};

extern vector<QT>resQT; //存放最终优化后的所有四元式
extern vector<QT>groupQT; //优化后存放的四元式组
extern vector<node>DAG;
extern vector<token>SYMBL;
extern vector<token>SYMBL_ALL;

int searchNode(string idtemp); //根据标识符寻找到结点标号
int searchNode(string tempOper, string tempB, string tempC); //根据操作符和操作数B，C查找结点标号（双目运算符）
int searchNode(string tempOper, string tempB); //根据操作符和操作数B查找结点标号（单目运算符）
int searchMark(string marktemp, int nodeNum);//在确定NODE内遍历标记找出是否存在此标记
bool searchActive(string temp); //在SYMBL表中根据标识符返回活跃信息
void deleteMark(string nameM, int n); //删除除了第N号结点以外的附标记
int judgeOperate(string t); //返回操作符的类型
string calculate(string tempOper, string tempB, string tempC); //计算两个数运算的值
void attachNode(int number, token tempA); //把标记A附着到结点上
void swapMark(node& temp); //标记互换
QT createQT(string tempOper, token opA, token opB, token res); //创建双目运算符四元式
QT createQT(string tempOper, token opA, token res); //创建单目运算符四元式
int createNode(); //建立新结点，返回最后一个结点的标号
void createDAG(vector<QT>block_qt); //根据给定的四元式组block_qt构造优化的DAG，原本的block_qt不改变，而生成一个DAG
void createQT(); //根据优化的DAG，重组四元式C
void createSYMBL(); //遍历优化后的QT组，生成活跃信息表
void createRes();//遍历groupQT，把四元式加入进resQT
int searchSYMBL(string temp); //在SYMBL表中搜索该标识符是否存在
void changeActive(); //逆序生成活跃信息
void addSYMBL(); //填入一个全局的符号表为DSEG段做准备
void printQT(vector<QT>tempQT); //打印四元式组
void initQT(vector<QT>& tempQT); //测试用构造初始四元式组

