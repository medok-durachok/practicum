#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

enum type_of_lex {                                                                                 
    LEX_NULL,       //0                                                                                   
    LEX_AND, LEX_BOOLEAN, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT,   //6    
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_TRUE, LEX_STRING, LEX_WRITE, LEX_CONTINUE,
    LEX_FIN,  //16                                                                                  
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
    bool declare, assign;
    type_of_lex type;
public:
    Id(const string n) {
        name = n;
        declare = false; 
        assign  = false;
    }

    bool operator== (const string &s) const { //используем это при поиске эл-а в векторе
        return name == s; 
    }
    string get_name() const { return name; }

    void set_declare() { declare = true; }

    bool get_declare() const { return declare; }

    void set_assign() { assign = true; }

    bool get_assign() const { return assign; }

    void set_type(type_of_lex t) { type = t; }

    type_of_lex get_type() const { return type; }
};
 
vector<Id> TID;
 
int set(const string &buf) {
    for(auto i = TID.begin(); i != TID.end(); ++i) {
        if(*i == buf) return i - TID.begin();
    }
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
    char gc () {
        return (c = fgetc(f));
    }
public:
    static const char *TW[], *TD[];

    Scanner(const char *program) {
        if (!(f = fopen(program, "r"))) 
            throw "can’t open file";
       	//else cout << "successful file opening\n" << endl;
    }

    void get_all() {
    	Lexem s;
        cout << "---LEXEM TABLE---" << endl;
    	while((s = this->get_lex()).get_type() != LEX_FIN)
            cout << s;
    }

    bool get_is_final() const { return is_final; }

    Lexem get_lex();
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
        gc();
        //cout << c << endl;
        switch(CUR_ST) {
            case NEW:
                if(c == EOF) {
                    is_final = true;
                    return Lexem(LEX_FIN, LEX_FIN);
                }
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
                        return Lexem((type_of_lex)(i + (int)LEX_FIN + 1), i);
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
                        cout << (type_of_lex)i << endl;
                        return Lexem((type_of_lex)i, i);
                    } else {
                        i = set(buf);
                        //cout << LEX_ID << " " << i << endl;
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
                    return Lexem((type_of_lex)(i + (int)LEX_FIN + 1), i);
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
                    return Lexem((type_of_lex)(i + (int)LEX_FIN + 1), i);
                } else if(c == ' ') {
                    ungetc(c, f);
                    i = look(buf, TD);
                    return Lexem((type_of_lex)(i + (int)LEX_FIN + 1), i);
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
        t = Scanner::TD[l.t_lex - LEX_FIN - 1];
        table = "table of dividers";
    }
    else if(l.t_lex == LEX_NUM) t = "NUMBER";
    else if(l.t_lex == LEX_ID) t = TID[l.v_lex].get_name();
    else if(l.t_lex == LEX_STRCONST) t = TID[l.v_lex].get_name();           
    else throw l;
    s << "From " << table << ": (\'" << t << "\', " << l.v_lex << ");" << endl;
    return s;
}

template <class T, class T_EL>
void from_stack(T &st, T_EL &i) {
    i = st.top(); 
    st.pop();
}
 
class Parser {
    Lexem curr_lex;
    type_of_lex c_type;
    int c_val;
    Scanner scan;
    stack <int> st_int;
    stack <type_of_lex> st_lex;
    void Prog();
    void D1();
    void V();
    void A();
    void D();
    void B();
    void S();
    void E();
    void E1();
    void T();
    void F();
    void dec(type_of_lex type);
    void check_id();
    void check_id_in_read();
    void gl() {
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        if(c_type == LEX_FIN && scan.get_is_final()) {
            exit(EXIT_SUCCESS);
        }
        c_val = curr_lex.get_value();
        cout << curr_lex;
    }
public:
    Parser(const char *program) : scan(program) { }
    void analyze();
};
 
