
#include <iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cstdlib>
#include<math.h>
#define nodeNumber 55
using namespace std;

class symbol {
public:
    string name;
    int type;
    string cat;
    int add;
};
class type {
    int type;
    int point;

};
typedef struct mapNode {
    int step;
    char ch;
    struct mapNode* next = NULL;

}mapNode, * mapNodePoint;
typedef struct listNode {
    mapNodePoint first = NULL;

} listNode;

class token{
public:
  string type;
  int point;
  
};
class symbollist {
public:
    token t;
    vector<symbol>SYMBOL;//符号表总表
    vector<type>TYPE;// 种类表
    vector<string>K_list, I_list, P_list, CT_list, ST_list;
    vector<int>C_list;
    vector<string>C2_list;
    void creat_list();
    void search_P(string& str);
    int search_K(string& str);
    void insert_CT(string& str);
    void insert_I(string& str);
    void insert_C(int str);
    void insert_C2(string& str);
    void insert_ST(string& str);
    void showlist();
};
class Machine {
public:
    void  getString(int& point, string sin,symbollist& sym);
    void createMap();
    listNode mapList[nodeNumber];
    void insertNode(int oristep, char ch, int nextstep);
    int move(int x0, char ch);
    void showMap();
    int test(string& sin);
  

};


