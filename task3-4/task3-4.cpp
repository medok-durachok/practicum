#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

enum type_of_lex {                                                                                 
    LEX_NULL,       //0                                                                                   
    LEX_AND, LEX_BOOLEAN, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT,   //6    
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_TRUE, LEX_STRING, LEX_WRITE, LEX_CONTINUE,
    LEX_FIN,  //15                                                                                 
    LEX_EQ, LEX_PERC, LEX_SEMICOLON, LEX_COMMA, LEX_LCBRACE, LEX_RCBRACE, LEX_LPAREN, LEX_RPAREN, LEX_QOUTE, LEX_ASSIGN, LEX_LSS,   //26
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_MINUSEQ, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ,               
    LEX_NUM,                                                                                    
    LEX_ID,
    LEX_STRCONST,//38
    POLIZ_LABEL, POLIZ_ADDRESS, POLIZ_GO, POLIZ_FGO
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
    int value;
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

    int  get_value() const { return value; }

    void set_value(int v) { value = v; }
};
 
vector<Id> TID;
vector<string> TSTR;
 
int set(const string &buf) {
    for(auto i = TID.begin(); i != TID.end(); ++i) {
        if(*i == buf) return i - TID.begin();
    }
    TID.push_back(Id(buf));
    return TID.size() - 1;
}

