#include<iostream>
#include<fstream>
#include <iostream>
#include<sstream>
#include<vector>
#include<math.h>

using namespace std;

int line = 1;
bool validCode = true;

const short T_num = 1;
const short T_list = 2;

//node for a variable or a function
struct table{
    string variable;
    short type;
    bool isFunc;
    short numberOfParams;
    vector<table> list;
};

//type and val of a expression
struct exprVal{
    string tok;
    short type;
};

//for searching a variable in symbol table
int search(vector<table> symbolTable, string find)
{
    for(int i=0 ; i<symbolTable.size();i++)
        if(symbolTable[i].variable == find)
            return i;
    
    return -1;
}

bool greaterThan(char gt)
{
    return gt == '>';
}

bool lessThan(char lt)
{
    return lt == '<';
}

bool equal(char eq)
{
    return eq == '=';
}

bool comma(char c)
{
    return c == ',';
}
 bool colon(char d)
 {
     return d == ':';
 }

 bool semiColon(char s)
 {
     return s == ';';
 }

bool isDigit(char digit)
{
    return digit >= '0' && digit <= '9';
}

bool openBrace(char obrace)
{
    return obrace == '{';
}

bool closeBrace(char cbrace)
{
    return cbrace == '}';
}

bool openParenthesis(char op)
{
    return op == '(';
}

bool closeParenthesis(char cp)
{
    return cp == ')';
}

bool openBracket(char b)
{
    return b == '[';
}

bool closeBracket(char b)
{
    return b == ']';
}

bool add(char p)
{
    return p == '+';
}

bool sub(char s)
{
    return s == '-';
}

bool mul(char m)
{
    return m == '*'; 
}

bool div(char d)
{
    return d == '/';
}

bool mod(char m)
{
    return m == '%';
}

bool doubleQ(char dq)
{
    return dq == '\"';
}

bool singleQ(char sq)
{
    return sq == '\'';
}

bool And(char a)
{
    return a == '&';
}

bool Or(char o)
{
    return o == '|';
}

bool Not(char n)
{
    return n == '!';
}


bool Main(string word)          { return word == "main"; }

bool If(string word)            { return word == "if"; }

bool Else(string word)          { return word == "else"; }

bool For(string word)           { return word == "for"; }

bool While(string word)         { return word == "while"; }

bool In(string word)            { return word == "in"; }

bool Return(string word)        { return word == "return";}

bool num(string word)           { return word == "num"; }

bool list(string word)          { return word == "list"; }

bool local(string word)         { return word == "local"; }

bool numread(string word)       { return word == "numread"; }

bool numprint(string word)      { return word == "numprint"; }

bool vec(string word)           { return word == "vec"; }

bool veclen(string word)        { return word == "veclen"; } 

bool exit(string word)          { return word == "exit"; }

bool isKey(string word)
{
    return Main(word) || If(word) || Else(word) || For(word) || While(word) || In(word)
        || Return(word) || num(word) || list(word) || local(word) || numread(word)
        || numprint(word) || vec(word) || veclen(word) || exit(word);
}

bool Letter(string letter)
{
    bool valid=false;
    if(!isKey(letter))
            if((int(letter[0])>=65 && int(letter[0])<=90) || 
            (int(letter[0])>=97 && int(letter[0]<=122))
            || letter[0] == '_')
            {
                if(letter.size() == 1) valid=true;
                    for(int i=1 ; i<letter.size() ; i++)
                        if(isDigit(letter[i]) || (int(letter[i])>=65 && int(letter[i])<=90) ||
                         (int(letter[i])>=97 && int(letter[i]<=122)) || letter[i] == '_' )
                            valid=true;
                        else
                        return false;
            }

    return valid;
}

bool isIntNumber(string number)
{
    double num;
    stringstream iss(number);
    iss>>num;

    if(num>= -2147483648 && num<= 2147483647)
    {
        for(int i=0; i<number.size(); i++)
            if(number[i] != '0' && number[i] != '1' && number[i] != '2' && number[i] != '3' && number[i] != '4' && 
            number[i] != '5' && number[i] != '6' && number[i] != '7' && number[i] != '8' && number[i] != '9' && number[i] !='.')
                return false;

        return true;
    }
    
    return false;
}

