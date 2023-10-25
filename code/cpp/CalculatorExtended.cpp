#include<iostream>
#include<vector>
#include<stdexcept>
#include<cmath>
#include<cctype>
using namespace std;
//===============================================================================================
//constants
const char let = '#'; //define variable command
const char cnst = '$'; //define constant command
const char quit = 'Q';
const char print = ';';
const char print_end = '\n';
const char help = 'h';
const char number = '8';
const char name = 'a';
const char sqroot = 'r';
const char power = 'p';
const string exitkey = "exit";
const string declkey = "let";  //define variable keyword
const string cnstkey = "const"; //define constant keyword
const string sqrtkey = "sqrt";
const string powkey = "pow";
const string helpkey = "help";
//===============================================================================================
//tools
void error(string str){throw runtime_error(str);}
bool is_int(double d){
    double f = d - int(d);
    if (f == 0) return true;
    return false;
}
//===============================================================================================
//Token System.
struct Token {
	char kind;
	double value;
	string name;
	//constructors
	Token(char ch) :kind(ch), value(0) { }
	Token(char ch, double val) :kind(ch), value(val) { }
	Token(char ch, string str) :kind(ch), name(str){ }
};
class Token_stream {
private:
	bool full; //is there any Token in buffer
	Token buffer; //used to unget a Token
public:
	Token_stream() :full(false), buffer(0) { }
	Token get();
	void unget(Token t) { buffer = t; full = true; }
	void ignore(char);//ignore tokens until char c
};

Token Token_stream::get()
{
	if (full) { full = false; return buffer;}
	char ch;
	cin.get(ch);//to enable reading spaces and new line (unlike: cin>>ch;)
	switch (ch) {
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
    case '=':
    case ',':
	case print:
    case print_end:
	case quit:
    case let:
    case cnst:
		return Token(ch);
	case '.':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	{
        cin.unget();
        double val;
        cin >> val;
        return Token(number, val);
	}
	default:
		if (isalpha(ch)|| ch == '_') {
			string s;
			s += ch;
			//read name that contains alpha, digits or underscore only
			while (cin.get(ch) && (isalpha(ch) ||ch == '_'|| isdigit(ch))) s += ch;
			cin.unget();
			if (s == declkey) return Token(let);
			if (s == cnstkey) return Token(cnst);
			if (s == sqrtkey) return Token(sqroot);
			if (s == powkey) return Token(power);
			if (s == exitkey) return Token(quit);
			if (s == helpkey) return Token(help);
			return Token(name, s); //variable name
		}
		if (isspace(ch)){return get();}//ignore spaces
		error("Bad token");
	}
}

void Token_stream::ignore(char c)
{//remove all chars from buffer and cin until newline or char c;
	if (full && c == buffer.kind) {
		full = false;
		return;
	}
	full = false;
	char ch = 0;
	while(ch != '\n' && ch != c){
        cin.get(ch);
	}
	cin.unget();
}
//===============================================================================================
//Variable System
struct Variable {
	string name;
	double value;
	bool editable;//variable or constant
	Variable(string n, double v) :name(n), value(v), editable(true) { }
	Variable(string n, double v, bool edit) :name(n), value(v), editable(edit) { }
};

class Symbol_table{
    vector<Variable> names;
public:
    double get_value(string s)
    {//return symbol s value
        for (int i = 0; i < names.size(); ++i)
            if (names[i].name == s) return names[i].value;
        error("get: undefined name "+s);
    }

    void set_value(string s, double d)
    {//set value d to variable symbol s
        for (int i = 0; i < names.size(); ++i)
            if (names[i].name == s) {
                if (names[i].editable == false) error("can't edit the constant "+ s);
                names[i].value = d;
                return;
            }
        error("set: undefined name "+ s);
    }

    void declare(string s, double v, bool edit = true){
        //add new symbol s to the table
        if (is_declared(s)) error(s+" already declared");
        names.push_back(Variable(s, v, edit));
    }
    bool is_declared(string s)
    { //return true if symbol s exist in the table
        for (int i = 0; i < names.size(); ++i)
            if (names[i].name == s) return true;
        return false;
    }
};
//===============================================================================================
//global variables
Symbol_table sym; //define a symbol table for variables and constants
Token_stream ts; //define a token stream for handling tokens
//===============================================================================================
//grammar functions
double expression();

