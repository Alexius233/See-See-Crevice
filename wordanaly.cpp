
#include <iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cstdlib>
#include<math.h>
#include"wordanaly.h"
#define itp 0
#define rtp 1
#define ctp 2

using namespace std;
void symbollist::showlist(){
	int i;
	 cout << endl << "SYMBOL:";
     cout << endl<< "name "<< " " << "type "<< ' ' << "cat "<< ' ' << "add"<<endl;
    for (i = 1; i < SYMBOL.size(); i++) {
        cout << SYMBOL[i].name<<"      "<< SYMBOL[i].type<<"     "<< SYMBOL[i].cat<<endl;
        //if (i != SYMBOL.size() - 1)
           // cout << " ";

    }

    cout << endl << "C1 :";
    for (i = 1; i < C_list.size(); i++) {
        cout << C_list[i];
        if (i != C_list.size() - 1)
            cout << " ";


    }
    cout << endl << "C2 :";
    for (i = 1; i < C2_list.size(); i++) {
        cout << C2_list[i];
        if (i != C2_list.size() - 1)
            cout << " ";


    }
    cout << endl << "CT :";
    for (i = 1; i < CT_list.size(); i++) {
        cout << CT_list[i];
        if (i != CT_list.size() - 1)
            cout << " ";


    }
    cout << endl << "ST :";
    for (i = 1; i < ST_list.size(); i++) {
        cout << ST_list[i];
        if (i != ST_list.size() - 1)
            cout << " ";


    }
}
	
void symbollist:: creat_list() {
    symbol s;
    SYMBOL.push_back(s);
    C_list.push_back(0);
    CT_list.push_back("#");
    C2_list.push_back("#");
    ST_list.push_back("#");
    K_list.push_back("#");
    K_list.push_back("int");
    K_list.push_back("void");
    K_list.push_back("break");
    K_list.push_back("float");
    K_list.push_back("while");
    K_list.push_back("do");
    K_list.push_back("struct");
    K_list.push_back("const");
    K_list.push_back("case");
    K_list.push_back("for");
    K_list.push_back("return");
    K_list.push_back("if");
    K_list.push_back("default");
    K_list.push_back("else");
    K_list.push_back("main");
    K_list.push_back("char");
    P_list.push_back("#");
    P_list.push_back("-");
    P_list.push_back("/");
    P_list.push_back("(");
    P_list.push_back(")");
    P_list.push_back("==");
    P_list.push_back("<=");
    P_list.push_back("!=");
    P_list.push_back(">=");
    P_list.push_back("<");
    P_list.push_back("+");
    P_list.push_back("*");
    P_list.push_back(">");
    P_list.push_back("=");
    P_list.push_back(",");
    P_list.push_back(";");
    P_list.push_back("++");
    P_list.push_back("{");
    P_list.push_back("}");
    P_list.push_back("[");
    P_list.push_back("]");
    P_list.push_back("||");
    P_list.push_back("&&");
}

void symbollist:: search_P(string& str) {

    int i;
    for (i = 1; i < P_list.size(); i++)
    {

        if (P_list[i] == str) {
        	t.type="P";
        	t.point=i;
            //cout << "(P " << i << ")";
        }
    }
    return;
}
int symbollist:: search_K(string& str) {
    int i = 1;
    for (i = 1; i < K_list.size(); i++)
    {
        if (K_list[i] == str)
        
        {
        	t.type="K";
        	t.point=i;
          // cout << "(K " << i << ")";
            return 1;

        }
    }
    return 0;
}

void symbollist::insert_CT(string& str) {
    int i = 1;
    for (i = 1; i < CT_list.size(); i++)
    {
        if (CT_list[i] == str) {
        	t.type="CT";
        	t.point=i;
           // cout << "(CT " << i << ")";
            return;
        }

    }
    CT_list.push_back(str);
    t.type="CT";
    t.point=i;
   // cout << "(CT " << i << ")";
    return;

}
void symbollist::insert_I(string& str) {
    int i = 1;
    symbol s;
    for (i = 1; i < SYMBOL.size(); i++)//Óöµ½¹Ø¼ü×Ö£¬ËÑË÷ÊÇ·ñ³öÏÖ
    {
        if (SYMBOL[i].name == str) {
        	t.type="I";
            t.point=i;
           
           // cout << "(I " << i << ")";
            return;
        }

    }
    s.name = str;
    SYMBOL.push_back(s);//Î´³öÏÖ¹ý£¬Ìí¼Ó
    t.type="I";
    t.point=i;
    //cout << "(I " << i << ")";
    return;

}
void symbollist:: insert_C(int str) {
    int i = 1;
    for (i = 1; i < C_list.size(); i++)
    {
        if (C_list[i] == str) {
        	t.type="C1";
            t.point=i;
            //cout << "(C1 " << i << ")";
            return;
        }

    }
    C_list.push_back(str);
    t.type="C1";
    t.point=i;
    //cout << "(C1 " << i << ")";
    return;

}