bool isDoubleNumber(string number)
{
    double num;
    stringstream iss(number);
    iss>>num;

    if(num>= -1.7 * pow(10,308)  && num<= 1.7 * pow(10,308))
    {
        for(int i=0; i<number.size(); i++)
            if(number[i] != '0' && number[i] != '1' && number[i] != '2' && number[i] != '3' && number[i] != '4' && 
            number[i] != '5' && number[i] != '6' && number[i] != '7' && number[i] != '8' && number[i] != '9' && number[i] !='.')
                return false;

        return true;
    }
    
    return false;
}

string nextToken(ifstream &myfile)
{
    char c;
    string token = "";

    if(!myfile.eof())
    {
        myfile.get(c);
        short a = (short) c;
        while((c == ' ' || c == '\n' || c == '#' || c == '\r') && !myfile.eof())    
        {
            if(c == '#')
            {
                myfile.get(c);
                while((c != '\r') && !myfile.eof())    myfile.get(c);
            }
            if(c == '\n')   line++;
            myfile.get(c);
        }

        if(!myfile.eof())
        {
            if((c>='a' && c<= 'z') || (c>='A' && c<='Z') || isDigit(c))
            {
                token ="";

                while(((c>= 'a' && c<='z') || (c>='A' && c<='Z') || isDigit(c)) && !myfile.eof())
                {
                    token = token + c;
                    myfile.get(c);
                }
                
                if(!myfile.eof())   
                {
                    myfile.unget();
                }
                
            }
            else if(comma(c))   return ",";
            else if(colon(c))   return ":";
            else if(semiColon(c))    return ";";
            else if(openBrace(c))    return "{";
            else if(closeBrace(c))   return "}";
            else if(openParenthesis(c))  return "(";
            else if(closeParenthesis(c)) return ")";
            else if(openBracket(c))  return "[";
            else if(closeBracket(c)) return "]";
            else if(add(c))          return "+";
            else if(sub(c))          return "-";
            else if(mul(c))          return "*";
            else if(div(c))          return "/";
            else if(mod(c))          return "%";
            else if(And(c)){
                if(!myfile.eof())
                {
                    char c1;
                    myfile.get(c1);
                    if(And(c1))
                        return "&&";

                    myfile.unget();
                    return "&";
                }
            }
            else if(Or(c)){
                if(!myfile.eof())
                {
                    char c1;
                    myfile.get(c1);
                    if(Or(c1))
                        return "||";

                    myfile.unget();
                    return "|";
                }
            }
            else if(Not(c))          return "!";
            else if(equal(c))
            {
                if(!myfile.eof())
                {
                    char c1;
                    myfile.get(c1);
                    if(equal(c1))
                        return "==";

                    myfile.unget();
                    return "=";
                }

            }   
            else if(c == '>')
            {
                if(!myfile.eof())
                {
                    char c1;
                    myfile.get(c1);
                    if(equal(c1))   return ">=";

                    myfile.unget();
                    return ">";
                }
            }
            else if(c == '<')
            {
                if(!myfile.eof())
                {
                    char c1;
                    myfile.get(c1);
                    if(c1 == '=')   return "<=";

                    myfile.unget();
                    return "<";
                }

            }
        }
    }
    return token;
}

void prog(ifstream &file, vector<table> &symbolTable, string &token);
void func(ifstream &file, vector<table> &symbolTable, string &token);
short flist(ifstream &file, vector<table> &symbolTable, string &token);
short Type(ifstream &file, string &token);
void stmt(ifstream &file, short t, vector<table> &symbolTable, string &token);
void body(ifstream &file, short t, vector<table> &symbolTable, string &token);

exprVal expr(ifstream &file, vector<table> &symbolTable, string &token);
exprVal expr1(ifstream &file, vector<table> &symbolTable, string &token);
exprVal expr2(ifstream &file, vector<table> &symbolTable, string &token);
exprVal expr3(ifstream &file, vector<table> &symbolTable, string &token);
exprVal expr4(ifstream &file, vector<table> &symbolTable, string &token);
exprVal expr5(ifstream &file, vector<table> &symbolTable, string &token);
exprVal expr6(ifstream &file, vector<table> &symbolTable, string &token);
exprVal expr7(ifstream &file, vector<table> &symbolTable, string &token);