double primary()
{
	Token t = ts.get();
	switch (t.kind) {
	case '('://(expression)
	{
	    double d = expression();
        t = ts.get();
        if (t.kind != ')') error("')' expected");
        return d;
	}
	case '-': //-primary (negative numbers)
		return -primary();
    case '+'://+primary (positive numbers)
		return primary();
	case number:
		return t.value;
	case name:{//a variable name: assign or get value
	    string var = t.name;
	    t = ts.get();
	    if (t.kind == '='){//if followed by '=' then assign
            double d = expression();
            sym.set_value(var, d);
            return d;
	    }else{
	        ts.unget(t);
            return sym.get_value(var);
	    }
	}
    case sqroot:{//sqrt(expression)
        t = ts.get();
        if (t.kind != '(') error("'(' expected");
        double d = expression();
        t = ts.get();
        if (t.kind != ')') error("')' expected");
        if (d < 0) error("sqrt: expression must be positive");
        return sqrt(d);
    }
    case power:{//pow(expression)
        t = ts.get();
        if (t.kind != '(') error("'(' expected");
        double x = expression();
        t = ts.get();
        if (t.kind != ',') error("',' expected");
        double i = expression();
        t = ts.get();
        if (t.kind != ')') error("')' expected");
        if ( is_int(i)== false) error("i not an integer");
        return pow(x, i);
    }
	default:
		error("primary expected");
	}
}

double term()
{
	double left = primary();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		case '*':
			left *= primary();
			break;
		case '/':
		{
		    double d = primary();
            if (d == 0) error("divide by zero");
            left /= d;
            break;
		}
		case '%':
		{
		    double d = primary();
            if (d == 0) error("divide by zero");
            left = fmod(left, d);
            break;
		}
		default:
			ts.unget(t);
			return left;
		}
	}
}

double expression()
{
	double left = term();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		case '+':
			left += term();
			break;
		case '-':
			left -= term();
			break;
        case '='://no assignment to expressions , ex: 5 = 7; error
            error("can't assign (=) expression to expression");
            break;
		default:
			ts.unget(t);
			return left;
		}
	}
}

double declaration(bool edit = true)
{//declare a symbol: name = expression
	Token t = ts.get();
	if (t.kind != name) error("name expected in declaration");
	string name = t.name;
	t = ts.get();
	if (t.kind != '=') error("= missing in declaration of "+name);
	double d = expression();
	sym.declare(name, d, edit);
	return d;
}

double statement()
{
	Token t = ts.get();
	switch (t.kind) {
	case let://declare variable
		return declaration(true);
    case cnst://declare constant
        return declaration(false);
	default://calculate expression
		ts.unget(t);
		return expression();
	}
}
//===============================================================================================
//Running the system
const string prompt = "> ";
const string result = "= ";

void calculate()
{
    double value = 0;
    cout << prompt;
	while (true) try {
	    Token t = ts.get();
        switch(t.kind){
        case print:case print_end:
            while(t.kind == print) t = ts.get();//ignore other print and print_end commands
            cout<<result<<value<<endl;
            if(t.kind == print_end) {cout << prompt;}else{ts.unget(t);}
            break;
        case quit:
            return ;
        case help:
            cout<<"HELP LIST\n";
            break;
        default:
            ts.unget(t);
            value = statement();
        }
	}
	catch (runtime_error& e) {
		cerr << e.what() << endl;
		ts.ignore(print); //clean the stream until the print symbol
	}
}

void predefined_constants(){
    sym.declare("pi", 3.141592654, false);
    sym.declare("e", 2.718281828, false);
    sym.declare("k", 1000, false);
}

int main(){
    try {
        predefined_constants();
        calculate();
        return 0;
    }
    catch (exception& e) {
        cerr << "exception: " << e.what() << endl;
        return 1;
    }
    catch (...) {
        cerr << "exception\n";
        return 2;
    }
}
