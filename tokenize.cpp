#include<iostream>
#include<fstream>
#include <iostream>
#include<sstream>
#include<vector>
#include<math.h>

using namespace std;

int line = 1;
bool validCode = true;

int search(vector<pair<string,string>> symbolTable, string find)
{
    for(int i=0 ; i<symbolTable.size();i++)
        if(symbolTable[i].first == find)
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
    if(!Main(letter) && !If(letter) && !Else(letter) && !For(letter) && 
    !While(letter) && !In(letter) && !Return(letter) && !num(letter) &&
    !list(letter) && !local(letter) && !numread(letter) && !numprint(letter) &&
    !vec(letter) && !veclen(letter) && !exit(letter))
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
                while(c != '#' && !myfile.eof())    myfile.get(c);
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

void start(ifstream &file, vector<pair<string,string>> &symbolTable);
void mainCode(ifstream &file, vector<pair<string,string>> &symbolTable, string &token);
void insideMain(ifstream &file, vector<pair<string,string>> &symbolTable, string &token);
void declaration(ifstream &file, vector<pair<string,string>> &symbolTable);
void checkNumEquation(ifstream &file, vector<pair<string,string>> &symbolTable,
string &token, bool &operand,bool &illegalAssignment);
void checkNumAssignment(ifstream &file, vector<pair<string,string>> &symbolTable,
string &token, bool &operand);
void checkOperands(ifstream &file, vector<pair<string,string>> &symbolTable,
string &token, bool &operand);
void checkNumread(ifstream &file, vector<pair<string,string>>  &symbolTable,
string &token);
// void functions(ifstream &file, vector<pair<string,string>> symbolTable);


int main()
{
    const int maxv = 100000;
    ifstream file;
    file.open("tslang", ios::in);
    vector<pair<string,string>> symbolTable;
    start(file, symbolTable);
    file.close();

    return 0;
}

void start(ifstream &file, vector<pair<string,string>> &symbolTable)
{
    string token = nextToken(file);
    if(Letter(token))        
    {
        // functions(file,symbolTable);
        start(file,symbolTable);
    }
    else if(Main(token))    
    {
        mainCode(file,symbolTable,token);
    }
    else if(token == "")    return;
    else
    {
        validCode = false;
        cout<<line<<": main must be defined or key words cannot be declared as functions"<<endl;
    }
}

void mainCode(ifstream &file, vector<pair<string,string>> &symbolTable, string &token)
{
    token = nextToken(file);
    if(openParenthesis(token[0]))
    {
        token = nextToken(file);
        if(closeParenthesis(token[0]))
        {
            token = nextToken(file);
            if(colon(token[0]))
            {
                token = nextToken(file);
                if(num(token))
                {
                    token = nextToken(file);
                    if(openBrace(token[0]))
                    {
                        token = nextToken(file);
                        insideMain(file,symbolTable, token);
                    }
                    else {
                        validCode = false;
                        cout<<line<<": main function must have a body"<<endl;
                    }
                }
                else
                {
                    validCode = false;
                    cout<<line<<": main must be declared num"<<endl;
                }
            }
            else
            {
                validCode = false;
                cout<<line<<": main must be declared"<<endl;
            }
        }
        else
        {
            validCode = false;
            cout<<line<<": ) is missing"<<endl;
        }
    }
    else
    {
        validCode = false;
        cout<<line<<": must declare main as function"<<endl;
    }
}

void insideMain(ifstream &file, vector<pair<string,string>> &symbolTable, string &token)
{
    if(local(token))
    {
        declaration(file,symbolTable);
        token = nextToken(file);
        if(semiColon(token[0]))
        {
            token = nextToken(file);
            while(semiColon(token[0]))   token = nextToken(file);

            insideMain(file,symbolTable, token);
        }
        else
        {
            validCode = false;
            cout<<line<<": semicolon"<<endl;
        }
    }
    else if(Letter(token))
    {
        int s = search(symbolTable,token);
        if(s == -1)
        {
            validCode = false;
            cout<<line<<": variable not declared"<<endl;
        }
        else
        {
            string dec = symbolTable[s].second;
            if(num(dec)){
                token = nextToken(file);
                if(equal(token[0]))
                {
                    token = nextToken(file);
                    bool operand = false;
                    bool illegalAssignment = false;
                    checkNumEquation(file, symbolTable, token, operand,illegalAssignment);
                    if(!operand && illegalAssignment)
                    {
                        validCode = false;
                        cout<<line<<": illegal assignment!"<<endl;
                    }
                    insideMain(file, symbolTable, token);

                }
                else
                {
                    validCode = false;
                    cout<<line<<": = expected!"<<endl;
                }
                
            }
            else if(list(dec)){
                
            }            
        }
        
    }
    else if(numprint(token))
    {
        token = nextToken(file);
        if(openParenthesis(token[0]))
        {
            token = nextToken(file);

        }
        else
        {
            validCode = false;
            cout<<line<<": ( is missing"<<endl;
        }
    }
    else if(numread(token))
    {
    }
    else if(vec(token))
    {}
    else if(veclen(token))
    {}
    else if(exit(token))
    {}
    else if(If(token))
    {}
    else if(While(token))
    {}
    else if(For(token))
    {}
    else if(Return(token))
    {}
    else if(openBrace(token[0]))
    {}
    else if(closeBrace(token[0]))
    { 
        token = nextToken(file);
    }
    else if(colon(token[0]))
    {
        validCode = false;
        cout<<line<<": char : is ambigous"<<endl;
    }
    else if(semiColon(token[0]))
    {
        while(semiColon(token[0]))  token = nextToken(file);
        insideMain(file,symbolTable, token);
    }
    else if(num(token) || list(token))
    {
        validCode = false;
        cout<<line<<": no variable to declare"<<endl;
    }
    else if(token == "")    return;
    else if(closeParenthesis(token[0]))
    {
        validCode = false;
        cout<<line<<": expected an expression"<<endl;
    }
    else
    {
        validCode = false;
        cout<<line<<": wrong definition or not declared"<<endl;
    }
}

void declaration(ifstream &file, vector<pair<string,string>> &symbolTable)
{
    string next = nextToken(file);
    if(Letter(next)){
        if(search(symbolTable,next) == -1)
        {
            string first = next;
            next = nextToken(file);
            if(colon(next[0]))
            {
                next = nextToken(file);
                if(num(next))
                {
                    pair<string,string> s;
                    s.first = first;
                    s.second = next;
                    symbolTable.push_back(s);
                }
                else if(list(next))
                {
                    pair<string,string> s;
                    s.first = first;
                    s.second = next;
                    symbolTable.push_back(s);
                }
            }
            else
            {
                validCode = false;
                cout<<line<<": char : is needed to declare a variable"<<endl;
            }
        }
        else
        {
            validCode = false;
            cout<<line<<": this variable already has been declared"<<endl;
        }
    }
    else
    {
        validCode = false;
        cout<<line<<": key word or wrong definition of variable"<<endl;
    }
}

void checkNumEquation(ifstream &file, vector<pair<string,string>> &symbolTable, string &token, bool &operand,bool &illegalAssignment)
{
    if(isIntNumber(token) || isDoubleNumber(token))
    {
        operand = true;
        illegalAssignment = false;
        checkNumAssignment(file,symbolTable, token, operand);
    }
    else if(Letter(token))
    {
        int s = search(symbolTable, token);
        if(s == -1)
        {
            validCode = false;
            cout<<line<<": variable "+token+" not declared"<<endl;
        }
        else
        {
            operand = false;
            if(list(symbolTable[s].second))
                illegalAssignment = true;
            else
                illegalAssignment = false;
            
            token = nextToken(file);
            checkOperands(file,symbolTable,token, operand);
            if(operand)
                illegalAssignment = false;
        }
    }
    else if(numread(token))
    {
        illegalAssignment = false;

        int curLine = line;
        checkNumread(file, symbolTable, token);
        if(semiColon(token[0]))
        {
            while(semiColon(token[0]))  token = nextToken(file);
            if(closeParenthesis(token[0]))
            {
                validCode = false;
                cout<<line<<": expected an expression"<<endl;
            }
        }
        else if(!closeParenthesis(token[0]))
        {
            validCode = false;
            cout<<curLine<<": illegal parameter!"<<endl;
        }
        
    }
    else if(openParenthesis(token[0]))
    {
        token = nextToken(file);
        checkNumEquation(file,symbolTable, token, operand,illegalAssignment);
        if(!closeParenthesis(token[0]))
        {
            validCode = false;
            cout<<line<<": ) is missing!"<<endl;
        }
        else
        {
            token = nextToken(file);
            if(!semiColon(token[0]))
                checkOperands(file,symbolTable,token, operand);
        }
    }
    else
    {
        illegalAssignment = false;
        validCode = false;
        cout<<line<<": assignment is ambigous!"<<endl;
    }
}

void checkNumAssignment(ifstream &file, vector<pair<string,string>> &symbolTable, string &token, bool &operand)
{
    if(isIntNumber(token) || isDoubleNumber(token))
    {
        token = nextToken(file);
        operand = true;
        checkOperands(file, symbolTable,token, operand);
    }
    else if(Letter(token))
    {
        int s = search(symbolTable, token);
        if(s == -1)
        {
            validCode = false;
            cout<<line<<": variable "+token+" not declared"<<endl;
        }
        else
        {
            if(list(symbolTable[s].second) && operand)
            {
                validCode = false;
                cout<<line<<": incompatible operands!"<<endl;
            }
            token = nextToken(file);
            checkOperands(file, symbolTable, token, operand);
        }
    }
    else if(openParenthesis(token[0]))
    {
        token = nextToken(file);
        checkNumAssignment(file, symbolTable, token, operand);
        if(!closeParenthesis(token[0]))
        {
            validCode = false;
            cout<<line<<": ) is missing!"<<endl;
        }
        else
        {
            token = nextToken(file);
            if(!semiColon(token[0]))
                checkOperands(file,symbolTable,token, operand);
        }
    }
    else if(closeParenthesis(token[0]))
    {
        validCode = false;
        cout<<line<<": expected an expression!"<<endl;
    }
}

void checkOperands(ifstream &file, vector<pair<string,string>> &symbolTable, string &token, bool &operand)
{
    if(semiColon(token[0]))
    {
    }
    else if(add(token[0]))
    {
        token = nextToken(file);
        operand = true;
        checkNumAssignment(file, symbolTable, token, operand);
    }
    else if(sub(token[0]))
    {
        token = nextToken(file);
        checkNumAssignment(file, symbolTable, token, operand);
    }
    else if(mul(token[0]))
    {
        token = nextToken(file);
        operand = true;
        checkNumAssignment(file, symbolTable, token, operand);
    }
    else if(div(token[0]))
    {
        token = nextToken(file);
        operand = true;
        checkNumAssignment(file, symbolTable, token, operand);
    }
    else if(mod(token[0]))
    {
        token = nextToken(file);
        operand = true;
        checkNumAssignment(file, symbolTable, token, operand);
    }
    else if(closeParenthesis(token[0]))
    {
    }
    else
    {
        validCode = false;
        cout<<line<<": invalid operation!"<<endl;
    }
}

void checkNumread(ifstream &file, vector<pair<string,string>>  &symbolTable, string &token)
{
    int curLine = line;
    token = nextToken(file);
    if(openParenthesis(token[0]))
    {
        token = nextToken(file);
        if(!closeParenthesis(token[0]))
        {
            validCode = false;
            cout<<curLine<<": illegal parameter!"<<endl;
            if(list(token))
            {
                token = nextToken(file);
                if(closeParenthesis(token[0]))
                    token = nextToken(file);
            }
            else
            {
                bool operand = true;
                checkNumAssignment(file, symbolTable, token, operand);
            }

        }
        else
            token = nextToken(file);
    }
    else
    {
        validCode = false;
        cout<<line<<": numread is a function"<<endl;
    }
}