void defvar(ifstream &file,vector<table> &symbolTable, string &token);
void clist(ifstream &file, int funcPosition, vector<table> &symbolTable, string &token, short &count);

int main()
{
    ifstream file;
    file.open("tslang", ios::in);
    vector<table> symbolTable;

    //////////adding existing functions//////
    table numRead; //index : 0
    numRead.variable = "numread";
    numRead.type = T_num;
    numRead.isFunc = true;
    numRead.numberOfParams = 0;

    table numPrint; //index : 1
    numPrint.variable = "numprint";
    numPrint.type = 0;
    numPrint.isFunc = true;
    numPrint.numberOfParams = 1;
    
    table input;
    input.variable = "input";
    input.type = T_num;
    input.isFunc = false;
    input.numberOfParams = 0;

    numPrint.list.push_back(input);

    table Vec; //index : 2
    Vec.variable = "vec";
    Vec.type = T_list;
    Vec.isFunc = true; 
    Vec.numberOfParams = 1;

    table number;
    number.variable = "number";
    number.type = T_num;
    number.isFunc = false;
    number.numberOfParams = 0;
    
    Vec.list.push_back(number);

    table Veclen; //index : 3
    Veclen.variable = "veclen";
    Veclen.type = T_num;
    Veclen.isFunc = true;
    Veclen.numberOfParams = 1;

    table vect;
    vect.variable = "vector";
    vect.type = T_list;
    vect.isFunc = false;
    vect.numberOfParams = 0;

    Veclen.list.push_back(vect);

    table Exit; //index : 4
    Exit.variable = "exit";
    Exit.type = 0;
    Exit.isFunc = true;
    Exit.numberOfParams = 1;
    Exit.list.push_back(input);

    symbolTable.push_back(numRead);
    symbolTable.push_back(numPrint);
    symbolTable.push_back(Vec);
    symbolTable.push_back(Veclen);
    symbolTable.push_back(Exit);
    /////////////////////////////////////////

    string token = nextToken(file);
    prog(file, symbolTable, token);
    file.close();

    return 0;
}

void prog(ifstream &file, vector<table> &symbolTable, string &token){
    func(file,symbolTable, token);
    token = nextToken(file);
    if(Letter(token) || Main(token))
    {
        prog(file, symbolTable, token);
    }
    else if(isKey(token))
    {
        cout<<line<<": key words can't be redefined"<<endl;
    }
    
}

void func(ifstream &file, vector<table> &symbolTable, string &token)
{
    short type;
    if(Letter(token) || Main(token))
    {
        int index = search(symbolTable, token);
        if(index == -1)
        {
            table s;
            s.variable = token;
            s.isFunc = true;
            token = nextToken(file);

            if(openParenthesis(token[0]))
            {
                token = nextToken(file);
                s.numberOfParams = flist(file, s.list,token);
                if(closeParenthesis(token[0]))
                {
                    token = nextToken(file);
                    if(colon(token[0]))
                    {
                        token = nextToken(file);
                        type = Type(file,token);
                        s.type = type;
                        symbolTable.push_back(s);
                        int size = symbolTable.size();

                        for(int c = 0 ; c<s.list.size() ; c++)
                            symbolTable.push_back(s.list[c]);

                        token = nextToken(file);
                        if(openBrace(token[0]))
                        {
                            token = nextToken(file);
                            body(file,type, symbolTable,token);
                            if(!closeBrace(token[0]))    
                            {
                                cout<<line<<": } is missing!"<<endl;
                            }

                            while(size != symbolTable.size())
                                symbolTable.erase(symbolTable.end());
                        }
                        else
                        {
                            cout<<line<<": body should be declared in {}"<<endl;
                        }
                    }
                    else
                    {
                        cout<<"function need a decleration"<<endl;
                    }

                }
                else 
                {
                    cout<<line<<": ) is missing!"<<endl;
                }
            }
            else
            {
                cout<<line<<": ( is missing!"<<endl;
            }
        }
        else
        {
            cout<<line<<": function already declared"<<endl;
        }
    }
    else if(isKey(token))
    {
        cout<<line<<": key words can't be redefined"<<endl;
    }
    else {
        cout<<line<<": "<<token<<" is ambigous"<<endl;
    }
    
}


