#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

enum type_of_lex {                                                                                 
    LEX_NULL,                                                                                   
    LEX_AND, LEX_BOOLEAN, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT,       
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_TRUE, LEX_STRING, LEX_WRITE, LEX_CONTINUE,
    LEX_FIN,                                                                                    
    LEX_EQ, LEX_PERC, LEX_SEMICOLON, LEX_COMMA, LEX_LCBRACE, LEX_RCBRACE, LEX_LPAREN, LEX_RPAREN, LEX_QOUTE, LEX_ASSIGN, LEX_LSS,   /*27*/
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ,               
    LEX_NUM,                                                                                    
    LEX_ID,
    LEX_STRCONST
};

class Lexem {
    type_of_lex t_lex;
    int v_lex;
public:
    Lexem(type_of_lex t = LEX_NULL, int v = 0) { 
        t_lex = t;
        v_lex = v;
    }
    type_of_lex get_type() const { 
    	return t_lex; 
    }
    int get_value() const { 
    	return v_lex; 
    }

    friend ostream &operator<< (ostream &s, Lexem l);
};

class Id {
    string name;
public:
    Id(const string n) {
        name = n;
    }

    bool operator== (const string& s) const { //используем это при поиске эл-а в векторе
        return name == s; 
    }
    string get_name() const { 
    	return name; 
    }
};
 
vector<Id> TID;
 
int set(const string &buf) {
    for(auto i = TID.begin(); i != TID.end(); ++i) {
        if(*i == buf) return i - TID.begin();
    }
    cout << buf;
    TID.push_back(Id(buf));
    return TID.size() - 1;
}
 
class Scanner {
    FILE *f;
    char c;
    bool is_first_qoute = true, is_final = false;
    int look(const string buf, const char **list) {
        int i = 0;
        while(list[i]) {
            if(list[i] == buf) return i;
            i++;
        }
        return 0;
    }

    Lexem get_lex();
public:
    static const char *TW[], *TD[];

    Scanner(const char *program) {
        if (!(f = fopen(program, "r"))) 
            throw "can’t open file";
       	else cout << "successful file opening\n" << endl;
    }

    void get_all() {
    	Lexem s;
        cout << "---LEXEM TABLE---" << endl;
    	while((s = this->get_lex()).get_type() != LEX_FIN || !is_final)
            if(!is_final) cout << s;
    }
};
 
const char
*Scanner::TW[] = {"", "and", "boolean", "else", "if", "false", "int", "not", "or", "program",
                      "read", "true", "string", "write", "continue", NULL};
 
const char
*Scanner::TD[] = {"==", "%", ";", ",", "{", "}", "(", ")", "\"", "=", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", NULL};
 
Lexem Scanner::get_lex() {
    enum state {NEW, IDENT, NUMB, COM, END_COM, SLASH, LGE, NEQ, QOUTE};
    int num, i;
    
    string buf;
    state CUR_ST = NEW;
    do {
        c = fgetc(f);
        if(c == EOF) {
            is_final = true;
             return Lexem(LEX_FIN);                //конец файла
        }
        switch(CUR_ST) {
            case NEW:
                if (c == ' ' || c == '\n' || c == '\r' || c == '\t') continue; 
                else if(isalpha(c)) {
                    buf.push_back(c);
                    CUR_ST = IDENT;
                }
                else if(isdigit(c)) {
                    num = c - '0';
                    CUR_ST = NUMB;
                } 
                else if(c == '/') { 
                    CUR_ST = SLASH;
                }
                else if(c == '<' || c == '>' || c == '=') { 
                    buf.push_back(c);
                    CUR_ST = LGE; 
                }
                else if(c == '!') {
                    buf.push_back(c);
                    CUR_ST = NEQ;
                }
                else if(c == '"' && is_first_qoute) {
                    is_first_qoute = false;
                    CUR_ST = QOUTE;
                }
                else {                  //обработка спецсимволов
                    buf.push_back(c);
                    if((i = look(buf, TD))) {
                        return Lexem((type_of_lex)(i + (int)LEX_FIN), i);
                    } else throw c;
                }
                break;
            case IDENT:                         //обработка идентификаторов
                if(isalpha(c) || isdigit(c)) {
                    buf.push_back(c); 
                }
                else {
                    ungetc(c, f);
                    if((i = look(buf, TW))) {
                        return Lexem((type_of_lex)i, i);
                    } else {
                        i = set(buf);
                        return Lexem(LEX_ID, i);
                    }
                }
                break;
            case NUMB:                  //обработка числа
                if(isdigit(c)) {
                    num = num * 10 + (c - '0');
                } else {
                    ungetc(c, f);
                    return Lexem(LEX_NUM, num);
                }
                break;
            case SLASH:         //проверяем, является ли комментарием
            	if(c == '*') {
            		CUR_ST = END_COM;
            	} else {        //обрабатываем как простой слэш
                    i = look(buf, TD);
                    return Lexem((type_of_lex)(i + (int)LEX_FIN), i);
                }
            	break;
            case COM:
                if (c == '*') {
                    CUR_ST = END_COM;
                }
                break;
            case END_COM:
                if (c == '/') {
                    CUR_ST = NEW;
                } else CUR_ST = COM;
                break;
            case LGE:
                if (c == '=') {
                    buf.push_back(c);
                    i = look(buf, TD);
                    return Lexem((type_of_lex)(i + (int)LEX_FIN), i);
                } else if(c == ' ') {
                    ungetc(c, f);
                    i = look(buf, TD);
                    return Lexem((type_of_lex)(i + (int)LEX_FIN), i);
                } else throw c;
                break;
            case NEQ:
                if (c == '=') {
                    buf.push_back(c);
                    i = look(buf, TD);
                    return Lexem((type_of_lex)(i + (int)LEX_FIN), i);
                } else throw '!';
                break;
            case QOUTE:             //обработка простых кавычек вида "..." без вложений
                if(c == '"') {
                    i = set(buf);
                    is_first_qoute = true;
                    return Lexem(LEX_STRCONST, i);
                } else {
                    buf.push_back(c);
                }
                break;
        }
    } while (true);
}
 
ostream & operator<< (ostream &s, Lexem l) {
    string t, table = "table of id and consts";
    if(l.t_lex <= LEX_CONTINUE) {
        t = Scanner::TW[l.t_lex];
        table = "table of words";
    }
    else if(l.t_lex >= LEX_FIN && l.t_lex <= LEX_GEQ) {
        t = Scanner::TD[l.t_lex - LEX_FIN];
        table = "table of dividers";
    }
    else if(l.t_lex == LEX_NUM) t = "NUMBER";
    else if(l.t_lex == LEX_ID) t = TID[l.v_lex].get_name();
    else if(l.t_lex == LEX_STRCONST) t = TID[l.v_lex].get_name();           
    else throw l;
    s << "From " << table << ": (\'" << t << "\', " << l.v_lex << ");" << endl;
    return s;
}

int main(int argc, char* argv[]) {
	Scanner sn(argv[1]);
	try {
		sn.get_all();
	}
	catch (char c) {
        cout << "unexpected symbol " << c << endl;
        return 1;
    }
    catch (Lexem l) {
        cout << "unexpected lexeme " << l << endl;
        return 1;
    }
    catch(...) {
        cout << "unexpected behaviour" << endl;
        return 1;
    }
	return 0;
}