int setstr(const string &buf) {
    for(auto i = TSTR.begin(); i != TSTR.end(); ++i) {
        if(*i == buf) return i - TSTR.begin();
    }
    TSTR.push_back(string(buf));
    return TSTR.size() - 1;
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
        if (!(f = fopen(program, "r"))) {
            cout << "can’t open file";
            throw "can’t open file";
        }
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
*Scanner::TD[] = {"==", "%", ";", ",", "{", "}", "(", ")", "\"", "=", "<", ">", "+", "-", "-=", "*", "/", "<=", "!=", ">=", NULL};
 
Lexem Scanner::get_lex() {
    enum state {NEW, IDENT, NUMB, COM, END_COM, SLASH, LGE, NEQ, QOUTE, MIN};
    int num, i;
    
    string buf;
    state CUR_ST = NEW;
    do {
        gc();
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
                else if(c == '-') {
                    buf.push_back(c);
                    CUR_ST = MIN;
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
                    return Lexem((type_of_lex)(i + (int)LEX_FIN + 1), i);
                }
            	break;
            case MIN:
                if (c == '=') {
                    buf.push_back(c);
                    i = look(buf, TD);
                    return Lexem(LEX_MINUSEQ, i);
                } else if(c == ' ') {
                    ungetc(c, f);
                    i = look(buf, TD);
                    return Lexem((type_of_lex)(i + (int)LEX_FIN + 1), i);
                } else throw c;
                break;
            case COM:
                if (c == '*') {
                    CUR_ST = END_COM;
                }
                break;
            case END_COM:
                if (c == '/') {
                    CUR_ST = NEW;
                } else {
                    CUR_ST = COM;
                }
                break;
            case LGE:
                if (c == '=') {
                    buf.push_back(c);
                    i = look(buf, TD);
                    if(buf[0] == '<') return Lexem(LEX_LEQ, i);
                    if(buf[0] == '>') return Lexem(LEX_GEQ, i);
                    if(buf[0] == '=') return Lexem(LEX_EQ, i);
                    return Lexem((type_of_lex)(i + (int)LEX_FIN), i);
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
                    return Lexem(LEX_NEQ, i);
                } else throw '!';
                break;
            case QOUTE:             //обработка простых кавычек вида "..." без вложений
                if(c == '"') {
                    i = setstr(buf);
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
    string t, table = "poliz table";
    if(l.t_lex <= LEX_CONTINUE) {
        t = Scanner::TW[l.t_lex];
        table = "table of words";
    }
    else if(l.t_lex >= LEX_FIN && l.t_lex <= LEX_GEQ) {
        t = Scanner::TD[l.t_lex - LEX_FIN - 1];
        table = "table of dividers";
    }
    else if(l.t_lex == LEX_NUM) t = "NUMBER";
    else if(l.t_lex == LEX_ID) {
        t = TID[l.v_lex].get_name();
        table = "table of identifiers";
    } 
    else if(l.t_lex == LEX_STRCONST) {
        table = "table of string consts";
        t = TSTR[l.v_lex]; 
    }
    else if(l.t_lex == POLIZ_LABEL)t = "Label";
    else if(l.t_lex == POLIZ_ADDRESS) t = "Addr";
    else if(l.t_lex == POLIZ_GO) t = "!";
    else if(l.t_lex == POLIZ_FGO) t = "!F";          
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
    void  check_op();
    void  check_not();
    void  eq_type();
    void  eq_bool();
    void gl() {
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
    }
public:
    vector<Lexem> poliz;
    Parser(const char *program) : scan(program) { }
    void analyze();
};
 
void Parser::analyze () {                   //первая запускаемая функция
    gl(); //получаем новую лексему и ее поля
    Prog(); // проверка на program
    if (c_type != LEX_FIN)
        throw curr_lex;
    //for(Lexem l : poliz) cout << l;
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
            cout << "Program finished with ERRORS";
            throw "Program finished with ERRORS";
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
                    poliz.push_back(Lexem(LEX_SEMICOLON, LEX_SEMICOLON));
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
            dec(tmp);
            try {
                if(c_type == LEX_ID) {
                    if (TID[c_val].get_declare()) {
                        cout << "declared twice";
                        throw "twice";
                    }
                    TID[c_val].set_declare();
                    TID[c_val].set_type(tmp);
                }
            }
            catch(const char *l) {      
                while(c_type != LEX_COMMA) gl();
            }
            V();
        }
    }
}

void Parser::V() {
    int tmp_c_val = c_val;
    if(c_type == LEX_ID) {       
        st_int.push(c_val);
        gl();
        if(c_type == LEX_ASSIGN) {
            st_lex.push(LEX_ID);
            st_lex.push(TID[tmp_c_val].get_type());
            poliz.push_back(Lexem(POLIZ_ADDRESS, tmp_c_val));   
            tmp_c_val = c_val;
            gl();
            A();     
            poliz.push_back(Lexem(LEX_ASSIGN, LEX_ASSIGN)); 
        }
    } else {
        if(c_type == LEX_COMMA || c_type == LEX_SEMICOLON) { }
        else throw curr_lex;
    }
}

void Parser::A() {
    if(c_type == LEX_STRCONST) {
        st_lex.push(LEX_STRCONST);
        poliz.push_back(curr_lex);
        gl();
    }
    else if(c_type == LEX_TRUE) {
        st_lex.push(LEX_BOOLEAN);
        eq_type();
        poliz.push_back(Lexem(LEX_TRUE, LEX_TRUE));
        gl();
    }
    else if(c_type == LEX_FALSE) {
        st_lex.push(LEX_BOOLEAN);
        eq_type();
        poliz.push_back(Lexem(LEX_FALSE, LEX_FALSE));
        gl();
    }
    else if(c_type == LEX_NUM) {
        st_lex.push(LEX_INT);
        eq_type();
        poliz.push_back(curr_lex);
        gl();
    }
    else throw curr_lex;
}
 
void Parser::B() {
    while(c_type != LEX_RCBRACE) {
        while(c_type != LEX_RCBRACE && c_type != LEX_FIN) {
            S();
        }
    }
}

void Parser::S() {
    int pl0, pl1, pl2, pl3;
    if (c_type == LEX_LCBRACE) gl();
    else if(c_type == LEX_RCBRACE) gl();
    else if(c_type == LEX_IF) {
        gl();
        E();

        eq_bool();          
        pl2 = poliz.size();
        poliz.push_back(Lexem());
        poliz.push_back(Lexem(POLIZ_FGO));

        if(c_type == LEX_LCBRACE) {
            gl();
            S();

            if(c_type != LEX_SEMICOLON) throw curr_lex;
            poliz.push_back(Lexem(LEX_SEMICOLON, LEX_SEMICOLON));
            gl();      

            pl3 = poliz.size(); 
            if (c_type != LEX_RCBRACE) throw curr_lex;
            gl();
            if(c_type == LEX_ELSE) {
                gl();
                if(c_type == LEX_LCBRACE) {
                    poliz.push_back(Lexem());
                    poliz.push_back(Lexem(POLIZ_GO, POLIZ_GO));
                    poliz[pl2] = Lexem(POLIZ_LABEL, poliz.size());
                    gl();
                    S();
                    poliz[pl3] = Lexem(POLIZ_LABEL, poliz.size());
                }
                if(c_type != LEX_SEMICOLON) throw curr_lex;
                poliz.push_back(Lexem(LEX_SEMICOLON, LEX_SEMICOLON));
                gl();

                if (c_type != LEX_RCBRACE) throw curr_lex;
                gl();
            }
            else {
                poliz[pl2] = Lexem(POLIZ_LABEL, poliz.size());
                S();
            }
        }
        else throw curr_lex;
    }
    else if(c_type == LEX_READ) {
        gl();
        if(c_type == LEX_LPAREN) {
            gl();
            if(c_type == LEX_ID) {
                check_id_in_read();

                poliz.push_back(Lexem(POLIZ_ADDRESS, c_val)); 

                gl();
            } else throw curr_lex;
            if(c_type == LEX_RPAREN) {
                gl();
                poliz.push_back(Lexem(LEX_READ)); 
            }
            else throw curr_lex;
        }
        else throw curr_lex;
    }
    else if(c_type == LEX_WRITE) {   
        gl();
        if(c_type == LEX_LPAREN) {
            gl();
            E();
            if (c_type == LEX_RPAREN) {
                gl();
                poliz.push_back(Lexem(LEX_WRITE)); 
            }
            else throw curr_lex;
        } else throw curr_lex;
    }
    else if(c_type == LEX_ID) {
        check_id();
        poliz.push_back(Lexem(POLIZ_ADDRESS, c_val)); 
        type_of_lex tmp = TID[c_val].get_type();
        st_lex.push(LEX_ID);
        st_lex.push(tmp);

        gl();
        if(c_type == LEX_ASSIGN) {
            gl();
            E();

            eq_type(); 
            poliz.push_back(Lexem(LEX_ASSIGN));
        } else if(c_type == LEX_MINUSEQ) {
            if(tmp == LEX_BOOLEAN) {
                cout << "can't do uno minus with bool";
                throw "can't do uno minus with bool";
            }
            if(tmp != LEX_STRING) {
                gl();
                E();

                eq_type();
                poliz.push_back(Lexem(LEX_MINUSEQ, LEX_MINUSEQ));
            }
            else {
                cout << "can't do uno minus with string";
                throw "can't do uno minus with string";
            }
        }
    }
    else if(c_type == LEX_CONTINUE) gl();
    else if(c_type == LEX_INT || c_type == LEX_BOOLEAN || c_type == LEX_STRING) D();
    else if (c_type == LEX_SEMICOLON) {
        poliz.push_back(Lexem(LEX_SEMICOLON, LEX_SEMICOLON));
        gl();
    }
    else throw curr_lex;
}

void Parser::E() {
    E1();                  // самый низкий приоритет операций проверяем тут
    if(c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR ||
         c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ || c_type == LEX_ASSIGN || c_type == LEX_MINUSEQ) {
        int tmp = c_type;
        /*if(tmp != LEX_ASSIGN)*/ st_lex.push(c_type);
        gl();
        E1(); 
        if(tmp == LEX_ASSIGN) eq_type();
        if(tmp != LEX_MINUSEQ) check_op(); 
    }
}
 
void Parser::E1() {
    T ();           
    while(c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR) {
        st_lex.push(c_type);
        gl();
        T();
        check_op();
    }
}
 
void Parser::T() {
    //cout << "im in T" << endl;
    F();       // первый объект 
    while(c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND) {
        st_lex.push(c_type);
        gl();
        F();               // по надобности второй
        check_op();
    }
}
 
void Parser::F() {
    if(c_type == LEX_ID) {        
        check_id();                // ид проверяем на объявл
        type_of_lex tmp = TID[c_val].get_type();
        //st_lex.push(tmp);
        poliz.push_back(curr_lex); 
        gl();
    }
    else if(c_type == LEX_NUM) {
        st_lex.push(LEX_INT);            // число просто вносим
        poliz.push_back(curr_lex); 
        gl();
    }
    else if(c_type == LEX_STRCONST) {
        st_lex.push(LEX_STRING);            // 
        poliz.push_back(curr_lex); 
        gl();
    }
    else if(c_type == LEX_TRUE) {
        st_lex.push(LEX_BOOLEAN);
        poliz.push_back(Lexem(LEX_TRUE, 1)); 
        gl();
    }
    else if(c_type == LEX_FALSE) {
        st_lex.push(LEX_BOOLEAN);
        poliz.push_back(Lexem(LEX_FALSE, 0)); 
        gl();
    }
    else if(c_type == LEX_NOT) {
        gl(); 
        F(); 
        check_not();
    }
    else if(c_type == LEX_LPAREN) {                      // скобки. в скобках запускаем еще раз Е и заново все проходим
        gl(); 
        E();
        if (c_type == LEX_RPAREN)
            gl();
        else throw curr_lex;
    }
}
 
void Parser::dec(type_of_lex type) {             // объявление и переобъявление 
    int i;
    while(!st_int.empty()) {
        from_stack(st_int, i);
        if(TID[i].get_declare()) {
            cout << "declared twice";
            throw "twice";
        }
        else {
            TID[i].set_declare();
            TID[i].set_type(type);
        }
    }
}
 
void Parser::check_id() {
    if(TID[c_val].get_declare())
        st_lex.push(TID[c_val].get_type());
    else {
        cout << "im not declared";
        throw "not declared";
    }
}
 
void Parser::check_id_in_read() {
    if(!TID [c_val].get_declare()) {
        cout << "im not declared";
        throw "not declared";
    }
}

void Parser::check_op(){ 
    type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOLEAN, s = LEX_STRING;
 
    from_stack(st_lex, t2);
    from_stack(st_lex, op);
    from_stack(st_lex, t1);
 
    if (t1 == t2){
        if (t1 == LEX_INT) {
            if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH) r = LEX_INT;
            else if (op == LEX_AND || op == LEX_OR) {
                cout << "wrong types are in operation";
                throw "wrong types are in operation";
            }
        } else if(t1 == LEX_BOOLEAN) {
            if(op == LEX_AND || op == LEX_OR || op == LEX_EQ || op == LEX_NEQ || op == LEX_GTR || op == LEX_LSS || op == LEX_GEQ || op == LEX_LEQ) r = LEX_BOOLEAN;
            else {
                cout << "wrong types are in operation";
                throw "wrong types are in operation";
            }
        } else if (t1 == LEX_STRING) {
            if (op == LEX_EQ || op == LEX_NEQ) r = LEX_STRING;
            else {
                cout << "wrong types are in operation";
                throw "wrong types are in operation";
            }
        }
        st_lex.push(r);
    }
    //cout << "*" << op << endl;
    poliz.push_back(Lexem(op));
}
 