short flist(ifstream &file, vector<table> &symbolTable, string &token)
{
    short type;
    if(closeParenthesis(token[0]))
    {
        return symbolTable.size();
    }
    else if(Letter(token))
    {
        int index = search(symbolTable, token);
        if(index == -1)
        {
            table s;
            s.variable = token;
            token = nextToken(file);
            if(colon(token[0]))
            {
                token = nextToken(file);
                type = Type(file,token);
                s.type = type;
                s.isFunc = false;
                s.numberOfParams = 0;
                symbolTable.push_back(s);
                token = nextToken(file);
                if(comma(token[0]))
                {
                    token = nextToken(file);
                    return flist(file,symbolTable,token);
                }
                else
                    return symbolTable.size();
            }
            else {
                cout<<line<<": variable must be declared"<<endl;
                return symbolTable.size();

            }
        }
        else {
            cout<<line<<": variable already declared"<<endl;
            return -1;
        }
    }
}

void clist(ifstream &file,int funcPosition, vector<table> &symbolTable, string &token, short &count)
{
    if(closeParenthesis(token[0]))
    {
        return;
    }
    else 
    {
        short type = expr(file,symbolTable, token).type;
        if(funcPosition != -1)
        {
            if(count<symbolTable[funcPosition].numberOfParams)
                if(symbolTable[funcPosition].list[count].type != type)
                    cout<<line<<": illegal parameter type!"<<endl;
        }

        count++;
        if(comma(token[0]))
        {
            token = nextToken(file);
            clist(file, funcPosition, symbolTable,token, count);
        }
    }
}

short Type(ifstream &file, string &token)
{
    if(num(token))
    {
        return T_num;
    }
    else if(list(token))
    {
        return T_list;
    }
}

void body(ifstream &file, short t, vector<table> &symbolTable, string &token)
{
    stmt(file,t,symbolTable,token);
    if(isKey(token) || Letter(token) || Not(token[0]) || add(token[0]) || sub(token[0])
    || openBrace(token[0]) || openParenthesis(token[0]) ||
    isIntNumber(token) || isDoubleNumber(token) && !Main(token))
        body(file,t,symbolTable,token);
}

