#include<iostream>
#include<fstream>
#include <iostream>
#include<sstream>
#include<vector>
#include<math.h>

using namespace std;

int line = 1;//line generator
int label = 1; // label generator
int reg = 0;//register generator

const short T_num = 1;
const short T_list = 2;

//node for a variable or a function
struct table{
    string variable;
    vector<double> value;
    int len = value.size()+1;
    short type;
    bool isFunc;
    short numberOfParams;
    int reg=-1;
    vector<table> list;
};

//type and val of a expression
struct exprVal{
    string tok;
    short type;
    vector<double> val;
    int len = val.size()+1;
    int lenReg;
    int reg = -1;
    int pos=0;
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

void prog(ifstream &file, ofstream &irCode, vector<table> &symbolTable, string &token);
void func(ifstream &file, ofstream &irCode, vector<table> &symbolTable, string &token);
short flist(ifstream &file, vector<table> &symbolTable, string &token);
short Type(ifstream &file, string &token);
void stmt(ifstream &file, ofstream &irCode, int position, vector<table> &symbolTable, string &token);
void body(ifstream &file, ofstream &irCode, int position, vector<table> &symbolTable, string &token);

exprVal expr(ifstream &file, ofstream &irCode, vector<table> &symbolTable, string &token);
exprVal expr1(ifstream &file, ofstream &irCode, vector<table> &symbolTable, string &token);
exprVal expr2(ifstream &file, ofstream &irCode, vector<table> &symbolTable, string &token);
exprVal expr3(ifstream &file, ofstream &irCode, vector<table> &symbolTable, string &token);
exprVal expr4(ifstream &file, ofstream &irCode, vector<table> &symbolTable, string &token);
exprVal expr5(ifstream &file, ofstream &irCode, vector<table> &symbolTable, string &token);
exprVal expr6(ifstream &file, ofstream &irCode, vector<table> &symbolTable, string &token);
exprVal expr7(ifstream &file, ofstream &irCode, vector<table> &symbolTable, string &token);

void defvar(ifstream &file, vector<table> &symbolTable, string &token);
void clist(ifstream &file, ofstream &irCode, int funcPosition, vector<table> &symbolTable, string &token, vector<exprVal> &params);

int main()
{
    ifstream file;
    file.open("tslang.txt", ios::in);
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

    ofstream irCode;
    irCode.open("ir.txt",ios::out);

    string token = nextToken(file);
    prog(file, irCode,symbolTable, token);
    file.close();

    return 0;
}

void prog(ifstream &file, ofstream &irCode, vector<table> &symbolTable, string &token){
    func(file,irCode,symbolTable, token);
    token = nextToken(file);
    if(Letter(token) || Main(token))
    {
        prog(file, irCode,symbolTable, token);
    }
    else if(isKey(token))
    {
        cout<<line<<": key words can't be redefined"<<endl;
    }
    
}

void func(ifstream &file, ofstream &irCode, vector<table> &symbolTable, string &token)
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

            reg = 0;
            irCode<<"proc "<<token<<endl;

            token = nextToken(file);
            