void Parser::analyze () {                   //первая запускаемая функция
    gl(); //получаем новую лексему и ее поля
    Prog(); // проверка на program
}
 
void Parser::Prog() {
    if (c_type == LEX_PROGRAM) gl();
    else throw curr_lex;
    
    if(c_type == LEX_LCBRACE) {
        gl();
        D1(); // объявление переменных разного типа
        B();
    }
    else throw curr_lex;
    if(c_type != LEX_RCBRACE) throw curr_lex;
    else {
        gl();
        if(c_type != LEX_FIN) {
            throw " Program finished with ERRORS ";
        }
    }
}

void Parser::D1(){
    while ((c_type == LEX_INT) || (c_type == LEX_BOOLEAN) || (c_type == LEX_STRING)) {
        D();
        if(c_type != LEX_SEMICOLON) {
            try {
                while ((c_type == LEX_INT) || (c_type == LEX_BOOLEAN) || (c_type == LEX_STRING)) {    
                    D(); 
                    if(c_type != LEX_SEMICOLON) throw curr_lex;
                    gl();
                }
            }
            catch(Lexem l) {
                gl();
            }
            catch(const char *l)
            {
                gl();
            }
        }
    }
}

void Parser::D() {
    type_of_lex tmp = c_type;
    if (c_type == LEX_INT || c_type == LEX_BOOLEAN || c_type == LEX_STRING) {
        //cout << 1111111111;
        gl();
        st_lex.push(tmp);
        if(c_type == LEX_ID) TID[c_val].set_type(tmp);
        V();

        dec(tmp);
        TID[c_val].set_declare();
        TID[c_val].set_type(tmp);
        V();

        while (c_type == LEX_COMMA) {
            gl();
            st_lex.push(tmp);
            if(c_type == LEX_ID) TID[c_val].set_type(tmp);

            V();
            try {
                if(c_type == LEX_ID) {
                    if (TID[c_val].get_declare()) throw "twice";
                    TID[c_val].set_declare();
                    TID[c_val].set_type(tmp);
                }
            }
            catch(const char *l) {      
                while (c_type != LEX_COMMA) gl();
            }
            V();
        }
    }
}

void Parser::V() {
    int tmp_c_val = c_val;
    if (c_type == LEX_ID) {   
        st_int.push(c_val);
        gl();
        if (c_type == LEX_ASSIGN) {
            st_lex.push(TID[tmp_c_val].get_type());
            st_lex.push(LEX_ID);
            tmp_c_val = c_val;
            gl();
            A();        
        }
    } else {
        if ( c_type == LEX_COMMA || c_type == LEX_SEMICOLON){}
        else throw curr_lex;
    }
}

void Parser::A() {
    //cout << 33333;
    if(c_type == LEX_STRCONST) {
        st_lex.push(LEX_STRCONST);
        gl();
    }
    else if (c_type == LEX_TRUE) {
        st_lex.push(LEX_BOOLEAN);
        gl();
    }
    else if (c_type == LEX_FALSE) {
        st_lex.push(LEX_BOOLEAN);
        gl();
    }
    else if (c_type == LEX_NUM) {
        st_lex.push(LEX_INT);
        gl();
    }
    else throw curr_lex;
}
 
void Parser::B() {
    //cout << "im in B" << endl;
    while(c_type != LEX_RCBRACE)
    while(c_type != LEX_RCBRACE && c_type != LEX_FIN) {
        S();
    }
}