void stmt(ifstream &file, short t, vector<table> &symbolTable, string &token)
{
    if(local(token))
    {
        defvar(file, symbolTable, token);
        if(!semiColon(token[0]))
        {
            cout<<line<<": expected a ;"<<endl;
        }

        token = nextToken(file);
    }
    else if(If(token))
    {
        token = nextToken(file);
        if(openParenthesis(token[0]))
        {
            token = nextToken(file);
            short type = expr(file,symbolTable,token).type;
            if(type == T_list)
                cout<<line<<": illegal condition type!"<<endl;

            if(closeParenthesis(token[0]))
            {
                int size = symbolTable.size();
                token = nextToken(file);
                stmt(file,t,symbolTable,token);

                while(symbolTable.size() != size)
                    symbolTable.erase(symbolTable.end());

                if(Else(token))
                {
                    int size = symbolTable.size();
                    token = nextToken(file);
                    stmt(file,t,symbolTable,token);

                    while(symbolTable.size() != size)
                        symbolTable.erase(symbolTable.end());
                }       
            }
            else
            {
                cout<<line<<": expected a )"<<endl;
            }
            
        }
        else
        {
            cout<<line<<": expected a ("<<endl;
        }
        
    }
    else if(While(token))
    {
        token = nextToken(file);
        if(openParenthesis(token[0]))
        {
            token = nextToken(file);
            short type = expr(file,symbolTable,token).type;
            if(type == T_list)
                cout<<line<<": illegal condition type!"<<endl;

            if(closeParenthesis(token[0]))
            {
                int size = symbolTable.size();
                token = nextToken(file);
                stmt(file,t,symbolTable,token);

                while(symbolTable.size() != size)
                    symbolTable.erase(symbolTable.end());
            }
            else
            {
                cout<<line<<": exected a )"<<endl;
            }
            
        }
        else
        {
            cout<<line<<": expected a ("<<endl;
        }
        
    }
    else if(For(token))
    {
        token = nextToken(file);
        if(openParenthesis(token[0]))
        {
            token = nextToken(file);
            if(Letter(token))
            {  
                int size = symbolTable.size();  
                table temp;
                temp.variable = token;
                temp.type = T_num;
                temp.isFunc = false;
                temp.numberOfParams = 0;
                symbolTable.push_back(temp);
                token = nextToken(file);
                if(In(token))
                {
                    token = nextToken(file);
                    short type = expr(file,symbolTable,token).type;

                    if(type == T_num)
                        cout<<line<<": illegal type!"<<endl;

                    if(closeParenthesis(token[0]))
                    {
                        token = nextToken(file);
                        stmt(file,t,symbolTable,token);
                        while(symbolTable.size() != size)
                        {
                            symbolTable.erase(symbolTable.end());
                        }
                    }
                    else {
                        cout<<line<<": a ) is missing"<<endl;
                    }
                }
                else {
                    cout<<line<<"word 'in' is missing"<<endl;
                }
            }
            else {
                cout<<line<<": invalid variable in loop"<<endl;
            }
        }
        else {
            cout<<line<<": expected a ("<<endl;
        }
    }
    else if(Return(token))
    {
        token = nextToken(file);
        short type = expr(file,symbolTable, token).type;
        if(type != t)
        {
            cout<<line<<": illegal return type!"<<endl;
        }
        
        if(!semiColon(token[0]))
        {
            cout<<line<<": expected a ;"<<endl;
        }
        token = nextToken(file);
    }
    else if(openBrace(token[0]))
    {
        token = nextToken(file);
        body(file,t,symbolTable,token);
        if(!closeBrace(token[0]))
        {
            cout<<line<<": expected a }"<<endl;
        }
        token = nextToken(file);
    }
    else if(!closeBrace(token[0]))
    {
        expr(file,symbolTable,token);
        if(!semiColon(token[0]))
        {
            cout<<line<<": expected a ;"<<endl;
        }

        token = nextToken(file);
    }
    
}

exprVal expr(ifstream &file, vector<table> &symbolTable, string &token)
{
    exprVal t;
    t = expr1(file,symbolTable,token);
    bool error = true;
    while(equal(token[0]))
    {
        token = nextToken(file);
        short t1 = expr1(file,symbolTable,token).type;

        if(t.type == 0 || t1 == 0 && error)
        {
            error = false;
            cout<<line<<": variable not declared!"<<endl;
        }
        else if((t.type == T_list && t1 == T_num) || (t.type == T_num && t1 == T_list)
        || ((isDoubleNumber(t.tok) || isIntNumber(t.tok)) &&
                (isDoubleNumber(token) || isIntNumber(token))) && error)
            {
                t.type = 0;
                cout<<line<<": illegal assignment!"<<endl;
            }
    }

    return t;
    
}

exprVal expr1(ifstream &file, vector<table> &symbolTable, string &token)
{
    exprVal t;
    t = expr2(file,symbolTable,token);
    bool error = true;
    while(sub(token[0]) || add(token[0]))
    {
        token = nextToken(file);
        short t1 = expr2(file, symbolTable,token).type;

        if(t.type == 0 || t1 == 0 && error)
        {
            error = false;
            cout<<line<<": variable not declared!"<<endl;
        }
        else if(t.type == T_list || t1 == T_list && error)
        {
            cout<<line<<": incompatible operands!"<<endl;
        }
    }

    return t;
}

exprVal expr2(ifstream &file, vector<table> &symbolTable, string &token)
{
    exprVal t;
    t = expr3(file,symbolTable,token);
    bool error = true;
    while(mul(token[0]) || div(token[0]) || mod(token[0]))
    {
        token = nextToken(file);
        short t1 = expr2(file,symbolTable,token).type;

        if(t.type == 0 || t1 == 0 && error)
        {
            error = false;
            cout<<line<<": variable not declared!"<<endl;
        }
        else if((t.type == T_list && t1 == T_num) || (t.type == T_num && t.type == T_list) && error)
        {
            cout<<line<<": incompatible operands!"<<endl;
        }
    }

    return t;
}