void Parser::check_not() {
    if(st_lex.top() != LEX_BOOLEAN) {
        cout << "wrong type is in not";
        throw "wrong type is in not";
    }
    else poliz.push_back(Lexem(LEX_NOT));
}
 
void Parser::eq_type() {
    type_of_lex t;
    from_stack(st_lex, t);
    if (t != st_lex.top()) {
        cout << "wrong type in assignment";
        throw "wrong type in assignment";
    }
    st_lex.pop();
}
 
void Parser::eq_bool() {
    if(st_lex.top() != LEX_BOOLEAN) {
        cout << "expression is not boolean";
        throw "expression is not boolean";
    }
    st_lex.pop();
}

class Executer {
public:
    void execute(vector<Lexem> &poliz);
};
 
void Executer::execute(vector<Lexem> &poliz) {
    Lexem pc_el;
    stack <int> args;
    stack <int> args_reverse;
    int i, j, k, index = 0, size = poliz.size();
    stack <string> args_str;
    stack <string> args_str_reverse;
    string str1, str2;

    while(index < size) {
        pc_el = poliz[index];
        switch(pc_el.get_type()) {
            case LEX_TRUE: case LEX_FALSE: case LEX_NUM: case POLIZ_ADDRESS: case POLIZ_LABEL:
                args.push(pc_el.get_value());
                break;
 
            case LEX_ID:
                i = pc_el.get_value();
                if(TID[i].get_assign()) {
                    if(TID[i].get_type() == LEX_STRING) {
                        str1 = TSTR[TID[i].get_value()];
                        args_str.push(str1);
                    }
                    args.push(TID[i].get_value());
                    break;
                }
                else {
                    cout << "indefinite identifier";
                    throw "indefinite identifier";
                }
                break;
 
            case LEX_NOT:
                from_stack(args, i);
                args.push(!i);
                break;
 
            case LEX_OR:
                from_stack(args, i); 
                from_stack(args, j);
                args.push (j || i);
                break;
 
            case LEX_AND:
                from_stack(args, i);
                from_stack(args, j);
                args.push(j && i);
                break;

            case POLIZ_GO:
                from_stack(args, i);
                index = i - 1;
                break;
 
            case POLIZ_FGO:
                from_stack(args, i);
                from_stack(args, j);
                if(!j) index = i - 1;
                break;
 
            case LEX_WRITE:
                while (!args_str.empty()) {
                    from_stack(args_str, str1);
                    from_stack(args, j);
                    args_str_reverse.push(str1);
                }
                while(!args.empty()) {
                    from_stack(args, j);
                    args_reverse.push(j);
                }

                while(!args_str_reverse.empty()) {
                    from_stack(args_str_reverse, str1);
                    cout << str1 << endl;
                }

                while(!args_reverse.empty()) {
                    from_stack(args_reverse, j);
                    cout << j << endl;
                }
                break;
 
            case LEX_READ:
                int m;
                from_stack(args, i);
                if (TID[i].get_type() == LEX_INT) {
                    cout << "Enter integer value for " << TID[i].get_name () << ": ";
                    cin >> m;
                } else if(TID[i].get_type() == LEX_STRING)
                {
                    cout << "Enter string value for " << TID[i].get_name () << ": ";
                    cin >> str1;
                    TSTR.push_back(str1);
                    m = TSTR.size() - 1;
                }
                else {
                    string j;
                    while(1) {
                        cout << "Input boolean value for " << TID[i].get_name() << ": ";
                        cin >> j;
                        if(j != "true" && j != "false") {
                            cout << "Error in input:true/false" << endl;
                            continue;
                        }
                        m = (j == "true") ? 1 : 0;
                        break;
                    }
                }
                TID[i].set_value(m);
                TID[i].set_assign();
                break;
 
            case LEX_PLUS:
                from_stack(args, i);
                from_stack(args, j);
                args.push(i + j);
                break;
 
            case LEX_TIMES:
                from_stack(args, i);
                from_stack(args, j);
                args.push(i * j);
                break;
 
            case LEX_MINUS:
                from_stack(args, i);
                from_stack(args, j);
                args.push(j - i);
                break;

            case LEX_MINUSEQ:
                from_stack(args, i);
                from_stack(args, j);
                k = TID[j].get_value();
                TID[j].set_value(k - i);
                TID[j].set_assign();
                break;
 
            case LEX_SLASH:
                from_stack(args, i);
                from_stack(args, j);
                if(i) {
                    args.push(j / i);
                    break;
                }
                else {
                    cout << "division by zero";
                    throw "POLIZ: division by zero";
                }
 
            case LEX_EQ:
                from_stack(args, i);
                from_stack(args, j);
                if(!args_str.empty()) {
                    from_stack(args_str, str1);
                    from_stack(args_str, str2);
                    args.push(str1 == str2);
                } else args.push (i == j);

                break;

            case LEX_LSS:
                from_stack(args, i);
                from_stack(args, j);
                args.push(j < i);
                break;
 
            case LEX_GTR:
                from_stack(args, i);
                from_stack(args, j);
                args.push(j > i);
                break;
 
            case LEX_LEQ:
                from_stack(args, i);
                from_stack(args, j);
                args.push(j <= i);
                break;
 
            case LEX_GEQ:
                from_stack(args, i);
                from_stack(args, j);
                args.push(j >= i);
                break;
 
            case LEX_NEQ:
                from_stack(args, i);
                from_stack(args, j);
                args.push(j != i);
                break;

            case LEX_STRCONST:
                i = pc_el.get_value();
                str1 = TSTR[i];
                args_str.push(str1);
                args.push(i);
                break;    
 
            case LEX_ASSIGN:
                if(!args_str.empty()) {
                    from_stack(args_str, str1);
                }
                from_stack(args, i);
                from_stack(args, j);
                TID[j].set_value(i);
                TID[j].set_assign(); 
                break;

            case LEX_SEMICOLON:
                if(!args.empty()) {
                    cout << "too much elems";
                    throw "POLIZ: Too much elems";
                }
                break;
 
            default:
                cout << "unexpected elem";
                throw "POLIZ: unexpected elem";
        }
        ++index;
    };
    cout << "Finish" << endl;
}
 
class Interpretator {
    Parser pars;
    Executer E;
public:
    Interpretator(const char* program): pars(program) {}
    void interpretation ();
};
 
void Interpretator::interpretation() {
    pars.analyze();
    //cout << "ok" << endl;
    E.execute(pars.poliz);
}

int main(int argc, char* argv[]) {
	try {
        Interpretator I("prog.txt");
        I.interpretation();
        return 0;
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
        cout << endl << "unexpected behaviour" << endl;
        return 1;
    }
	return 0;
}


/* 


*/