void Parser::S() {
    if (c_type == LEX_LCBRACE) gl();
    else if (c_type == LEX_RCBRACE) gl();
    else if(c_type == LEX_IF) {
        gl();
        E();
        if(c_type == LEX_LCBRACE) {
            gl();
            S();
            if (c_type != LEX_SEMICOLON) throw curr_lex;
            gl();
            if (c_type != LEX_RCBRACE) throw curr_lex;
            gl();
            if(c_type == LEX_ELSE) {
                gl();
                if(c_type == LEX_LCBRACE) {
                    gl();
                    S();
                }
                if (c_type != LEX_SEMICOLON) throw curr_lex;

                gl();
                if (c_type != LEX_RCBRACE) throw curr_lex;
                gl();
            }
            else S();
        }
        else throw curr_lex;
    }
    else if(c_type == LEX_READ) {
        gl();
        if(c_type == LEX_LPAREN) {
            gl();
            if(c_type == LEX_ID) {
                check_id_in_read();
                gl();
            } else throw curr_lex;
            if(c_type == LEX_RPAREN) gl();
            else throw curr_lex;
        }
        else throw curr_lex;
    }
    else if(c_type == LEX_WRITE) {   
        gl();
        if(c_type == LEX_LPAREN) {
            gl();
            E();
            if (c_type == LEX_RPAREN) gl();
            else throw curr_lex;
        } else throw curr_lex;
    }
    else if(c_type == LEX_ID) {
        check_id();
        type_of_lex tmp = TID[c_val].get_type();
        st_lex.push(tmp);
        st_lex.push(LEX_ID);

        gl();
        if(c_type == LEX_ASSIGN) {
            gl();
            E();
        }
    }
    else if(c_type == LEX_INT) D();
    else if (c_type == LEX_SEMICOLON) gl();
    //else 
       // throw curr_lex;
}

void Parser::E() {
    //cout << "im in E" << endl;
    E1();                  // самый низкий приоритет операций проверяем тут
    if(c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR ||
         c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ ) {
        st_lex.push(c_type);
        gl(); 
        E1(); 
    }
}
 
void Parser::E1() {
    //cout << "im in E1" << endl;
    T ();           
    while(c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR) {
        st_lex.push(c_type);
        gl();
        T();
    }
}
 
void Parser::T() {
    //cout << "im in T" << endl;
    F();       // первый объект 
    while(c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND) {
        st_lex.push(c_type);
        gl();
        F();               // по надобности второй
    }
}
 
void Parser::F() {
    cout << "im in F" << endl;
    if(c_type == LEX_ID) {          
        check_id();                // ид проверяем на объявл
        gl();
    }
    else if(c_type == LEX_NUM) {
        st_lex.push(LEX_INT);            // число просто вносим
        gl();
    }
    else if(c_type == LEX_STRCONST) {
        st_lex.push(LEX_STRCONST);            // число просто вносим
        gl();
    }
    else if(c_type == LEX_TRUE) {
        st_lex.push(LEX_BOOLEAN);
        gl ();
    }
    else if(c_type == LEX_FALSE) {
        st_lex.push(LEX_BOOLEAN);
        gl();
    }
    else if(c_type == LEX_NOT) {
        gl(); 
        F(); 
    }
    else if(c_type == LEX_LPAREN) {                      // скобки. в скобках запускаем еще раз Е и заново все проходим
        gl(); 
        E();
        if (c_type == LEX_RPAREN)
            gl();
        else throw curr_lex;
    }
   // else 
       // throw curr_lex;
}
 
void Parser::dec(type_of_lex type) {             // объявление и переобъявление 
    int i;
    while(!st_int.empty()) {
        from_stack(st_int, i);
        if(TID[i].get_declare()) throw "twice";
        else {
            TID[i].set_declare();
            TID[i].set_type(type);
        }
    }
}
 
void Parser::check_id() {
    if(TID[c_val].get_declare())
        st_lex.push(TID[c_val].get_type());
    else
        cout << "im not declared" << endl;
        //throw "not declared";
}
 
void Parser::check_id_in_read() {
    if(!TID [c_val].get_declare()) throw "not declared";
}

int main(int argc, char* argv[]) {
	Scanner sn(argv[1]);
    Parser pars(argv[1]);
	try {
        //sn.get_all();
        //  cout << endl;
		pars.analyze();
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