exprVal expr3(ifstream &file, vector<table> &symbolTable, string &token)
{
    exprVal t;
    t = expr4(file,symbolTable,token);
    bool error = false;
    while(greaterThan(token[0]) || lessThan(token[0]) ||
    (equal(token[0]) && equal(token[1])) || (greaterThan(token[0]) && equal(token[1])) ||
    (lessThan(token[0]) && equal(token[1])))
    {
        token = nextToken(file);
        short t1 = expr4(file,symbolTable,token).type;
        
        if(t.type == 0 || t1 == 0 && error)
        {
            error = false;
            cout<<line<<": variable not declared!"<<endl;
        }
        else if((t.type == T_list && t1 == T_num) || (t.type == T_num && t.type == T_list) && error)
        {
            cout<<line<<": incompatible comparation!"<<endl;
        }
    }

    return t; 
}

exprVal expr4(ifstream &file, vector<table> &symbolTable, string &token)
{
    exprVal t;
    t = expr5(file,symbolTable,token);
    bool error = false;
    while((Or(token[0]) && Or(token[1])) || (And(token[0]) && And(token[1])))
    {
        token = nextToken(file);
        short t1 = expr5(file,symbolTable,token).type;
        
        if(t.type == 0 || t1 == 0 && error)
        {
            error = false;
            cout<<line<<": variable not declared!"<<endl;
        }
        else if((t.type == T_list && t1 == T_num) || (t.type == T_num && t.type == T_list) && error)
        {
            cout<<line<<": incompatible logical operands!"<<endl;
        }
    }

    return t;
}

exprVal expr5(ifstream &file, vector<table> &symbolTable, string &token)
{
    exprVal t;
    t = expr6(file,symbolTable,token);
    bool error = true;
    bool isOBracket = false;
    while(openBracket(token[0]))
    {
        isOBracket = true;
        token = nextToken(file);
        short t1 = expr7(file,symbolTable,token).type;
        if(t.type == 0 || t1 == 0 && error)
        {
            error = false;
            cout<<line<<": variable not declared!"<<endl;
        }
        else if(t.type == T_num && error)
        {
            if(isIntNumber(t.tok) || isDoubleNumber(t.tok))
                cout<<line<<": number is ambigous"<<endl;

            cout<<line<<": must be list"<<endl;
        }
        else if(t1 == T_list && error)
            cout<<line<<": illegal type!"<<endl;
        
        if(!closeBracket(token[0]))
        {
            cout<<line<<": expected a ]"<<endl;
            break;
        }
        token = nextToken(file);
    }

    if(error && isOBracket)
        t.type = T_num;

    return t;
}

