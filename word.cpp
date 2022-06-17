#include"word.h"
int row = 1;
const int KEYLIST_LENGTH = 40;
const string keyList[] = { "if", "else", "while", "signed", "throw", "this", "int", "char",
						"double", "const", "virtual", "for", "float", "break", "auto", "class",
						"operator", "case", "do", "long", "typedef", "static", "firend", "template",
						"default", "new", "void", "return", "inline", "try", "short", "continue",
						"sizeof", "switch", "private", "struct", "delete", "public", "catch","printf"
};				//关键字表
const int BOUNDARYLIST_LENGTH = 27;
const string boundaryList[] = { "+", "-", "*", "/", "++", "--", "+=", "*=", "/=", "%", "%=", ";", "(",
							")", "{", "}", "[", "]", "&", "|", "&&", "||", "<", ">", "<<", ">>", "!=",
};		//界符表
Token::Token(char t, string c, int r) {
	type = t;
	content = c;
	row = r;
}
Token::Token() {}
void Token::set(char t, string c, int r) {
	type = t;
	content = c;
	row = r;
}

void Recognizer::init(string f) {
	this->ifs.open(f);
	if (!ifs.is_open()) {
		cout << "扫描器打开文件失败" << endl;
		exit(1);
		system("pause");
	}
	state = 1;		//自动机的状态初始化成1
}

void Recognizer::getAChar() {
	ifs.get(ch);
	if (ifs.eof()) {
		ch = '$';
	}
}

void Recognizer::pushbackAChar() {
	ifs.seekg(-1, ios::cur);
}

bool Recognizer::isALetter() {
	if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z') {
		return true;
	}
	else {
		return false;
	}
}

bool Recognizer::isANumber() {
	if (ch >= '0' && ch < '9') {
		return true;
	}
	else {
		return false;
	}
}

