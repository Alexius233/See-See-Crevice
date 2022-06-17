#pragma once
#include "code.h"
#include "DAG.h"

void firstview(); //第一遍遍历，生成所有优化后的四元式于resQT
void secondview(); //第二遍遍历，填跳转块号
void thirdview(); //第三遍遍历，生成目标代码
void reviewresQT(); //遍历resQT并输出