exprVal expr6(ifstream &file, vector<table> &symbolTable, string &token)
{
    exprVal ret;
    if(Not(token[0]))
    {
        token = nextToken(file);
        ret = expr(file,symbolTable,token);
    }
    else if(sub(token[0]))
    {
        token = nextToken(file);
        ret = expr(file,symbolTable,token);
    }   
    else if(add(token[0]))
    {
        token = nextToken(file);
        ret = expr(file,symbolTable,token);
    }
    else if( openParenthesis(token[0]))
    {
        token = nextToken(file);
        ret = expr(file,symbolTable,token);
        if(!closeParenthesis(token[0]))
            cout<<line<<": expected a )"<<endl;
        
        token = nextToken(file);
        
    }
    else if(Letter(token))
    {
        int s = search(symbolTable,token);
        if(s == -1)
        {
            ret.type = 0;
            cout<<line<<": variable not declared!"<<endl;
        }
        else{
            ret.type = symbolTable[s].type;
            ret.tok = token;
            token = nextToken(file);
            if(openParenthesis(token[0]))
            {
                if(symbolTable[s].isFunc)
                {
                    token = nextToken(file);
                    short countNoOfParams = 0;
                    clist(file,s,symbolTable,token,countNoOfParams);
                    if(closeParenthesis(token[0]))
                    {
                        if(countNoOfParams != symbolTable[s].numberOfParams)
                            cout<<line<<": to few arguments for a function call"<<endl;
                        token = nextToken(file);
                    }
                    else
                        cout<<line<<": expected a )"<<endl;
                }
                else
                    cout<<line<<": not a function!"<<endl;
            }
            else if(symbolTable[s].isFunc)
                cout<<line<<": parameter expected"<<endl;
        }
    }
    else if(isIntNumber(token) || isDoubleNumber(token))
    {
        ret.type = T_num;
        ret.tok = token;
        token = nextToken(file);
    }
    else if(numread(token))
    {
        ret.type = T_num;
        ret.tok = token;
        token = nextToken(file);
        if(openParenthesis(token[0]))
        {
            token = nextToken(file);
            short c = 0;
            clist(file,-1,symbolTable,token,c);
            if(closeParenthesis(token[0]))
            {
                if(c>0)
                    cout<<line<<"illegal parameter!"<<endl;

                token = nextToken(file);
            }
            else
                cout<<line<<"expected a )"<<endl;
        }
        else
            cout<<line<<": expected a ("<<endl;
    }
    else if(numprint(token))
    {
        ret.type = 0;
        ret.tok = token;
        token = nextToken(file);
        if(openParenthesis(token[0]))
        {
            token = nextToken(file);
            short c = 0;
            clist(file,1,symbolTable,token,c);
            if(closeParenthesis(token[0]))
            {
                if(c != 1)
                    cout<<line<<"illegal parameter!"<<endl;
                token = nextToken(file);
            }
            else
                cout<<line<<"expected a )"<<endl;
        }
        else
            cout<<line<<": expected a ("<<endl;
    }
    else if(vec(token))
    {
        ret.type = T_list;
        ret.tok = token;
        token = nextToken(file);
        if(openParenthesis(token[0]))
        {
            token = nextToken(file);
            short c = 0;
            clist(file,2,symbolTable,token,c);
            if(closeParenthesis(token[0]))
            {
                if(c != 1)
                    cout<<line<<"illegal parameter!"<<endl;
                token = nextToken(file);
            }
            else
                cout<<line<<"expected a )"<<endl;
        }
        else
            cout<<line<<": expected a ("<<endl;
    }
    else if(veclen(token))
    {
        ret.type = T_num;
        ret.tok = token;
        token = nextToken(file);
        if(openParenthesis(token[0]))
        {
            token = nextToken(file);
            short c = 0;
            clist(file,3,symbolTable,token,c);
            if(closeParenthesis(token[0]))
            {
                if(c != 1)
                    cout<<line<<"illegal parameter!"<<endl;
                token = nextToken(file);
            }
            else
                cout<<line<<"expected a )"<<endl;
        }
        else
            cout<<line<<": expected a ("<<endl;
    }
    else if(exit(token))
    {
        ret.type = 0;
        ret.tok = token;
        token = nextToken(file);
        if(openParenthesis(token[0]))
        {
            token = nextToken(file);
            short c = 0;
            clist(file,4,symbolTable,token,c);
            if(closeParenthesis(token[0]))
            {
                if(c != 1)
                    cout<<line<<"illegal parameter!"<<endl;
                token = nextToken(file);
            }
            else
                cout<<line<<"expected a )"<<endl;
        }
        else
            cout<<line<<": expected a ("<<endl;
    }

    return ret;

}