string Recognizer::getAWord() {
	while (true) {
		switch (state) {
		case 1:
			keyOrIdentifier = "";
			number = "";
			String = "";
			c = "";
			getAChar();
			if (ch == ' ' || ch == '\n' || ch == '\t') {
				if (ch == '\n') {
					row++;
				}
				break;
			}
			else if (isALetter()) {
				state = 2;
				break;
			}
			else if (ch == '\"') {
				state = 4;
				break;
			}
			else if (ch == '\'') {
				state = 7;
				break;
			}
			else if (isANumber()) {
				state = 11;
				pushbackAChar(); //进入常数处理机之前回退一个字符
				numState = 1;
				break;
			}
			else if (ch == '=') {
				state = 12;
				break;
			}
			else if (ch == '<') {
				state = 16;
				break;
			}
			else if (ch == '>') {
				state = 22;
				break;
			}
			else if (ch == ';') {
				type = 5;
				return ";";
			}
			else if (ch == '(') {
				type = 5;
				return "(";
			}
			else if (ch == ')') {
				type = 5;
				return ")";
			}
			else if (ch == '{') {
				type = 5;
				return "{";
			}
			else if (ch == '}') {
				type = 5;
				return "}";
			}
			else if (ch == '[') {
				type = 5;
				return "[";
			}
			else if (ch == ']') {
				type = 5;
				return "]";
			}
			else if (ch == ',') {
				type = 5;
				return ",";
			}
			else if (ch == '.') {
				type = 5;
				return ".";
			}
			else if (ch == ':') {
				type = 5;
				return ":";
			}
			else if (ch == '~') {
				type = 5;
				return "~";
			}
			else if (ch == '+') {
				state = 27;
				break;
			}
			else if (ch == '-') {
				state = 31;
				break;
			}
			else if (ch == '*') {
				state = 35;
				break;
			}
			else if (ch == '/') {
				state = 38;
				break;
			}
			else if (ch == '&') {
				state = 41;
				break;
			}
			else if (ch == '|') {
				state = 44;
				break;
			}
			else if (ch == '!') {
				state = 47;
				break;
			}
			else if (ch == '$') {
				type = 6;
				return "$";
			}
			else {
				cout << "遇到非法符号:" << ch << endl;
				return "Meet a forbidden char";
			}
		case 2:
			keyOrIdentifier += ch;
			getAChar();
			if (isALetter() || isANumber()) {
				break;
			}
			else {
				state = 3;
				pushbackAChar();
				break;
			}
		case 3:
			state = 1;
			type = 1;
			return keyOrIdentifier;
		case 4:
			String += ch;
			getAChar();
			if (ch == '"') {
				state = 5;
			}
			else if (ch == '\\') {
				state = 6;
			}
			break;
		case 6:
			String += ch;
			getAChar();
			state = 4;
			break;
		case 5:
			String += ch;
			state = 1;
			type = 2;
			return String;
		case 7:
			c += ch;
			getAChar();
			if (ch != '\\') {
				state = 9;
			}
			else {
				state = 8;
			}
			break;
		case 8:
			c += ch;
			getAChar();
			state = 9;
			break;
		case 9:
			c += ch;
			getAChar();
			if (ch != '\'') {
				cout << "在识别字符时出错" << endl;
				return "Error when recongize a char";
			}
			state = 10;
			break;
		case 10:
			c += ch;
			state = 1;
			type = 3;
			return c;
		case 11:
			while (numState != 8) {
				if (numState == 1) {
					n = p = m = t = 0;
					e = 1; num = 0;
					getAChar();
					if (isANumber()) {
						numState = 2;
					}
					continue;
				}
				else if (numState == 2) {
					n = n * 10 + ch - '0';
					getAChar();
					if (isANumber()) {
						continue;
					}
					else if (ch == '.') {
						numState = 3;
					}
					else if (ch == 'e') {
						numState = 5;
					}
					else {
						numState = 8;
					}
					continue;
				}
				else if (numState == 3) {
					t = 1;
					getAChar();
					if (isANumber()) {
						numState = 4;
						continue;
					}
					else {
						cout << "在识别常数时出错,小数点后不是数字";
						exit(1);
					}
				}
				else if (numState == 4) {
					n = 10 * n + ch - '0';
					m += 1;
					getAChar();
					if (isANumber()) {
						numState = 4;
					}
					else if (ch == 'e') {
						numState = 5;
					}
					else {
						numState = 8;
					}
					continue;
				}
				else if (numState == 5) {
					t = 1;
					getAChar();
					if (isANumber()) {
						numState = 7;
					}
					else if (ch == '-' || ch == '+') {
						numState = 6;
					}
					else {
						cout << "在识别常数时出错，e后面出现的不是数字，-，+";
						exit(1);
					}
					continue;
				}
				else if (numState == 6) {
					if (ch == '-') {
						e = -1;
					}
					getAChar();
					if (isANumber()) {
						numState = 7;
					}
					else {
						cout << "在识别常数时出错，+,-号后面不是数字";
						exit(1);
					}
					continue;
				}
				else if (numState == 7) {
					p = 10 * p + ch - '0';
					getAChar();
					numState = 8;
					continue;
				}
			}
			if ((int)ch != -1) {
				num = n * pow(10, e*p - m);
				state = 1;
				pushbackAChar();
				type = 4;
				return "It's a number";  // 计算工作交给扫描器
			}
			else {
				num = n * pow(10, e*p - m);
				state = 1;
				type = 4;
				return "It's a number";  // 计算工作交给扫描
			}
		case 12:
			getAChar();
			if (ch == '=') {
				state = 14;
			}
			else {
				state = 13;
				pushbackAChar();
			}
			break;
		case 13:
			//pushbackAChar();
			state = 1;
			type = 5;
			return "=";
		case 14:
			state = 1;
			type = 5;
			return "==";
		case 16:
			getAChar();
			if (ch == '<') {
				state = 18;
			}
			else if (ch == '=') {
				state = 20;
			}
			else {
				state = 17;
				pushbackAChar();
			}
			break;
		case 17:
			//pushbackAChar();
			state = 1;
			type = 5;
			return "<";
		case 18:
			state = 1;
			type = 5;
			return "<<";
		case 20:
			state = 1;
			type = 5;
			return "<=";
		case 22:
			getAChar();
			if (ch == '>') {
				state = 24;
			}
			else if (ch == '=') {
				state = 26;
			}
			else {
				state = 23;
				pushbackAChar();
			}
			break;
		case 23:
			//pushbackAChar();
			state = 1;
			type = 5;
			return ">";
		case 24:
			state = 1;
			type = 5;
			return ">>";
		case 26:
			state = 1;
			type = 5;
			return ">=";
		case 27:
			getAChar();
			if (ch == '=') {
				state = 28;
			}
			else if (ch == '+') {
				state = 29;
			}
			else {
				state = 30;
				pushbackAChar();
			}
			break;
		case 28:
			state = 1;
			type = 5;
			return "+=";
		case 29:
			state = 1;
			type = 5;
			return "++";
		case 30:
			state = 1;
			type = 5;
			return "+";
		case 31:
			getAChar();
			if (ch == '=') {
				state = 32;
			}
			else if (ch == '-') {
				state = 33;
			}
			else {
				state = 34;
				pushbackAChar();
			}
			break;
		case 32:
			state = 1;
			type = 5;
			return "-=";
		case 33:
			state = 1;
			type = 5;
			return "--";
		case 34:
			state = 1;
			type = 5;
			return "-";
		case 35:
			getAChar();
			if (ch == '=') {
				state = 36;
			}
			else {
				state = 37;
				pushbackAChar();
			}
			break;
		case 36:
			state = 1;
			type = 5;
			return "*=";
		case 37:
			state = 1;
			type = 5;
			return "*";
		case 38:
			getAChar();
			if (ch == '=') {
				state = 39;
			}
			else {
				state = 40;
				pushbackAChar();
			}
			break;
		case 39:
			state = 1;
			type = 5;
			return "/=";
		case 40:
			state = 1;
			type = 5;
			return "/";
		case 41:
			getAChar();
			if (ch == '&') {
				state = 42;
			}
			else {
				state = 43;
				pushbackAChar();
			}
			break;
		case 42:
			state = 1;
			type = 5;
			return "&&";
		case 43:
			state = 1;
			type = 5;
			return "&";
		case 44:
			getAChar();
			if (ch == '|') {
				state = 45;
			}
			else {
				state = 46;
				pushbackAChar();
			}
			break;
		case 45:
			state = 1;
			type = 5;
			return "||";
		case 46:
			state = 1;
			type = 5;
			return "|";
		case 47:
			getAChar();
			if (ch == '=') {
				state = 49;
			}
			else {
				pushbackAChar();
				state = 48;
			}
			break;
		case 48:
			type = 5;
			state = 1;
			return "!";
		case 49:
			type = 5;
			state = 1;
			return "!=";
		}
	}
}

