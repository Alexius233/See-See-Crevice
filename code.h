#pragma once
#include "four.h"

#include <iostream>
#include <string>
#include <vector>

void createDSEG(); //建立DSEG段
void createCSEGBEGIN(); //建立CSEG段
void createCSEGEND(); //建立CSEG段
void createCode(vector<QT>resQT); //根据一个四元式组生成一段代码
void code(QT temp, string lastOper, int i); //根据单个四元式生成CODE