exprVal expr7(ifstream &file, vector<table> &symbolTable, string &token)
{
    exprVal ret;
    if(Not(token[0]))
    {
        token = nextToken(file);
        ret = expr(file,symbolTable,token);
    }
    else if(sub(token[0]))
    {
        token = nextToken(file);
        ret = expr(file,symbolTable,token);
    }   
    else if(add(token[0]))
    {
        token = nextToken(file);
        ret = expr(file,symbolTable,token);
    }
    else if( openParenthesis(token[0]))
    {
        token = nextToken(file);
        ret = expr(file,symbolTable,token);
        if(!closeParenthesis(token[0]))
            cout<<line<<": expected a )"<<endl;
        
        token = nextToken(file);
        
    }
    else if(Letter(token))
    {
        int s = search(symbolTable,token);
        if(s == -1)
        {
            ret.type = 0;
            cout<<line<<": variable not declared!"<<endl;
        }
        else{
            ret.type = symbolTable[s].type;
            ret.tok = token;
            token = nextToken(file);
            if(openParenthesis(token[0]))
            {
                if(symbolTable[s].isFunc)
                {
                    token = nextToken(file);
                    short countNoOfParams = 0;
                    clist(file,s,symbolTable,token,countNoOfParams);
                    if(closeParenthesis(token[0]))
                    {
                        if(countNoOfParams != symbolTable[s].numberOfParams)
                            cout<<line<<": to few arguments for a function call"<<endl;
                        token = nextToken(file);
                    }
                    else
                        cout<<line<<": expected a )"<<endl;
                }
                else
                    cout<<line<<": not a function!"<<endl;
            }
            else if(symbolTable[s].isFunc)
                cout<<line<<": parameter expected"<<endl;
        }
    }
    else if(isIntNumber(token) || isDoubleNumber(token))
    {
        ret.type = T_num;
        ret.tok = token;
        token = nextToken(file);
    }
    else if(numread(token))
    {
        ret.type = T_num;
        ret.tok = token;
        token = nextToken(file);
        if(openParenthesis(token[0]))
        {
            token = nextToken(file);
            short c = 0;
            clist(file,-1,symbolTable,token,c);
            if(closeParenthesis(token[0]))
            {
                if(c>0)
                    cout<<line<<"illegal parameter!"<<endl;

                token = nextToken(file);
            }
            else
                cout<<line<<"expected a )"<<endl;
        }
        else
            cout<<line<<": expected a ("<<endl;
    }
    else if(numprint(token))
    {
        ret.type = 0;
        ret.tok = token;
        token = nextToken(file);
        if(openParenthesis(token[0]))
        {
            token = nextToken(file);
            short c = 0;
            clist(file,1,symbolTable,token,c);
            if(closeParenthesis(token[0]))
            {
                if(c != 1)
                    cout<<line<<"illegal parameter!"<<endl;
                token = nextToken(file);
            }
            else
                cout<<line<<"expected a )"<<endl;
        }
        else
            cout<<line<<": expected a ("<<endl;
    }
    else if(vec(token))
    {
        ret.type = T_list;
        ret.tok = token;
        token = nextToken(file);
        if(openParenthesis(token[0]))
        {
            token = nextToken(file);
            short c = 0;
            clist(file,2,symbolTable,token,c);
            if(closeParenthesis(token[0]))
            {
                if(c != 1)
                    cout<<line<<"illegal parameter!"<<endl;
                token = nextToken(file);
            }
            else
                cout<<line<<"expected a )"<<endl;
        }
        else
            cout<<line<<": expected a ("<<endl;
    }
    else if(veclen(token))
    {
        ret.type = T_num;
        ret.tok = token;
        token = nextToken(file);
        if(openParenthesis(token[0]))
        {
            token = nextToken(file);
            short c = 0;
            clist(file,3,symbolTable,token,c);
            if(closeParenthesis(token[0]))
            {
                if(c != 1)
                    cout<<line<<"illegal parameter!"<<endl;
                token = nextToken(file);
            }
            else
                cout<<line<<"expected a )"<<endl;
        }
        else
            cout<<line<<": expected a ("<<endl;
    }
    else if(exit(token))
    {
        ret.type = 0;
        ret.tok = token;
        token = nextToken(file);
        if(openParenthesis(token[0]))
        {
            token = nextToken(file);
            short c = 0;
            clist(file,4,symbolTable,token,c);
            if(closeParenthesis(token[0]))
            {
                if(c != 1)
                    cout<<line<<"illegal parameter!"<<endl;
                token = nextToken(file);
            }
            else
                cout<<line<<"expected a )"<<endl;
        }
        else
            cout<<line<<": expected a ("<<endl;
    }

    return ret;

}

void defvar(ifstream &file, vector<table> &symbolTable, string &token)
{
    if(local(token))
    {
        token = nextToken(file);
        flist(file,symbolTable,token);
    }
}