void symbollist::insert_C2(string& str) {
    int i = 1;
    for (i = 1; i < C2_list.size(); i++)
    {
        if (C2_list[i] == str) {
        	t.type="C2";
            t.point=i;

            //cout << "(C2 " << i << ")";
            return;
        }

    }
    C2_list.push_back(str);
    t.type="C2";
    t.point=i;
   // cout << "(C2 " << i << ")";
    return;
}
void symbollist:: insert_ST(string& str) {
    int i = 1;
    for (i = 1; i < ST_list.size(); i++)
    {
        if (ST_list[i] == str) {
        	t.type="ST";
            t.point=i;
           // cout << "(ST " << i << ")";
            return;
        }
    }
    ST_list.push_back(str);
    t.type="ST";
	t.point=i;
   // cout << "(ST " << i << ")";
    return;

}

int Machine::test(string& sin) {
    int point = 0;
    int step = 0;//åˆå§‹çŠ¶æ€?
    int nextstep = 0;//æ¬¡æ€?
    while (point < sin.length()) {

        nextstep = move(step, sin[point]);
      // cout << step <<"->"<< sin[point] << nextstep<<endl;
        if (nextstep == -1)
        {
            cout << "ERROR";
            exit(1);
        }
        step = nextstep;
        switch (step) {
        case 2:
        case 4:
        case 7:
        case 8:
        case 11:
        case 12:
        case 15:
        case 16:
        case 18:
        case 35:
        case 36:
        case 26:
        case 30:
        case 37:
        case 34:step = 0; break;
        }
        point++;//æŒ‡é’ˆç§»åŠ¨

    }
    return 1;
}
void Machine::showMap() {

    int i = 0;
    for (i = 0; i < nodeNumber; i++)
    {
        mapNodePoint p1 = mapList[i].first;
        while (p1)
        {
            cout << i << '-' << p1->ch << '-' << p1->step << endl;
            p1 = p1->next;
        }
    }
}
void Machine::createMap() {//æž„å»ºå›¾èŠ‚ç‚¹å…³ç³?
    int i, j;
    for (i = 0; i < nodeNumber; i++)
        mapList[i].first = NULL;

    insertNode(0, ' ', 0);
    insertNode(0, '\n', 0);
    insertNode(0, '#', 55);
    insertNode(0, '_', 1);
    insertNode(0, '0', 31);
    insertNode(31, 'x', 32);
    insertNode(31, 'X', 32);
    for (i = 65; i <= 90; i++)
    {   
        insertNode(0, char(i), 1);
        insertNode(0, char(i + 32), 1);
        insertNode(1, char(i), 1);//A-Z
        insertNode(1, char(i + 32), 1);//a-z

        insertNode(19, char(i), 19);
        insertNode(21, char(i), 22);
        insertNode(5, char(i), 8);
        insertNode(6, char(i), 7);
        insertNode(9, char(i), 12);
        insertNode(10, char(i), 11);
        insertNode(13, char(i), 16);
        insertNode(14, char(i), 15);
        insertNode(17, char(i), 18);
        insertNode(38, char(i), 40);//ÖÕÖ¹
        insertNode(39, char(i), 41);//ÖÕÖ¹
        insertNode(42, char(i), 44);//ÖÕÖ¹
        insertNode(43, char(i), 45);//ÖÕÖ¹
        insertNode(51, char(i), 54);//ÖÕÖ¹
        insertNode(52, char(i), 53);//ÖÕÖ¹
        insertNode(47, char(i), 50);//ÖÕÖ¹
        insertNode(48, char(i), 49);//ÖÕÖ¹

        insertNode(38, char(i+32), 40);//ÖÕÖ¹
        insertNode(39, char(i+32), 41);//ÖÕÖ¹
        insertNode(42, char(i+32), 44);//ÖÕÖ¹
        insertNode(43, char(i+32), 45);//ÖÕÖ¹
        insertNode(51, char(i+32), 54);//ÖÕÖ¹
        insertNode(52, char(i+32), 53);//ÖÕÖ¹
        insertNode(47, char(i+32), 50);//ÖÕÖ¹
        insertNode(48, char(i+32), 49);//ÖÕÖ¹


        insertNode(5, char(i + 32), 8);
        insertNode(6, char(i + 32), 7);
        insertNode(9, char(i + 32), 12);
        insertNode(10, char(i + 32), 11);
        insertNode(13, char(i + 32), 16);
        insertNode(14, char(i + 32), 15);
        insertNode(17, char(i + 32), 18);
        insertNode(19, char(i + 32), 19);
        insertNode(21, char(i + 32), 22);
       
        if (i <= 70)
        {
            insertNode(32, char(i + 32), 33);
            insertNode(33, char(i + 32), 33);
        }

    }
    insertNode(1, '_', 1);//1 _ 1
    //insertNode(0, '-', 3);//0 - 3
    for (i = 48; i <= 58; i++) {
        insertNode(1, char(i), 1);// 1 0~9 1
     if(i!=48)
         insertNode(0, char(i), 3);//0 0~9 3
        insertNode(3, char(i), 3);//3 0~9 3
        insertNode(5, char(i), 8);
        insertNode(6, char(i), 7);
        insertNode(9, char(i), 12);
        insertNode(10, char(i), 11);
        insertNode(13, char(i), 16);
        insertNode(14, char(i), 15);
        insertNode(17, char(i), 18);
        insertNode(24, char(i), 25);
        insertNode(25, char(i), 25);
        insertNode(28, char(i), 29);
        insertNode(27, char(i), 29);
        insertNode(29, char(i), 29);
        insertNode(31, char(i), 3);
        insertNode(32, char(i), 33);
        insertNode(33, char(i), 33);
        insertNode(19, char(i), 19);
        insertNode(21, char(i), 22);
        insertNode(38, char(i), 40);//ÖÕÖ¹
        insertNode(39, char(i), 41);//ÖÕÖ¹
        insertNode(42, char(i), 44);//ÖÕÖ¹
        insertNode(43, char(i), 45);//ÖÕÖ¹
        insertNode(51, char(i), 54);//ÖÕÖ¹
        insertNode(52, char(i), 53);//ÖÕÖ¹
        insertNode(47, char(i), 50);//ÖÕÖ¹
        insertNode(48, char(i), 49);//ÖÕÖ¹
       

    }


    insertNode(0, '=', 5);//
    insertNode(5, '=', 6);//
    insertNode(0, '<', 9);//
    insertNode(9, '=', 10);//
    insertNode(0, '+', 13);//
    insertNode(13, '+', 14);
    insertNode(0, '|', 38);//
    insertNode(38, '|', 39);
    insertNode(0, '&', 42);//
    insertNode(42, '&', 43);
    insertNode(0, '>', 51);
    insertNode(51, '=', 52);
    insertNode(0, '!', 47);
    insertNode(47, '=', 48);


    insertNode(0, '-', 17);//
    insertNode(0, '/', 17);//
    insertNode(0, '(', 17);//
    insertNode(0, ')', 17);//
    insertNode(0, '*', 17);//
   // insertNode(0, '>', 17);//
    insertNode(0, ',', 17);
    insertNode(0, ';', 17);
    insertNode(0, '{', 17);
    insertNode(0, '}', 17);
    insertNode(0, ']', 17);
    insertNode(0, '[', 17);
   // insertNode(0, '!', 17);


    insertNode(3, '.', 24);
    insertNode(31, '.', 24);
    insertNode(3, 'e', 27);
    insertNode(25, 'e', 27);
    insertNode(27, '+', 28);
    insertNode(27, '-', 28);


    insertNode(0, char(34), 19);
    insertNode(19, char(34), 20);


    insertNode(0, char(39), 21);
    insertNode(22, char(39), 23);


    string str = "!=<+/ [](){}*,;->#'\"\n!&|";
    for (i = 0; i < str.length(); i++)
    {
        insertNode(1, str[i], 2);
        insertNode(3, str[i], 4);
        if (str[i] != '=')
        {
            insertNode(5, str[i], 8);
            insertNode(9, str[i], 12);
            insertNode(47, str[i], 50);
            insertNode(51, str[i], 54);
        }
        insertNode(6, str[i], 7);
        insertNode(10, str[i], 11);
        insertNode(14, str[i], 15);
        if (str[i] != '+')
            insertNode(13, str[i], 16);

        insertNode(17, str[i], 18);

        insertNode(25, str[i], 26);
        insertNode(31, str[i], 37);
        insertNode(29, str[i], 30);
        insertNode(33, str[i], 34);
        insertNode(20, str[i], 35);
        insertNode(23, str[i], 36);

        insertNode(48, str[i], 49);
        insertNode(52, str[i], 53);
        //insertNode(23, str[i], 36);
    }
}
void  Machine::getString(int& point, string sin,symbollist& sym) {
    int step = 0;//åˆå§‹çŠ¶æ€?
    int nextstep = 0;//æ¬¡æ€?
    string sout = "";//æš‚å­˜é€šè¿‡è¯†åˆ«çš„å­—ç¬?
    int num = 0;
    float num_afterpoint = 0;
    float real = 0;
    int e_num = 0;
    int sign = 1;
    int n = 0;//å°æ•°ä½æ•°




    while (true) {
       // cout << "µ±Ç°×Ö·û£º";
        //cout << sin[point];
        nextstep = move(step, sin[point]);//çŠ¶æ€è½¬ç§»å‡½æ•?
       
      // cout << step <<"->"<< sin[point] << nextstep<<endl;
        if (nextstep == -1)
        {
 
            cout << "´Ê·¨ERROR£¡";//æ— å¯è·³è½¬çŠ¶æ€ï¼ŒæŽ¥å—å¤±è´¥ï¼?
            exit(1);
        }
        step = nextstep;

        switch (step) {
        case 0:break;

        case 2:if (!sym.search_K(sout))//è¯†åˆ«å…³é”®å­—ä¸Žæ ‡è¯†ç¬?
            sym.insert_I(sout);
            point++;
            return;


        case 3:num = 10 * num + (sin[point] - 48);//è¯†åˆ«æ•´æ•°
            sout += char(sin[point]);
            break;
        case 4:sym.insert_C(num);
            point++;
            return;

        
        case 7://è¯†åˆ«è¿ç®—ç¬?
        case 8:
        case 11:
        case 12:
        case 15:
        case 16:
        case 18: 
        case 40:
        case 41:
        case 44:
        case 45:
        case 50:
        case 49:
        case 53:
        case 54:
        case 55:

            sym.search_P(sout);
            point++;
            return;


        case 19:if (sin[point] != '"')//è¯†åˆ«å­—ç¬¦ä¸?
        {
            //cout << sin[point];
            sout += char(sin[point]);
        }
        case 20:break;
        case 35:
            sym.insert_ST(sout);
            point++;
            return;


        case 21://è¯†åˆ«å­—ç¬¦
        case 23:break;
        case 36:sym.insert_CT(sout);
            point++;
            return;


        case 25:n++;//è¯†åˆ«æµ®ç‚¹æ•?
            num_afterpoint = num_afterpoint + pow(0.1, n) * (sin[point] - 48);
            sout += char(sin[point]);
            break;
        case 26:real = num + num_afterpoint;
            sym.insert_C2(sout);
            point++;
            return;

        case 27:real = num + num_afterpoint;
            sout += char(sin[point]);
            break;//è¯†åˆ«ç§‘å­¦è®¡æ•°æ³?
        case  28:sign = (sin[point] == '+' ? 1 : -1);
            sout += char(sin[point]);
            break;
        case 29:e_num = 10 * e_num + (sin[point] - 48);
            sout += char(sin[point]);
            break;
        case 30:real = real * pow(10, sign * e_num);
            sym.insert_C2(sout);
            point++;
            return;

        case 31:sout += char(sin[point]);
        case 32:break;
        case 33:num = num * 16 + (sin[point] > '9' ? (10 + sin[point] - 97) : (sin[point] - 48));
            break;
        case 34:sym.insert_C(num);
            point++;
            return;
        case 37:
            sym.insert_C(num);
            point++;
            return;
        default:sout += char(sin[point]); break;//å…¶ä½™çŠ¶æ€å­—ç¬¦ä¸²è¿½åŠ 
        }
        
            point++;
       

    }
}

void Machine::insertNode(int oristep, char ch, int nextstep)//æž„å»ºå›¾æ’å…¥èŠ‚ç‚¹å‡½æ•?
{

    mapNodePoint p1 = NULL, p2 = NULL;
    p2 = new mapNode;//å¼€è¾Ÿç©ºé—?
    p2->ch = ch;
    p2->step = nextstep;
    p1 = mapList[oristep].first;//åŽŸfirstæŒ‡é’ˆ
    mapList[oristep].first = p2;//å¤´æ’æ³?
    p2->next = p1;
    return;
}
int Machine::move(int x0, char ch)
{
    mapNodePoint p1 = mapList[x0].first;
    while (p1)
    {
        if (ch == p1->ch)//æŸ¥è¯¢x0çŠ¶æ€å¯è·³è½¬çš„çŠ¶æ€?
            return p1->step;//æŸ¥è¯¢æˆåŠŸï¼Œè¿”å›žæ¬¡æ€ï¼
        p1 = p1->next;//å·¥ä½œæŒ‡é’ˆç§»åŠ¨
    }
    return -1;//æŸ¥æ‰¾å¤±è´¥ï¼Œè¿”å›?1

}