            if(openParenthesis(token[0]))
            {
                token = nextToken(file);
                s.numberOfParams = flist(file,s.list,token);
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
                            body(file,irCode,size-1, symbolTable,token);
                            if(!closeBrace(token[0]))    
                            {
                                cout<<line<<": } is missing!"<<endl;
                            }
                            else
                                irCode<<"ret"<<endl;

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
                s.reg = reg;
                
                double r;
                s.value.push_back(r);
                  
                reg++;
                
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

void clist(ifstream &file, ofstream &irCode, int funcPosition, vector<table> &symbolTable, string &token, vector<exprVal> &params)
{
    if(closeParenthesis(token[0]))
    {
        return;
    }
    else 
    {
        exprVal t1 = expr(file,irCode,symbolTable, token);
        
        if(funcPosition != -1)
        {
            if(params.size()<symbolTable[funcPosition].numberOfParams)
                if(symbolTable[funcPosition].list[params.size()].type != t1.type)
                    cout<<line<<": illegal parameter type!"<<endl;
        }

        params.push_back(t1);

        if(comma(token[0]))
        {
            token = nextToken(file);
            clist(file, irCode,funcPosition, symbolTable,token, params);
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

void body(ifstream &file, ofstream &irCode, int position, vector<table> &symbolTable, string &token)
{
    stmt(file,irCode,position,symbolTable,token);
    if(isKey(token) || Letter(token) || Not(token[0]) || add(token[0]) || sub(token[0])
    || openBrace(token[0]) || openParenthesis(token[0]) ||
    isIntNumber(token) || isDoubleNumber(token) && !Main(token))
        body(file,irCode,position,symbolTable,token);
}

void stmt(ifstream &file, ofstream &irCode, int position, vector<table> &symbolTable, string &token)
{
    if(local(token))
    {
        defvar(file,symbolTable, token);
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
            exprVal e = expr(file,irCode,symbolTable,token);

            irCode<<"jz r"<<e.reg<<",label"<<label<<endl;

            int tempLabel = label;
            label++;
            if(e.type == T_list)
                cout<<line<<": illegal condition type!"<<endl;

            if(closeParenthesis(token[0]))
            {
                // int size = symbolTable.size();
                
                token = nextToken(file);

                stmt(file,irCode,position,symbolTable,token);

                // while(symbolTable.size() != size)
                //     symbolTable.erase(symbolTable.end());

                if(Else(token))
                {
                    int size = symbolTable.size();
                    token = nextToken(file);
                    irCode<<"jmp label"<<label<<endl;
                    irCode<<"label"<<tempLabel<<":"<<endl;
                    stmt(file,irCode,position,symbolTable,token);

                    while(symbolTable.size() != size)
                        symbolTable.erase(symbolTable.end());

                    irCode<<"label"<<label<<":"<<endl;
                }
                else
                    irCode<<"label"<<tempLabel<<":"<<endl;

                label++;
                reg++;       
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
            exprVal e = expr(file,irCode,symbolTable,token);

            irCode<<"label"<<label<<":"<<endl;
            irCode<<"jz r"<<e.reg<<",label"<<label+1<<endl;
            int tempLabel = label;

            if(e.type == T_list)
                cout<<line<<": illegal condition type!"<<endl;

            if(closeParenthesis(token[0]))
            {
                int size = symbolTable.size();
                token = nextToken(file);
                stmt(file,irCode,position,symbolTable,token);

                while(symbolTable.size() != size)
                    symbolTable.erase(symbolTable.end());

                irCode<<"jmp label"<<tempLabel<<endl;
                irCode<<"label"<<tempLabel+1<<":"<<endl;
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
                int look = search(symbolTable,token);
                if(look != -1)
                {
                    int size = symbolTable.size();
                    irCode<<"cmp< r"<<reg<<",";
                    int tempReg = reg;
                    reg++;
                    token = nextToken(file);
                    if(In(token))
                    {
                        token = nextToken(file);
                        exprVal type = expr(file,irCode,symbolTable,token);

                        if(type.type == T_num)
                            cout<<line<<": illegal type!"<<endl;
                        else
                            irCode<<"r"<<symbolTable[look].reg<<",r"<<type.reg<<endl;


                        irCode<<"label"<<label<<":"<<endl;
                        irCode<<"jz r"<<tempReg<<",label"<<label+1<<endl;
                        int tempLabel = label;

                        if(closeParenthesis(token[0]))
                        {
                            token = nextToken(file);
                            stmt(file,irCode,position,symbolTable,token);
                            while(symbolTable.size() != size)
                            {
                                symbolTable.erase(symbolTable.end());
                            }

                            irCode<<"mov r"<<reg<<",1"<<endl;
                            irCode<<"add r"<<tempReg<<",r"<<tempReg<<",r"<<reg<<endl;
                            irCode<<"jmp label"<<tempLabel<<endl;
                            irCode<<"label"<<tempLabel+1<<":"<<endl;
                            reg++;
                        }
                        else {
                            cout<<line<<": a ) is missing"<<endl;
                        }
                    }
                    else {
                        cout<<line<<"word 'in' is missing"<<endl;
                    }
            }
            else
                cout<<"variable not defined!"<<endl;
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
        exprVal e = expr(file,irCode,symbolTable, token);
        if(e.type != symbolTable[position].type) //function's type
        {
            cout<<line<<": illegal return type!"<<endl;
        }
        else
            symbolTable[position].value = e.val;
         
         
            irCode<<"mov r0,r"<<e.reg<<endl;    

        
        if(!semiColon(token[0]))
        {
            cout<<line<<": expected a ;"<<endl;
        }


        token = nextToken(file);
    }
    else if(openBrace(token[0]))
    {
        token = nextToken(file);
        body(file,irCode,position,symbolTable,token);
        if(!closeBrace(token[0]))
        {
            cout<<line<<": expected a }"<<endl;
        }
        token = nextToken(file);
    }
    else if(!closeBrace(token[0]))
    {
        expr(file,irCode,symbolTable,token);
        if(!semiColon(token[0]))
        {
            cout<<line<<": expected a ;"<<endl;
        }

        token = nextToken(file);
    }
    
}

exprVal expr(ifstream &file,ofstream &irCode, vector<table> &symbolTable, string &token)
{
    exprVal t;
    t = expr1(file,irCode,symbolTable,token);
    bool error = true;
    while(equal(token[0]))
    {
        int f = search(symbolTable,t.tok);
        token = nextToken(file);
        exprVal t1 = expr(file,irCode,symbolTable,token);
        
        if(t.type == 0 || t1.type == 0 && error)
        {
            error = false;
            cout<<line<<": variable not declared!"<<endl;
        }
        else if((t.type == T_list && t1.type == T_num) || (t.type == T_num && t1.type == T_list)
        || ((isDoubleNumber(t.tok) || isIntNumber(t.tok)) &&
            (isDoubleNumber(token) || isIntNumber(token))) && error)
        {
            t.type = 0;
            cout<<line<<": illegal assignment!"<<endl;
        }
        else if(f != -1)
        {
            if(symbolTable[f].isFunc)
            {
                cout<<line<<": can't assign to functions!"<<endl;
            }
            else 
            {
                symbolTable[f].value = t1.val;
                if(t.type == T_num && symbolTable[f].type == T_list)
                {
                    irCode<<"st r"<<t1.reg<<",r"<<symbolTable[f].reg<<endl;
                }
                else if(t.type == T_list)
                {
                    symbolTable[f].len = t1.val.size();
                    if(Letter(t1.tok))
                    {
                        irCode<<"mov r"<<symbolTable[f].reg<<",r"<<t1.reg<<endl;
                    }
                    else if(vec(t1.tok))
                    {
                        symbolTable[f].len = t1.len;
                        irCode<<"mov r"<<symbolTable[f].reg<<",r"<<t1.reg<<endl;
                        irCode<<"call mem,r"<<symbolTable[f].reg<<endl;
                        irCode<<"mov r"<<symbolTable[f].reg<<",r"<<t1.lenReg<<endl;
                    }
                }
                else
                {
                    irCode<<"mov r"<<symbolTable[f].reg<<",r"<<t1.reg<<endl;
                }

                t.val = t1.val;
            }    
        }
            
    }

    return t;   
}

void generateCodeForAddOrSub(ofstream &irCode, exprVal &token1, exprVal &token2,
    string cur, vector<table> symbolTable, int f1,bool &firstMovOpr)
{
    if(Letter(token1.tok) || isKey(token1.tok) && f1!=-1)
    {
        if(Letter(token2.tok) || isKey(token2.tok))
        {
            int f2 = search(symbolTable,token2.tok);
            if(f2!=-1)
            {
                if(symbolTable[f1].type == T_list)
                {
                   irCode<<"ld r"<<reg<<",r"<<symbolTable[f1].reg<<endl;
                   irCode<<"sub r"<<symbolTable[f1].reg<<",r"<<symbolTable[f1].reg
                        <<",r"<<reg-1<<endl;
                    
                    irCode<<"mov r"<<reg-1<<",r"<<reg<<endl;
                    if(symbolTable[f2].type == T_list)
                    {
                        irCode<<"ld r"<<reg<<",r"<<symbolTable[f2].reg<<endl;
                        irCode<<"sub r"<<symbolTable[f2].reg<<",r"<<
                            symbolTable[f2].reg<<",r"<<reg-1<<endl;
                        irCode<<"mov r"<<reg-1<<",r"<<reg<<endl;
                        if(add(cur[0]))
                            irCode<<"add r"<<reg-1<<",";
                        else
                            irCode<<"sub r"<<reg-1<<",";
                        irCode<<"r"<<symbolTable[f1].reg<<",r"<<reg-1<<endl;
                        token1.reg = reg-1;
                    }
                    else
                    {
                        if(add(cur[0]))
                            irCode<<"add r"<<reg-1<<",";
                        else
                            irCode<<"sub r"<<reg-1<<",";
                        irCode<<"r"<<reg-1<<",r"<<token2.reg<<endl;
                        
                        token1.reg = reg-1;
                    }
                }
                else
                {
                    if(symbolTable[f2].type == T_list)
                    {
                        irCode<<"ld r"<<reg<<",r"<<symbolTable[f2].reg<<endl;
                        irCode<<"sub r"<<symbolTable[f2].reg<<",r"<<
                            symbolTable[f2].reg<<",r"<<reg-1<<endl;
                        irCode<<"mov r"<<reg-1<<",r"<<reg<<endl;
                        if(add(cur[0]))
                            irCode<<"add r"<<reg-1<<",";
                        else
                            irCode<<"sub r"<<reg-1<<",";
                        irCode<<"r"<<symbolTable[f1].reg<<",r"<<reg-1<<endl;

                        token1.reg = reg-1;
                    }
                    else
                    {
                        if(add(cur[0]))
                            irCode<<"add r"<<reg<<",";
                        else
                            irCode<<"sub r"<<reg<<",";
                        irCode<<"r"<<symbolTable[f1].reg<<",r"<<token2.reg<<endl;
                        token1.reg = reg;
                        reg++;
                    }
                }
            }
        }
        else
        {
            if(add(cur[0]))
                irCode<<"add r"<<reg-1<<",";
            else
                irCode<<"sub r"<<reg-1<<",";

            if(symbolTable[f1].type == T_list)
                irCode<<"r"<<reg-2<<",r"<<reg-1<<endl;
            else
                irCode<<"r"<<symbolTable[f1].reg<<",r"<<reg-1<<endl;

            token1.reg = reg-1;
        }
    }
    else 
    {
        if(firstMovOpr)
        {
            irCode<<"mov r"<<reg<<","<<token1.tok<<endl;
            token1.reg = reg;
            reg++;
            firstMovOpr = false;
        }
        if(Letter(token2.tok) || isKey(token2.tok))
        {
            int f2 = search(symbolTable,token2.tok);
            if(f2!=-1)
            {

                if(token2.type == T_num && symbolTable[f2].type == T_list)
                {
                    irCode<<"ld r"<<reg<<",r"<<symbolTable[f2].reg<<endl;
                    irCode<<"sub r"<<symbolTable[f2].reg<<",r"<<symbolTable[f2].reg
                        <<",r"<<reg-1<<endl;
                    irCode<<"mov r"<<reg-1<<",r"<<reg<<endl;
                    if(add(cur[0]))
                        irCode<<"add r"<<reg-1<<",";
                    else
                        irCode<<"sub r"<<reg-1<<",";
                    irCode<<"r"<<reg-2<<",r"<<reg-1<<endl;

                    token1.reg = reg-1;
                }
                else 
                {
                    if(add(cur[0]))
                        irCode<<"add r"<<token1.reg<<",";
                    else
                        irCode<<"sub r"<<token1.reg<<",";
                    irCode<<"r"<<token1.reg<<",r"<<token2.reg<<endl;
                }
            }
        }
        else
        {
            irCode<<"mov r"<<reg<<","<<token2.tok<<endl;
            token2.reg = reg;
            reg++;
            if(add(cur[0]))
                irCode<<"add r"<<reg-1<<",";
            else
                irCode<<"sub r"<<reg-1<<",";
            irCode<<"r"<<reg-2<<",r"<<reg-1<<endl;
            token1.reg = reg-1;
        }
    }
    firstMovOpr = false;
}

exprVal expr1(ifstream &file,ofstream &irCode, vector<table> &symbolTable, string &token)
{
    exprVal t;
    t = expr2(file,irCode,symbolTable,token);
    
    bool noError = true;

    bool firstMovOpr = true;
    while(sub(token[0]) || add(token[0]))
    {
        string cur = token;

        int f1;
        if(Letter(t.tok))
        {
            f1 = search(symbolTable,t.tok);
            if(f1 !=-1 && t.type == T_num && symbolTable[f1].type == T_list)
            {
                irCode<<"ld r"<<reg<<",r"<<symbolTable[f1].reg<<endl;
                irCode<<"sub r"<<symbolTable[f1].reg<<",r"<<symbolTable[f1].reg<<",r"<<reg-1<<endl;
                irCode<<"mov r"<<reg-1<<",r"<<reg<<endl;
            }
        }

        token = nextToken(file);
        exprVal t1 = expr2(file,irCode, symbolTable,token);
        if(t.type == 0 || t1.type == 0 && noError)
        {
            noError = false;
            cout<<line<<": variable not declared!"<<endl;
        }
        else if(t.type == T_list || t1.type == T_list && noError)
        {
            cout<<line<<": incompatible operands!"<<endl;
        }
        else
        {
            if(add(cur[0]))
                t.val[t.pos] = t.val[t.pos] + t1.val[t1.pos];
            else
                t.val[t.pos] = t.val[t.pos] - t1.val[t1.pos];

            generateCodeForAddOrSub(irCode,t,t1,cur,symbolTable,f1,firstMovOpr); 

            t.tok = t.val[t.pos];
        }  
    }

    return t;
}

void generateCodeForOperands(ofstream &irCode, exprVal &token1, exprVal &token2,
    string cur, vector<table> symbolTable, int f1,bool &firstMovOpr)
{
    if(Letter(token1.tok) || isKey(token1.tok) && f1!=-1)
    {
        if(Letter(token2.tok) || isKey(token2.tok))
        {
            int f2 = search(symbolTable,token2.tok);
            if(f2!=-1)
            {
                if(symbolTable[f1].type == T_list)
                {
                   irCode<<"ld r"<<reg<<",r"<<symbolTable[f1].reg<<endl;
                   irCode<<"sub r"<<symbolTable[f1].reg<<",r"<<symbolTable[f1].reg
                        <<",r"<<reg-1<<endl;
                    
                    irCode<<"mov r"<<reg-1<<",r"<<reg<<endl;
                    if(symbolTable[f2].type == T_list)
                    {
                        irCode<<"ld r"<<reg<<",r"<<symbolTable[f2].reg<<endl;
                        irCode<<"sub r"<<symbolTable[f2].reg<<",r"<<
                            symbolTable[f2].reg<<",r"<<reg-1<<endl;
                        irCode<<"mov r"<<reg-1<<",r"<<reg<<endl;
                        
                        if(mul(cur[0]))
                            irCode<<"mul r"<<reg-1<<",";
                        else if(div(cur[0]))
                            irCode<<"div r"<<reg-1<<",";
                        else if(mod(cur[0]))
                            irCode<<"mod r"<<reg-1<<",";
                        else if(greaterThan(cur[0]))
                            irCode<<"cmp> r"<<reg-1<<",";
                        else if(lessThan(cur[0]))
                            irCode<<"cmp< r"<<reg-1<<",";
                        else if(greaterThan(cur[0]) && equal(cur[1]))
                            irCode<<"cmp>= r"<<reg-1<<",";
                        else if(lessThan(cur[0]) && equal(cur[1]))
                            irCode<<"cmp<= r"<<reg-1<<",";
                        else if(equal(cur[0]) && equal(cur[1]))
                            irCode<<"cmp= r"<<reg-1<<",";
                        else if(Or(cur[0]) && Or(cur[1]))
                            irCode<<"mul r"<<reg-1<<",";
                        else
                            irCode<<"add r"<<reg-1<<",";

                        irCode<<"r"<<symbolTable[f1].reg<<",r"<<reg-1<<endl;

                        token1.reg = reg-1;
                    }
                    else
                    {
                        if(mul(cur[0]))
                            irCode<<"mul r"<<reg-1<<",";
                        else if(div(cur[0]))
                            irCode<<"div r"<<reg-1<<",";
                        else if(mod(cur[0]))
                            irCode<<"mod r"<<reg-1<<",";
                        else if(greaterThan(cur[0]))
                            irCode<<"cmp> r"<<reg-1<<",";
                        else if(lessThan(cur[0]))
                            irCode<<"cmp< r"<<reg-1<<",";
                        else if(greaterThan(cur[0]) && equal(cur[1]))
                            irCode<<"cmp>= r"<<reg-1<<",";
                        else if(lessThan(cur[0]) && equal(cur[1]))
                            irCode<<"cmp<= r"<<reg-1<<",";
                        else if(equal(cur[0]) && equal(cur[1]))
                            irCode<<"cmp= r"<<reg-1<<",";
                        else if(Or(cur[0]) && Or(cur[1]))
                            irCode<<"mul r"<<reg-1<<",";
                        else
                            irCode<<"add r"<<reg-1<<",";

                        irCode<<"r"<<reg-1<<",r"<<token2.reg<<endl;

                        token1.reg = reg-1;
                    }
                }
                else
                {
                    if(symbolTable[f2].type == T_list)
                    {
                        irCode<<"ld r"<<reg<<",r"<<symbolTable[f2].reg<<endl;
                        irCode<<"sub r"<<symbolTable[f2].reg<<",r"<<
                            symbolTable[f2].reg<<",r"<<reg-1<<endl;
                        irCode<<"mov r"<<reg-1<<",r"<<reg<<endl;

                        if(mul(cur[0]))
                            irCode<<"mul r"<<reg-1<<",";
                        else if(div(cur[0]))
                            irCode<<"div r"<<reg-1<<",";
                        else if(mod(cur[0]))
                            irCode<<"mod r"<<reg-1<<",";
                        else if(greaterThan(cur[0]))
                            irCode<<"cmp> r"<<reg-1<<",";
                        else if(lessThan(cur[0]))
                            irCode<<"cmp< r"<<reg-1<<",";
                        else if(greaterThan(cur[0]) && equal(cur[1]))
                            irCode<<"cmp>= r"<<reg-1<<",";
                        else if(lessThan(cur[0]) && equal(cur[1]))
                            irCode<<"cmp<= r"<<reg-1<<",";
                        else if(equal(cur[0]) && equal(cur[1]))
                            irCode<<"cmp= r"<<reg-1<<",";
                        else if(Or(cur[0]) && Or(cur[1]))
                            irCode<<"mul r"<<reg-1<<",";
                        else
                            irCode<<"add r"<<reg-1<<",";

                        irCode<<"r"<<symbolTable[f1].reg<<",r"<<reg-1<<endl;

                        token1.reg = reg-1;
                    }
                    else
                    {
                        if(mul(cur[0]))
                            irCode<<"mul r"<<reg<<",";
                        else if(div(cur[0]))
                            irCode<<"div r"<<reg<<",";
                        else if(mod(cur[0]))
                            irCode<<"mod r"<<reg<<",";
                        else if(greaterThan(cur[0]))
                            irCode<<"cmp> r"<<reg<<",";
                        else if(lessThan(cur[0]))
                            irCode<<"cmp< r"<<reg<<",";
                        else if(greaterThan(cur[0]) && equal(cur[1]))
                            irCode<<"cmp>= r"<<reg<<",";
                        else if(lessThan(cur[0]) && equal(cur[1]))
                            irCode<<"cmp<= r"<<reg<<",";
                        else if(equal(cur[0]) && equal(cur[1]))
                            irCode<<"cmp= r"<<reg<<",";
                        else if(Or(cur[0]) && Or(cur[1]))
                            irCode<<"mul r"<<reg<<",";
                        else
                            irCode<<"add r"<<reg<<",";

                        irCode<<"r"<<symbolTable[f1].reg<<",r"<<token2.reg<<endl;

                        token1.reg = reg;
                        reg++;
                    }
                }
            }
        }
        else
        {
            if(mul(cur[0]))
                irCode<<"mul r"<<reg-1<<",";
            else if(div(cur[0]))
                irCode<<"div r"<<reg-1<<",";
            else if(mod(cur[0]))
                irCode<<"mod r"<<reg-1<<",";
            else if(greaterThan(cur[0]))
                irCode<<"cmp> r"<<reg-1<<",";
            else if(lessThan(cur[0]))
                irCode<<"cmp< r"<<reg-1<<",";
            else if(greaterThan(cur[0]) && equal(cur[1]))
                irCode<<"cmp>= r"<<reg-1<<",";
            else if(lessThan(cur[0]) && equal(cur[1]))
                irCode<<"cmp<= r"<<reg-1<<",";
            else if(equal(cur[0]) && equal(cur[1]))
                irCode<<"cmp= r"<<reg-1<<",";
            else if(Or(cur[0]) && Or(cur[1]))
                irCode<<"mul r"<<reg-1<<",";
            else
                irCode<<"add r"<<reg-1<<",";

            if(symbolTable[f1].type == T_list)
                irCode<<"r"<<reg-2<<",r"<<reg-1<<endl;
            else
                irCode<<"r"<<symbolTable[f1].reg<<",r"<<reg-1<<endl;

            token1.reg = reg-1;
        }
    }
    else 
    {
        if(firstMovOpr)
        {
            irCode<<"mov r"<<reg<<","<<token1.tok<<endl;
            token1.reg = reg;
            reg++;
            firstMovOpr = false;
        }
        if(Letter(token2.tok) || isKey(token2.tok))
        {
            int f2 = search(symbolTable,token2.tok);
            if(f2!=-1)
            {

                if(token2.type == T_num && symbolTable[f2].type == T_list)
                {
                    irCode<<"ld r"<<reg<<",r"<<symbolTable[f2].reg<<endl;
                    irCode<<"sub r"<<symbolTable[f2].reg<<",r"<<symbolTable[f2].reg
                        <<",r"<<reg-1<<endl;
                    irCode<<"mov r"<<reg-1<<",r"<<reg<<endl;

                    if(mul(cur[0]))
                        irCode<<"mul r"<<reg-1<<",";
                    else if(div(cur[0]))
                        irCode<<"div r"<<reg-1<<",";
                    else if(mod(cur[0]))
                        irCode<<"mod r"<<reg-1<<",";
                    else if(greaterThan(cur[0]))
                        irCode<<"cmp> r"<<reg-1<<",";
                    else if(lessThan(cur[0]))
                        irCode<<"cmp< r"<<reg-1<<",";
                    else if(greaterThan(cur[0]) && equal(cur[1]))
                        irCode<<"cmp>= r"<<reg-1<<",";
                    else if(lessThan(cur[0]) && equal(cur[1]))
                        irCode<<"cmp<= r"<<reg-1<<",";
                    else if(equal(cur[0]) && equal(cur[1]))
                        irCode<<"cmp= r"<<reg-1<<",";
                    else if(Or(cur[0]) && Or(cur[1]))
                        irCode<<"mul r"<<reg-1<<",";
                    else
                        irCode<<"add r"<<reg-1<<",";

                    irCode<<"r"<<reg-2<<",r"<<reg-1<<endl;

                    token1.reg = reg-1;
                }
                else 
                {
                    if(mul(cur[0]))
                        irCode<<"mul r"<<token1.reg<<",";
                    else if(div(cur[0]))
                        irCode<<"div r"<<token1.reg<<",";
                    else if(mod(cur[0]))
                        irCode<<"mod r"<<token1.reg<<",";
                    else if(greaterThan(cur[0]))
                        irCode<<"cmp> r"<<token1.reg<<",";
                    else if(lessThan(cur[0]))
                        irCode<<"cmp< r"<<token1.reg<<",";
                    else if(greaterThan(cur[0]) && equal(cur[1]))
                        irCode<<"cmp>= r"<<token1.reg<<",";
                    else if(lessThan(cur[0]) && equal(cur[1]))
                        irCode<<"cmp<= r"<<token1.reg<<",";
                    else if(equal(cur[0]) && equal(cur[1]))
                        irCode<<"cmp= r"<<token1.reg<<",";
                    else if(Or(cur[0]) && Or(cur[1]))
                        irCode<<"mul r"<<token1.reg<<",";
                    else
                        irCode<<"add r"<<token1.reg<<",";

                    irCode<<"r"<<token1.reg<<",r"<<token2.reg<<endl;
                }
            }
        }
        else
        {
            irCode<<"mov r"<<reg<<","<<token2.tok<<endl;
            token2.reg = reg;
            reg++;

            if(mul(cur[0]))
                irCode<<"mul r"<<reg-1<<",";
            else if(div(cur[0]))
                irCode<<"div r"<<reg-1<<",";
            else if(mod(cur[0]))
                irCode<<"mod r"<<reg-1<<",";
            else if(greaterThan(cur[0]))
                irCode<<"cmp> r"<<reg-1<<",";
            else if(lessThan(cur[0]))
                irCode<<"cmp< r"<<reg-1<<",";
            else if(greaterThan(cur[0]) && equal(cur[1]))
                irCode<<"cmp>= r"<<reg-1<<",";
            else if(lessThan(cur[0]) && equal(cur[1]))
                irCode<<"cmp<= r"<<reg-1<<",";
            else if(equal(cur[0]) && equal(cur[1]))
                irCode<<"cmp= r"<<reg-1<<",";
            else if(Or(cur[0]) && Or(cur[1]))
                irCode<<"mul r"<<reg-1<<",";
            else
                irCode<<"add r"<<reg-1<<",";

            irCode<<"r"<<reg-2<<",r"<<reg-1<<endl;
            token1.reg = reg-1;
        }
    }

    firstMovOpr = false;
}

exprVal expr2(ifstream &file,ofstream &irCode, vector<table> &symbolTable, string &token)
{
    exprVal t;
    t = expr3(file,irCode,symbolTable,token);
    bool noError = true;
    bool firstMovOpr = true;
    while(mul(token[0]) || div(token[0]) || mod(token[0]))
    {
        string cur = token;

        int f1;
        if(Letter(t.tok))
        {
            f1 = search(symbolTable,t.tok);
            if(f1 !=-1 && t.type == T_num && symbolTable[f1].type == T_list)
            {
                irCode<<"ld r"<<reg<<",r"<<symbolTable[f1].reg<<endl;
                irCode<<"sub r"<<symbolTable[f1].reg<<",r"<<symbolTable[f1].reg<<",r"<<reg-1<<endl;
                irCode<<"mov r"<<reg-1<<",r"<<reg<<endl;
            }
        }

        token = nextToken(file);
        exprVal t1 = expr3(file,irCode,symbolTable,token);

        if(t.type == 0 || t1.type == 0 && noError)
        {
            noError = false;
            cout<<line<<": variable not declared!"<<endl;
        }
        else if((t.type == T_list && t1.type == T_num) || (t.type == T_num && t.type == T_list) && noError)
        {
            noError = false;
            cout<<line<<": incompatible operands!"<<endl;
        }
        else
        {
            if(mul(cur[0]))
                t.val[t.pos] = t.val[t.pos] * t1.val[t1.pos];
            else if(div(cur[0]))
                if(t1.val[t1.pos] == 0)
                {
                    cout<<line<<": cannot divide by 0!"<<endl;
                    break;
                }
                else
                    t.val[t.pos] = t.val[t.pos] / t1.val[t1.pos];
            else
                if(t1.val[t1.pos] == 0)
                {
                    cout<<line<<": cannot divide by 0!"<<endl;
                    break;
                }
                else
                    t.val[t.pos] = (int) t.val[t.pos] % (int) t1.val[t1.pos];

            generateCodeForOperands(irCode,t,t1,cur,symbolTable,f1,firstMovOpr);

            t.tok = t.val[t.pos];
        }  
    }

    return t;
}


exprVal expr3(ifstream &file, ofstream &irCode, vector<table> &symbolTable, string &token)
{
    exprVal t;
    t = expr4(file,irCode,symbolTable,token);
    bool noError = true;
    bool firstMovOpr = true;
    while(greaterThan(token[0]) || lessThan(token[0]) ||
    (equal(token[0]) && equal(token[1])) || (greaterThan(token[0]) && equal(token[1])) ||
    (lessThan(token[0]) && equal(token[1])))
    {
        string cur = token;

        int f1;
        if(Letter(t.tok))
        {
            f1 = search(symbolTable,t.tok);
            if(f1 !=-1 && t.type == T_num && symbolTable[f1].type == T_list)
            {
                irCode<<"ld r"<<reg<<",r"<<symbolTable[f1].reg<<endl;
                irCode<<"sub r"<<symbolTable[f1].reg<<",r"<<symbolTable[f1].reg<<",r"<<reg-1<<endl;
                irCode<<"mov r"<<reg-1<<",r"<<reg<<endl;
            }
        }

        token = nextToken(file);
        exprVal t1 = expr4(file,irCode,symbolTable,token);
        
        if(t.type == 0 || t1.type == 0 && noError)
        {
            noError = false;
            cout<<line<<": variable not declared!"<<endl;
        }
        else if((t.type == T_list && t1.type == T_num) || (t.type == T_num && t.type == T_list) && noError)
        {
            noError = false;
            cout<<line<<": incompatible comparation!"<<endl;
        }
        else
        {
            if(greaterThan(cur[0]))
                t.val[t.pos] = t.val[t.pos] > t1.val[t1.pos];
            else if(lessThan(cur[0]))
                t.val[t.pos] = t.val[t.pos] < t1.val[t1.pos];
            else if(equal(token[0]) && equal(token[1]))
                t.val[t.pos] = t.val[t.pos] == t1.val[t1.pos];
            else if(greaterThan(token[0]) && equal(token[1]))
                t.val[t.pos] = t.val[t.pos] >= t1.val[t1.pos];
            else
                t.val[t.pos] = t.val[t.pos] <= t1.val[t1.pos];

            generateCodeForOperands(irCode,t,t1,cur,symbolTable,f1,firstMovOpr);

            t.tok = t.val[t.pos];
        }
    }

    return t; 
}

exprVal expr4(ifstream &file, ofstream &irCode, vector<table> &symbolTable, string &token)
{
    exprVal t;
    t = expr5(file,irCode,symbolTable,token);
    bool noError = true;
    bool firstMovOpr = true;
    while((Or(token[0]) && Or(token[1])) || (And(token[0]) && And(token[1])))
    {
        string cur = token;

        int f1;
        if(Letter(t.tok))
        {
            f1 = search(symbolTable,t.tok);
            if(f1 !=-1 && t.type == T_num && symbolTable[f1].type == T_list)
            {
                irCode<<"ld r"<<reg<<",r"<<symbolTable[f1].reg<<endl;
                irCode<<"sub r"<<symbolTable[f1].reg<<",r"<<symbolTable[f1].reg<<",r"<<reg-1<<endl;
                irCode<<"mov r"<<reg-1<<",r"<<reg<<endl;
            }
        }

        token = nextToken(file);
        exprVal t1 = expr5(file,irCode,symbolTable,token);
        
        if(t.type == 0 || t1.type == 0 && noError)
        {
            noError = false;
            cout<<line<<": variable not declared!"<<endl;
        }
        else if((t.type == T_list && t1.type == T_num) || (t.type == T_num && t.type == T_list) && noError)
        {
            noError = false;
            cout<<line<<": incompatible logical operands!"<<endl;
        }
        else
        {
            if(Or(token[0]) && Or(token[1]))
                t.val[t.pos] = t.val[t.pos] || t1.val[t1.pos];
            else 
                t.val[t.pos] = t.val[t.pos] && t1.val[t1.pos];

            generateCodeForOperands(irCode,t,t1,cur,symbolTable,f1,firstMovOpr);

            t.tok = t.val[t.pos];   
        }
    }

    return t;
}

exprVal expr5(ifstream &file,ofstream &irCode, vector<table> &symbolTable, string &token)
{
    exprVal t;
    t = expr6(file,irCode,symbolTable,token);
    bool noError = true;
    if(openBracket(token[0]))
    {
        token = nextToken(file);
        exprVal t1 = expr7(file,irCode,symbolTable,token);
        if(t.type == 0 || t1.type == 0 && noError)
        {
            noError = false;
            cout<<line<<": variable not declared!"<<endl;
        }
        else if(t.type == T_num && noError)
        {
            noError = false;
            if(isIntNumber(t.tok) || isDoubleNumber(t.tok))
                cout<<line<<": number is ambigous"<<endl;

            cout<<line<<": must be list"<<endl;
        }
        else if(t1.type == T_list && noError)
        {
            noError=false;
            cout<<line<<": illegal type!"<<endl;
        }
        if(!closeBracket(token[0]))
        {
            noError = false;
            cout<<line<<": expected a ]"<<endl;
        }
        else
        {
            t.type = T_num;
            t.pos = t1.val[t1.pos];
            irCode<<"mov r"<<reg<<",8"<<endl;
            irCode<<"mul r"<<reg-1<<",r"<<reg-1<<",r"<<reg<<endl;
            reg++;
            int f = search(symbolTable,t.tok);
            if(f!=-1)
            {    
                irCode<<"add r"<<symbolTable[f].reg<<",r"<<symbolTable[f].reg<<",r"<<reg-1<<endl;

                t.reg = symbolTable[f].reg;   
            }
        }

        token = nextToken(file);
    }

    return t;
}

exprVal expr6(ifstream &file, ofstream &irCode, vector<table> &symbolTable, string &token)
{
    exprVal ret;
    if(Not(token[0]))
    {
        token = nextToken(file);
        ret = expr(file,irCode,symbolTable,token);
        if(ret.type == T_list)
            cout<<line<<":illegal operand!"<<endl;
        else
        {
            ret.val[ret.pos] = !ret.val[ret.pos];

            irCode<<"mov r"<<reg<<",0"<<endl;
            irCode<<"mov r"<<reg+1<<",1"<<endl;
            irCode<<"cmp= r"<<reg+2<<",";
            int f;
            if(Letter(ret.tok))
            {
                f = search(symbolTable,ret.tok);
                if(f!=-1)
                    irCode<<"r"<<symbolTable[f].reg<<",r"<<reg<<endl;
            }
            else
                irCode<<"r"<<reg-1<<",r"<<reg<<endl;

            irCode<<"jz r"<<reg+2<<",label"<<label<<endl;
            irCode<<"mov r";
            if(Letter(ret.tok))
            {
                if(f!=-1)
                    irCode<<symbolTable[f].reg<<",r"<<reg+1<<endl;
            }
            else 
                irCode<<reg-1<<",r"<<reg+1<<endl;

            irCode<<"jmp label"<<label+1<<endl;

            irCode<<"label"<<label<<":mov r";
            if(Letter(ret.tok))
            {
                if(f!=-1)
                    irCode<<symbolTable[f].reg<<",0"<<endl;
            }
            else 
                irCode<<reg-1<<",0"<<endl;

            label++;
            irCode<<"label"<<label<<":"<<endl;

            label++;
            reg+=3;
        }
    }
    else if(sub(token[0]))
    {
        token = nextToken(file);
        ret = expr(file,irCode,symbolTable,token);
        if(ret.type == T_list)
            cout<<line<<":illegal operand!"<<endl;
        else
        {
            ret.val[ret.pos] = -ret.val[ret.pos];

            irCode<<"mov r"<<reg<<",-1"<<endl;

            if(Letter(ret.tok))
            {
                int f = search(symbolTable,ret.tok);
                if(f!=-1)
                    irCode<<"mul r"<<symbolTable[f].reg<<",r"<<symbolTable[f].reg<<",r"<<reg<<endl;
            }
            else
                irCode<<"mul r"<<reg-1<<",r"<<reg-1<<",r"<<reg<<endl;

            reg++;
        }
    }   
    else if(add(token[0]))
    {
        token = nextToken(file);
        ret = expr(file,irCode,symbolTable,token);
        if(ret.type == T_list)
            cout<<line<<":illegal operand!"<<endl;
    }
    else if( openParenthesis(token[0]))
    {
        token = nextToken(file);
        ret = expr(file,irCode,symbolTable,token);
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
            ret.val = symbolTable[s].value;
            ret.reg = symbolTable[s].reg;
            token = nextToken(file);
            
            if(openParenthesis(token[0]))
            {
                if(symbolTable[s].isFunc)
                {
                    token = nextToken(file);
                    vector<exprVal> params;
                    clist(file,irCode,s,symbolTable,token,params);
                    if(closeParenthesis(token[0]))
                    {
                        if(params.size() != symbolTable[s].numberOfParams)
                            cout<<line<<": too few arguments for a function call"<<endl;
                        else
                        {
                            symbolTable[s].reg = params[0].reg;
                            ret.reg = reg;
                            irCode<<"mov r"<<reg<<",r"<<params[0].reg<<endl;
                            irCode<<"call "<<symbolTable[s].variable;
                            irCode<<",r"<<reg;
                            reg++;
                            for(int i=1;i<params.size();i++)
                                irCode<<",r"<<params[i].reg;
                            irCode<<endl;
                        }
                        
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
        ret.val.push_back(atoi(token.c_str()));
        token = nextToken(file);
        if(semiColon(token[0]) || closeParenthesis(token[0]) || comma(token[0]))
        {
            irCode<<"mov r"<<reg<<","<<ret.tok<<endl;
            ret.reg = reg;
            reg++;
        }
    }
    else if(numread(token))
    {
        ret.type = T_num;
        ret.tok = token;
        token = nextToken(file);
        if(openParenthesis(token[0]))
        {
            token = nextToken(file);
            vector<exprVal> params;
            clist(file,irCode,-1,symbolTable,token,params);
            if(closeParenthesis(token[0]))
            {
                if(params.size()>0)
                    cout<<line<<"illegal parameter!"<<endl;
                else
                {
                    irCode<<"call iget,r"<<reg<<endl;
                    ret.reg = reg;
                    reg++;
                }
                
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
            vector<exprVal> params;
            clist(file,irCode,1,symbolTable,token,params);
            if(closeParenthesis(token[0]))
            {
                if(params.size() != 1)
                    cout<<line<<"illegal parameter!"<<endl;
                else
                {
                    irCode<<"call iput,r"<<params[0].reg<<endl;
                }
                
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
            vector<exprVal> params;
            clist(file,irCode,2,symbolTable,token,params);
            ret.len = params[0].val[params[0].pos];
            if(closeParenthesis(token[0]))
            {
                if(params.size() != 1)
                    cout<<line<<"illegal parameter!"<<endl;
                else
                {
                    irCode<<"mov r"<<reg<<",8"<<endl;
                    reg++;
                    irCode<<"mov r"<<reg<<",1"<<endl;
                    reg++;
                    irCode<<"mov r"<<reg<<",r"<<params[0].reg<<endl;
                    irCode<<"add r"<<reg<<",r"<<reg<<",r"<<reg-1<<endl;

                    irCode<<"mul r"<<reg<<",r"<<reg<<",r"<<reg-1<<endl;
                    ret.reg = reg;
                    ret.lenReg = params[0].reg;
                    reg++;
                }
                
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
            vector<exprVal> params;
            clist(file,irCode,3,symbolTable,token,params);
            if(closeParenthesis(token[0]))
            {
                if(params.size() != 1)
                    cout<<line<<"illegal parameter!"<<endl;
                else
                {
                    irCode<<"mov r"<<reg<<",r"<<params[0].reg<<endl;
                    ret.reg = reg;
                    reg++;
                }
                
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
            vector<exprVal> params;
            clist(file,irCode,4,symbolTable,token,params);
            if(closeParenthesis(token[0]))
            {
                if(params.size() != 1)
                    cout<<line<<"illegal parameter!"<<endl;
                else
                    irCode<<"ret"<<endl;
                
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

exprVal expr7(ifstream &file, ofstream &irCode, vector<table> &symbolTable, string &token)
{
    exprVal ret;
    if(Not(token[0]))
    {
        token = nextToken(file);
        ret = expr(file,irCode,symbolTable,token);
        if(ret.type == T_list)
            cout<<line<<":illegal operand!"<<endl;
        else
        {
            ret.val[ret.pos] = !ret.val[ret.pos];

            irCode<<"mov r"<<reg<<",0"<<endl;
            irCode<<"mov r"<<reg+1<<",1"<<endl;
            irCode<<"cmp= r"<<reg+2<<",";
            int f;
            if(Letter(ret.tok))
            {
                f = search(symbolTable,ret.tok);
                if(f!=-1)
                    irCode<<"r"<<symbolTable[f].reg<<",r"<<reg<<endl;
            }
            else
                irCode<<"r"<<reg-1<<",r"<<reg<<endl;

            irCode<<"jz r"<<reg+2<<",label"<<label<<endl;
            irCode<<"mov r";
            if(Letter(ret.tok))
            {
                if(f!=-1)
                    irCode<<symbolTable[f].reg<<",r"<<reg+1<<endl;
            }
            else 
                irCode<<reg-1<<",r"<<reg+1<<endl;

            irCode<<"jmp label"<<label+1<<endl;

            irCode<<"label"<<label<<":mov r";
                        if(Letter(ret.tok))
            {
                if(f!=-1)
                    irCode<<symbolTable[f].reg<<",0"<<endl;
            }
            else 
                irCode<<reg-1<<",0"<<endl;

            label++;
            irCode<<"label"<<label<<":"<<endl;

            label++;
            reg+=3;
        }
    }
    else if(sub(token[0]))
    {
        token = nextToken(file);
        ret = expr(file,irCode,symbolTable,token);
        if(ret.type == T_list)
            cout<<line<<":illegal operand!"<<endl;
        else
        {
            ret.val[ret.pos] = -ret.val[ret.pos];

            irCode<<"mov r"<<reg<<",-1"<<endl;

            if(Letter(ret.tok))
            {
                int f = search(symbolTable,ret.tok);
                if(f!=-1)
                    irCode<<"mul r"<<symbolTable[f].reg<<",r"<<symbolTable[f].reg<<",r"<<reg<<endl;
            }
            else
                irCode<<"mul r"<<reg-1<<",r"<<reg-1<<",r"<<reg<<endl;

            reg++;
        }
    }  
    else if(add(token[0]))
    {
        token = nextToken(file);
        ret = expr(file,irCode,symbolTable,token);
        if(ret.type == T_list)
            cout<<line<<":illegal operand!"<<endl;
    }
    else if( openParenthesis(token[0]))
    {
        token = nextToken(file);
        ret = expr(file,irCode,symbolTable,token);
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
            ret.val = symbolTable[s].value;
            ret.reg = symbolTable[s].reg;
            token = nextToken(file);
            if(openParenthesis(token[0]))
            {
                if(symbolTable[s].isFunc)
                {
                    token = nextToken(file);
                    vector<exprVal> params;
                    clist(file,irCode,s,symbolTable,token,params);
                    if(closeParenthesis(token[0]))
                    {
                        if(params.size() != symbolTable[s].numberOfParams)
                            cout<<line<<": too few arguments for a function call"<<endl;
                        else
                        {
                            irCode<<"call "<<symbolTable[s].variable<<",r"<<reg;
                            for(int i=0;i<params.size();i++)
                                irCode<<",r"<<params[i].reg;
                            irCode<<endl;

                            ret.reg = reg;
                            reg++;
                        }
                        
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
        ret.val.push_back(atoi(token.c_str()));
        irCode<<"mov r"<<reg<<",1"<<endl;
        reg++;
        irCode<<"mov r"<<reg<<","<<token<<endl;
        irCode<<"add r"<<reg<<",r"<<reg<<",r"<<reg-1<<endl;
        ret.reg = reg;
        reg++;
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
            vector<exprVal> params;
            clist(file,irCode,-1,symbolTable,token,params);
            if(closeParenthesis(token[0]))
            {
                if(params.size()>0)
                    cout<<line<<"illegal parameter!"<<endl;
                else
                {
                    irCode<<"call iget,r"<<reg<<endl;
                    ret.reg = reg;
                    reg++;
                }
                
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
            vector<exprVal> params;
            clist(file,irCode,1,symbolTable,token,params);
            if(closeParenthesis(token[0]))
            {
                if(params.size() != 1)
                    cout<<line<<"illegal parameter!"<<endl;
                else
                {
                    irCode<<"call iput,r"<<params[0].reg<<endl;
                }
                
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
            vector<exprVal> params;
            clist(file,irCode,2,symbolTable,token,params);
            ret.len = params[0].val[params[0].pos];
            if(closeParenthesis(token[0]))
            {
                if(params.size() != 1)
                    cout<<line<<"illegal parameter!"<<endl;
                else
                {
                    irCode<<"mov r"<<reg<<",8"<<endl;
                    reg++;
                    irCode<<"mov r"<<reg<<",1"<<endl;
                    reg++;
                    irCode<<"mov r"<<reg<<",r"<<params[0].reg<<endl;
                    irCode<<"add r"<<reg<<",r"<<reg<<",r"<<reg-1<<endl;

                    irCode<<"mul r"<<reg<<",r"<<reg<<",r"<<reg-1<<endl;
                    ret.reg = reg;
                    ret.lenReg = params[0].reg;
                    reg++;
                }
                
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
            vector<exprVal> params;
            clist(file,irCode,3,symbolTable,token,params);
            if(closeParenthesis(token[0]))
            {
                if(params.size() != 1)
                    cout<<line<<"illegal parameter!"<<endl;
                else
                {
                    irCode<<"mov r"<<reg<<",r"<<params[0].reg<<endl;
                    ret.reg = reg;
                    reg++;
                }
                
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
            vector<exprVal> params;
            clist(file,irCode,4,symbolTable,token,params);
            if(closeParenthesis(token[0]))
            {
                if(params.size() != 1)
                    cout<<line<<"illegal parameter!"<<endl;
                else
                    irCode<<"ret"<<endl;
                
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