void Lexer::init(string name) {
	ofstream ofs("C:\\byresult\\tokens.txt");
	ofs.clear();
	rec.init(name);
}
Token Lexer::next_Token() {			//根据扫描器提供的单词生成Token
	currentWord = rec.getAWord();
	Token token;
	ofstream ofs("C:\\byresult\\tokens.txt", ios::app);
	switch (rec.type) {
	case 1:
		for (unsigned int i = 0; i < KEYLIST_LENGTH; i++) {
			if (currentWord == keyList[i]) {
				token.set('k', currentWord, row);
				ofs << token.content << "\t\t" << token.type << "\t" << token.row << endl;
				return token;
			}
		}
		token.set('i', currentWord, row);
		ofs << token.content << "\t\t" << token.type << "\t" << token.row << endl;
		return token;
	case 2:									//是字符串
		token.set('s', currentWord, row);
		ofs << token.content << "\t\t" << token.type << "\t" << token.row << endl;
		return token;
	case 3:									//是字符
		token.set('c', currentWord, row);
		ofs << token.content << "\t\t" << token.type << "\t" << token.row << endl;
		return token;
	case 4:									//是数字常数
		if (rec.t == 1) {
			token.set('f', to_string(rec.num), row);//浮点型常数
			ofs << token.content << "\t\t" << token.type << "\t" << token.row << endl;
			return token;
		}
		else if (rec.t == 0) {
			token.set('n', to_string(rec.num), row); //n整形常数
			ofs << token.content << "\t\t" << token.type << "\t" << token.row << endl;
			return token;
		}
		break;
	case 5:									//是界符
		token.set('p', currentWord, row);		//生成界符token
		ofs << token.content << "\t\t" << token.type << "\t" << token.row << endl;
		return token;
	case 6:
		token.set('e', "$", row);
		ofs << token.content << "\t\t" << token.type << "\t" << token.row << endl;
		return token;
	}
}